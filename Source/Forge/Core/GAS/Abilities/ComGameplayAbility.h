#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ComGameplayAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityActivated);

UCLASS()
class FORGE_API UComGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UComGameplayAbility();

	// Send a projectile toward the player attack click direction
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	// Make ability tags visible for blueprint
	UFUNCTION(BlueprintCallable, Category="Ability")
	const FGameplayTagContainer& GetAssetTagsBP() const;

	// Get the ability final damage (the gameplay effect modifier magnitude)
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetFinalDamage() const;

	int32 GetBaseDamage() const { return BaseDamage; }

	UFUNCTION(BlueprintCallable, Category="Ability")
	int32 GetCost() const { return Cost; }

protected:
	// Gameplay effect when actor is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 BaseDamage = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 Cost = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAbilityActivated OnAbilityActivated;
};
