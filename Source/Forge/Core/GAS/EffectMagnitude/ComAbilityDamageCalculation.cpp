#include "ComAbilityDamageCalculation.h"

#include "AbilitySystemInterface.h"
#include "Forge/Core/GAS/AttributeSet/ComCombatAttributeSet.h"
#include "Forge/Core/GAS/AttributeSet/ComDamageModifierAttributeSet.h"
#include "Forge/Core/GAS/Abilities/ComGameplayAbility.h"
#include "Forge/Core/ComDataTableRow.h"

float UComAbilityDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UComGameplayAbility* Ability = CastChecked<UComGameplayAbility>(Spec.GetEffectContext().GetAbility());
	
	const IAbilitySystemInterface* Instigator = CastChecked<IAbilitySystemInterface>(Spec.GetEffectContext().GetInstigator());	
	const UAbilitySystemComponent* AbilitySystemComp = CastChecked<UAbilitySystemComponent>(Instigator->GetAbilitySystemComponent());	
	const UComDamageModifierAttributeSet* DamageModifierSet = Cast<UComDamageModifierAttributeSet>(
		AbilitySystemComp->GetAttributeSet(UComDamageModifierAttributeSet::StaticClass()));

	float AbilityDamage = Ability->GetBaseDamage();
	
	if (DamageModifierSet)
	{
		float FlatDamage = ComputeFlatDamage(DamageModifierSet, Ability);
		float AdditiveDamage = ComputeAdditiveDamage(DamageModifierSet, Ability);
		float MultiplicativeDamage = ComputeMultiplicativeDamage(DamageModifierSet, Ability);

		AbilityDamage = (AbilityDamage + FlatDamage) * AdditiveDamage * MultiplicativeDamage;
	}
	
	// Remove decimals
	return FMath::RoundToInt32(AbilityDamage);
}

float UComAbilityDamageCalculation::ComputeFlatDamage(const UAttributeSet* AttributeSet, const UComGameplayAbility* Ability) const
{
	TArray<FComDamageModifierRow*> ModifierRows;
	FlatDamageModifierTable->GetAllRows<FComDamageModifierRow>(TEXT(""), ModifierRows);
    
	float Total = 0.0f;
	
	for (const FComDamageModifierRow* Row : ModifierRows)
	{
		if (CanApplyModifier(Ability, Row->RequiredTag.Get(FGameplayTag::EmptyTag)))
			Total += Row->DamageModifierAttribute.GetNumericValueChecked(AttributeSet);
	}
    
	return Total;
}

float UComAbilityDamageCalculation::ComputeAdditiveDamage(const UAttributeSet* AttributeSet, const UComGameplayAbility* Ability) const
{
	TArray<FComDamageModifierRow*> ModifierRows;
	FlatDamageModifierTable->GetAllRows<FComDamageModifierRow>(TEXT(""), ModifierRows);
    
	float Total = 1.0f;
	
	for (const FComDamageModifierRow* Row : ModifierRows)
	{
		if (CanApplyModifier(Ability, Row->RequiredTag.Get(FGameplayTag::EmptyTag)))
		{
			const float ModifierPercent = Row->DamageModifierAttribute.GetNumericValueChecked(AttributeSet);
			Total += ModifierPercent / 100.0f;
		}
	}
    
	return Total;
}

float UComAbilityDamageCalculation::ComputeMultiplicativeDamage(const UAttributeSet* AttributeSet, const UComGameplayAbility* Ability) const
{
	TArray<FComDamageModifierRow*> ModifierRows;
	FlatDamageModifierTable->GetAllRows<FComDamageModifierRow>(TEXT(""), ModifierRows);
    
	float Total = 1.0f;
	
	for (const FComDamageModifierRow* Row : ModifierRows)
	{
		if (CanApplyModifier(Ability, Row->RequiredTag.Get(FGameplayTag::EmptyTag)))
		{
			const float ModifierPercent = Row->DamageModifierAttribute.GetNumericValueChecked(AttributeSet);
			Total *= (1.0f + ModifierPercent / 100.0f);
		}
	}
    
	return Total;
}

bool UComAbilityDamageCalculation::CanApplyModifier(const UComGameplayAbility* Ability, const FGameplayTag& ModifierRequiredTag) const
{	
	if (ModifierRequiredTag == FGameplayTag::EmptyTag)
		return true;
    
	return Ability->GetAssetTags().HasTag(ModifierRequiredTag);
}