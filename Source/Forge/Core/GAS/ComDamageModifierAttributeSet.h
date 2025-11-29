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
