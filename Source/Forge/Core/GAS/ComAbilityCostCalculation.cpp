#include "ComAbilityCostCalculation.h"

#include "ComProjectileAbility.h"

float UComAbilityCostCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UComGameplayAbility* Ability { Cast<UComGameplayAbility>(Spec.GetEffectContext().GetAbility()) };
	
	if (Ability == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UComAbilityCostCalculation: Can't find context ability"));
		return 0.0f;
	}

	// Return -Cost because we subtract instead of adding the cost to the attribute 
	return -Ability->GetCost();
}
