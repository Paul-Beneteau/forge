#include "ComNonPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Forge/Core/GAS/ComAbilitySystemComponent.h"
#include "Forge/Core/GAS/ComCombatAttributeSet.h"

AComNonPlayerCharacter::AComNonPlayerCharacter()
{
	// Increase update frequency for GAS components
	SetNetUpdateFrequency(100.0f);
	AbilitySystemComp = CreateDefaultSubobject<UComAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	CombatAttributeSet = CreateDefaultSubobject<UComCombatAttributeSet>(TEXT("CombatAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComCombatAttributeSet>(CombatAttributeSet);
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

