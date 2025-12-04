#include "ComProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "ComBaseProjectile.h"
#include "ComDamageModifierAttributeSet.h"
#include "Forge/Core/Character/ComPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UComProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	// Spawn a projectile in the direction of the cursor click
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		FHitResult Hit;
		
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit))
		{			
			// Clear bOrientRotationToMovement so that character can rotate. Best way to rotate would be to use animation
			Character->GetCharacterMovement()->bOrientRotationToMovement = false;

			const FRotator CharacterRotation = (Hit.Location - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
			
			// Rotate toward click location
			PlayerController->SetControlRotation(CharacterRotation);

			// Spawn projectile. Loop to wait until character finish rotating
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UComProjectileAbility::OnCharacterRotated, 0.1f, true);
		}
	} // AI character abilities doesn't need to rotate before. TODO: Find a clean way to handle this
	else if (AController* AiController = Cast<AController>(Character->GetController()))
	{				
		SpawnProjectiles(1);
	
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);		
	}
}

// Wait for character to rotate before spawning projectiles
void UComProjectileAbility::OnCharacterRotated()
{
	AComPlayerCharacter* Character = CastChecked<AComPlayerCharacter>(GetAvatarActorFromActorInfo());

	// Wait until character finish rotating
	if (Character->ReachedDesiredRotation() == false)
		return;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;	

	// Add the AdditionalProjectile modifier that can increase number of projectile
	SpawnProjectiles(1 + Character->DamageAttributeSet->GetAdditionalProjectile());
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

// Compute the direction of each projectile and spawn them
void UComProjectileAbility::SpawnProjectiles(int32 ProjectilesCount)
{

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());

	TArray<FRotator> ProjectileRotations = GetProjectileRotations(ProjectilesCount);

	for (FRotator ProjectileRotation : ProjectileRotations)
	{		
		FVector ProjectileLocation = Character->GetMesh()->GetSocketLocation(SpawnSocketName);
		
		AComBaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<AComBaseProjectile>(ProjectileClass,
			FTransform(ProjectileRotation, ProjectileLocation),nullptr, Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (Projectile)
		{
			Projectile->Initialize(GameplayEffectClass, this);
			Projectile->FinishSpawning(FTransform(ProjectileRotation, ProjectileLocation));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UComProjectileAbility: Projectile didn't spawn"));
			return;
		}
	}
}

// Compute the rotation of each projectile in an array. A default spread is applied (ConeProjectileSpread) when there
// is less than 12 projectiles, then projectiles are spawn in a circle
TArray<FRotator> UComProjectileAbility::GetProjectileRotations(int32 ProjectilesCount)
{
	check (ProjectilesCount > 0);

	const ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	
	TArray<FRotator> ProjectileRotations;

	if (ProjectilesCount == 1)
	{
		ProjectileRotations.Add(Character->GetActorRotation());
		return ProjectileRotations;
	}

	FRotator FirstProjectileRotation = Character->GetActorRotation();

	// If there is less than 12 projectile, the spread is a predefined spread (15 degree). Else projectiles are spawn
	// in a circle so the spread is 360 / ProjectilesCount.
	float ProjectileSpread = ProjectilesCount < 12 ? ConeProjectileSpread : 360 / ProjectilesCount;
	
	if (ProjectilesCount < 12)
	{		
		// The total angle. each projectile is seperated by 15 degree
		float TotalSpread = (ProjectilesCount - 1) * ProjectileSpread;

		// The character rotation is at the middle of the spread. So we can subtract half of the spread to get the first
		// projectile, or add it to get the last projectile
		FirstProjectileRotation -= FRotator(0.0f, TotalSpread * 0.5, 0.0);
	}
	
	for (int32 i = 0; i < ProjectilesCount; ++i)
		ProjectileRotations.Add(FirstProjectileRotation + FRotator(0.0f, ProjectileSpread * i, 0.0f));
	
	return ProjectileRotations;
}
