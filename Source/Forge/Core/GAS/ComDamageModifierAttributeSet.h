#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ComDamageModifierAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Attribute set for damage modifiers which can be divided in 3 category
// 1. Flat damage - Added directly to base damage (e.g., +15 added fire damage)
// 2. Additive modifiers - Summed percentages applied together (e.g., +20% fire + +30% projectile = +50% total)
// 3. Multiplicative modifiers - Applied as separate multipliers (e.g., 30% more damage, 20% more spell damage: damage is multiply by 30% then by 20% again)
//
// Final damage formula: (BaseDamage + FlatDamage) * (1 + AdditiveTotal) * MultiplicativeTotal
// Example: (100 base + 15 flat) * (1 + 0.50 additive) * (1.30 * 1.20 multiplicative) = 268 damage
UCLASS()
class FORGE_API UComDamageModifierAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, AddedPhysicalDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, AddedFireDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, AddedLightningDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, AddedColdDamage)	
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedFireDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedLightningDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedColdDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedAttackDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedProjectileDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, MoreDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, AdditionalProjectile)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
protected:
	// Flat Damage
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddedPhysicalDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddedFireDamage;	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddedLightningDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddedColdDamage;
	
	// Additive damage
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedPhysicalDamage;	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedFireDamage;	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedLightningDamage;	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedColdDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedAttackDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedProjectileDamage;
	
	// Multiplicative damage
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoreDamage;

	// Misc modifier

	// Add a number of projectiles spawned par ability
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AdditionalProjectile;
};
