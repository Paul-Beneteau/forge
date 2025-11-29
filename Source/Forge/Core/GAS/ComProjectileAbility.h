#pragma once

#include "CoreMinimal.h"
#include "ComGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "ComProjectileAbility.generated.h"

UCLASS()
class FORGE_API UComProjectileAbility : public UComGameplayAbility
{
	GENERATED_BODY()

public:
	// Send a projectile toward the player attack click direction
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	// Projectile class spawned
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> ProjectileClass;

	FTimerHandle TimerHandle;

	// Spread between 2 projectiles when there is less than 12 projectiles
	float ConeProjectileSpread { 15.0 };
	
	UFUNCTION()
	void OnCharacterRotated();

	// Spawn the number of projectile given
	void SpawnProjectiles(int32 ProjectilesCount);

	// Calculate every projectile rotations that will be spawned given a projectile count
	TArray<FRotator> GetProjectileRotations(int32 ProjectilesCount);
};
