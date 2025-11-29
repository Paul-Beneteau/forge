#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComDataTableRow.generated.h"

// Used by a data table to store every damage type (fire, lightning, projectile, attack, spell etc )
USTRUCT()
struct FComDamageModifierRow : public FTableRowBase
{
	GENERATED_BODY()

	// Optional tag that can be required to do damage (e.g. Increased fire damage attribute need an ability with fire
	// damage type tag, More damage doesn't need a required tag as it is applied for every abilities)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TOptional<FGameplayTag> RequiredTag;

	// Attribute from UComDamageModifierAttributeSet representing damage modifier (e.g. increased cold damage)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute DamageModifierAttribute;
};

// Store character GAS attribute in a data table
USTRUCT()
struct FComAttributeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute Attribute;
};
