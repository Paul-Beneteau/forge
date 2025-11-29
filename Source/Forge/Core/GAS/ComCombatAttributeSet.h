#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ComCombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, AActor*, EffectInstigator, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPeriodicAttributeChanged, FGameplayAttribute, Attribute, float, EffectModifierMagnitude, FName, EffectName);

UCLASS()
class FORGE_API UComCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, MaxHealth)	
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Damage)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Heal)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, HealthRegen)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, ManaRegen)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, AttackSpeed)
	
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	mutable FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	mutable FOnAttributeChanged OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	mutable FOnPeriodicAttributeChanged OnHealthRegenChanged;
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	mutable FOnPeriodicAttributeChanged OnManaRegenChanged;

	UComCombatAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// Create active effect from periodic attributes. Should be called if you are using periodic attribute (e.g. health/mana regen)
	void InitPeriodicAttributes();

	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> HealthRegenEffect;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData HealthRegen;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ManaRegen;
	
	float HealthBeforeChange { 0.0f };	
	float ManaBeforeChange { 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackSpeed;
	
	// Meta attribute that handle damage applied to the health. TODO: Rename with a more explicit name
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Damage;	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Heal;

	// Period of attribute with infinite duration (eg: health/mana regen)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PeriodicAttribute")
	float AttributePeriod { 1.0f };

	// Contains every active periodic effect with their target attribute
	TMap<FGameplayAttribute, FActiveGameplayEffectHandle> ActivePeriodicEffectMap;
	
	// Apply a periodic gameplay effect with a magnitude to an attribute
	UFUNCTION()
	void UpdateActiveEffect(FGameplayAttribute TargetAttribute, float EffectMagnitude, FName EffectName);
};
