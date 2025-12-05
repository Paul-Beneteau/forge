#pragma once

#include "CoreMinimal.h"
#include "ComGameplayAbility.h"
#include "ComMeleeAbility.generated.h"

UCLASS()
class FORGE_API UComMeleeAbility : public UComGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
