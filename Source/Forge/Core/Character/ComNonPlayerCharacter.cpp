#include "ComNonPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Forge/Core/GAS/ComAbilitySystemComponent.h"
#include "Forge/Core/GAS/ComCombatAttributeSet.h"
#include "Forge/Item/Components/ItmEquipmentComponent.h"
#include "Forge/Item/Components/ItmInventoryComponent.h"
#include "Forge/Item/Generation/ItmItemGeneratorSubsystem.h"
#include "Forge/Item/Managers/ItmInventoryManager.h"

AComNonPlayerCharacter::AComNonPlayerCharacter()
{
	// Increase update frequency for GAS components
	SetNetUpdateFrequency(100.0f);
	AbilitySystemComp = CreateDefaultSubobject<UComAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	CombatAttributeSet = CreateDefaultSubobject<UComCombatAttributeSet>(TEXT("CombatAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComCombatAttributeSet>(CombatAttributeSet);

	CombatAttributeSet->OnHealthChanged.AddDynamic(this, &AComNonPlayerCharacter::HandleHealthChanged);
}

void AComNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComp->InitAbilityActorInfo(this, this);

	check(InitialGameplayEffect);
	AbilitySystemComp->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 1.0f, AbilitySystemComp->MakeEffectContext());
}

UAbilitySystemComponent* AComNonPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp; 
}

void AComNonPlayerCharacter::HandleHealthChanged(AActor* EffectInstigator, float OldValue, float NewValue)
{
	if (NewValue <= 0.f && OldValue >= 0.f)
		Die();
}

void AComNonPlayerCharacter::Die()
{
	// Stop AI behavior tree
	if (const AAIController* AiController = Cast<AAIController>(GetController()))
		AiController->BrainComponent->StopLogic(TEXT("Dead"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	float LifeSpawn = 3.0f;
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(DeathMontage);
		LifeSpawn = DeathMontage->GetPlayLength() + 1.5f;

		// Stop animation after death montage to avoid the character to go back on his feet with the animation blueprint.
		// TODO: find why a cleaner way to stop anim earlier than using a 0.25f delay before stopping animations
		FTimerHandle TimerHandle;
		const auto StopAnim = [this](){ GetMesh()->bPauseAnims = true; };
		GetWorldTimerManager().SetTimer(TimerHandle, StopAnim, DeathMontage->GetPlayLength() - 0.25f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AComNonPlayerCharacter: Death montage has not been set"));
	}

	if (UItmItemGeneratorSubsystem* ItemGenerator = GetGameInstance()->GetSubsystem<UItmItemGeneratorSubsystem>())
		ItemGenerator->TrySpawnItem(GetActorLocation());
	
	// Destroy after delay or ragdoll
	SetLifeSpan(LifeSpawn);
}

