#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ComAbilitySystemComponent.generated.h"

UCLASS()
class FORGE_API UComAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets the base value of an attribute. Make the base class function SetNumericAttribute() exposed to Blueprint.
	// Used by UI to manually add or subtract a value to the attribute dynamically without having to call the specific
	// setter for each attribute. So it isn't impacted if a new attribute is added to the set.
	// Should only be used for by the UI for this purpose, the correct way to modify an attribute is by using gameplay effect.
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void SetNumericAttributeBaseBP(const FGameplayAttribute &Attribute, float NewBaseValue);
};