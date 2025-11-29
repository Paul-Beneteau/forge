#include "ComAbilityDamageCalculation.h"

#include "AbilitySystemInterface.h"
#include "ComCombatAttributeSet.h"
#include "ComDamageModifierAttributeSet.h"
#include "ComGameplayAbility.h"
#include "Forge/Core/ComDataTableRow.h"

float UComAbilityDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const IAbilitySystemInterface* Instigator { Cast<IAbilitySystemInterface>(Spec.GetEffectContext().GetInstigator()) };
	if (Instigator == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UComAbilityDamageCalculation: Can't find gameplay effect instigator"));
		return 0.0f;
	}
	
	const UAbilitySystemComponent* AbilitySystemComp { Instigator->GetAbilitySystemComponent() };
	if (AbilitySystemComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UComAbilityDamageCalculation: Can't find ability system component of instigator"));
		return 0.0f;
	}
	
	const UComDamageModifierAttributeSet* DamageModifierSet { Cast<UComDamageModifierAttributeSet>(AbilitySystemComp->GetAttributeSet(UComDamageModifierAttributeSet::StaticClass())) };
	if (DamageModifierSet == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UComAbilityDamageCalculation: Can't find damage attribute set of instigator"));
		return 0.0f;
	}	

	const UComGameplayAbility* Ability { Cast<UComGameplayAbility>(Spec.GetEffectContext().GetAbility()) };
	
	if (Ability == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UComAbilityDamageCalculation: Can't find gameplay ability instigator"));
		return 0.0f;
	}

	// Retrieve added damage modifiers from data table
	TArray<FComDamageModifierRow*> FlatDamageModifierRows;
	FlatDamageModifierTable->GetAllRows<FComDamageModifierRow>(FString(""), FlatDamageModifierRows);

	// Sum of flat damage added to the ability base damage
	float FlatDamageTotal { 0.0f };
	
	for (FComDamageModifierRow* FlatDamageModifierRow : FlatDamageModifierRows)
	{
		const FGameplayTag RequiredTag { FlatDamageModifierRow->RequiredTag.Get(FGameplayTag::EmptyTag) };
		
		// If there is no required tag or the instigator ability has the required tag 
		if (RequiredTag == FGameplayTag::EmptyTag || (Ability->GetAssetTags().HasTag(RequiredTag)))
		{
			// Add the flat damage attribute from the instigator attribute set
			FlatDamageTotal += FlatDamageModifierRow->DamageModifierAttribute.GetNumericValueChecked(DamageModifierSet);
		}
	}

	// Retrieve additive damage modifiers from data table
	TArray<FComDamageModifierRow*> AdditiveDamageModifierRows;
	AdditiveDamageModifierTable->GetAllRows<FComDamageModifierRow>(FString(""), AdditiveDamageModifierRows);

	// Sum of additive damage modifiers applied
	float AdditiveDamageTotal { 1.0f };
	
	for (FComDamageModifierRow* AdditiveDamageModifierRow : AdditiveDamageModifierRows)
	{
		const FGameplayTag RequiredTag { AdditiveDamageModifierRow->RequiredTag.Get(FGameplayTag::EmptyTag) };
		
		// If there is no required tag or the instigator ability has the required tag 
		if (RequiredTag == FGameplayTag::EmptyTag || (Ability->GetAssetTags().HasTag(RequiredTag)))
		{
			// Add the additive damage attribute from the instigator attribute set
			AdditiveDamageTotal += AdditiveDamageModifierRow->DamageModifierAttribute.GetNumericValueChecked(DamageModifierSet) / 100;
		}
	}

	// Retrieve multiplicative damage modifiers from data table
	TArray<FComDamageModifierRow*> MultiplicativeDamageModifierRows;
	MultiplicativeDamageModifierTable->GetAllRows<FComDamageModifierRow>(FString(""), MultiplicativeDamageModifierRows);

	// Sum of multiplicative damage modifiers applied
	float MultiplicativeDamageTotal { 1 };

	// Iterates over every multiplicative damage modifiers
	for (FComDamageModifierRow* MultiplicativeDamageRow : MultiplicativeDamageModifierRows)
	{
		const FGameplayTag RequiredTag { MultiplicativeDamageRow->RequiredTag.Get(FGameplayTag::EmptyTag) };

		// If there is no required tag or the instigator ability has the required tag, apply the modifer
		if (RequiredTag == FGameplayTag::EmptyTag || (Ability->GetAssetTags().HasTag(RequiredTag)))			
		{
			// Add the multiplicative damage modifer from the instigator attribute set to the sum
			MultiplicativeDamageTotal *= (1 + (MultiplicativeDamageRow->DamageModifierAttribute.GetNumericValueChecked(DamageModifierSet) / 100));
		}
	}
	
	const float Damage { (Ability->GetBaseDamage() + FlatDamageTotal) * AdditiveDamageTotal * MultiplicativeDamageTotal };
	
	// Remove decimals
	return FMath::RoundToInt32(Damage);
}