#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComAbilityDamageCalculation.generated.h"

class UComGameplayAbility;
struct FComDamageModifierRow;
// Compute the damage of an ability
UCLASS()
class FORGE_API UComAbilityDamageCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	// Contains every additive damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> FlatDamageModifierTable;	
	// Contains every additive damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> AdditiveDamageModifierTable;
	// Contains every multiplicative damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> MultiplicativeDamageModifierTable;
	
	float ComputeFlatDamage(const UAttributeSet* AttributeSet, const UComGameplayAbility* Ability) const;
	float ComputeAdditiveDamage(const UAttributeSet* AttributeSet, const UComGameplayAbility* Ability) const;
	float ComputeMultiplicativeDamage(const UAttributeSet* AttributeSet, const UComGameplayAbility* Ability) const;
	
	bool CanApplyModifier(const UComGameplayAbility* Ability, const FGameplayTag& ModifierRequiredTag) const;
};
