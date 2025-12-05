#include "ComDamageModifierAttributeSet.h"

#include "GameplayEffectExtension.h"

void UComDamageModifierAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Check that attribute value can't go below 0. We don't check which attribute it is because it applies to every attributes
	NewValue = FMath::Clamp<float>(NewValue, 0.0f, NewValue);
}