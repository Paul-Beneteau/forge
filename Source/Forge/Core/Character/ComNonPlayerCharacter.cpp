#include "ComNonPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Forge/Core/GAS/ComAbilitySystemComponent.h"
#include "Forge/Core/GAS/AttributeSet/ComCombatAttributeSet.h"
#include "Forge/Item/Generation/ItmItemGeneratorSubsystem.h"

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
	// Stop AI behavior
	if (const AAIController* AiController = Cast<AAIController>(GetController()))
	{
		if (AiController->BrainComponent)
			AiController->BrainComponent->StopLogic(TEXT("Dead"));
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TrySpawnItem();
	
	PlayDeathMontage();
	
	SetLifeSpan(DeathDelay);
}

void AComNonPlayerCharacter::PlayDeathMontage()
{    
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !DeathMontage)
		return;
    
	AnimInstance->Montage_Play(DeathMontage);
    
	// Stop animation after death montage to avoid character going back on feet with animation blueprint
	// TODO: Find cleaner way to stop animation without 0.25f delay. Fix death montage.
	FTimerHandle TimerHandle;
	auto StopAnim = [this]() { GetMesh()->bPauseAnims = true; };
	GetWorldTimerManager().SetTimer(TimerHandle,StopAnim ,DeathMontage->GetPlayLength() - 0.25f,false);
}

void AComNonPlayerCharacter::TrySpawnItem()
{	
	if (UItmItemGeneratorSubsystem* ItemGenerator = GetGameInstance()->GetSubsystem<UItmItemGeneratorSubsystem>())
		ItemGenerator->TrySpawnItem(GetActorLocation());
}
