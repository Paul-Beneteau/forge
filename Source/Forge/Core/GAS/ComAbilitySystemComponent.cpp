#include "ComAbilitySystemComponent.h"

void UComAbilitySystemComponent::SetNumericAttributeBaseBP(const FGameplayAttribute& Attribute,	float NewBaseValue)
{
	SetNumericAttributeBase(Attribute, NewBaseValue);
}