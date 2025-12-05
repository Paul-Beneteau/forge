#pragma once

#include "CoreMinimal.h"
#include "ComBaseProjectile.h"
#include "ComChainProjectile.generated.h"

// Projectile chaining on other targets after overlapping a target
UCLASS()
class FORGE_API AComChainProjectile : public AComBaseProjectile
{
	GENERATED_BODY()

public:
	virtual void Initialize(TSubclassOf<UGameplayEffect> InHitActorGameplayEffect, TObjectPtr<UGameplayAbility> InInstigatorAbility) override;

	void SetChainParameters(int32 InMaxChains, float InChainRadius);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UParticleSystem> HitActorEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Chain")
	int32 MaxChains = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Chain")
	float ChainRadius = 400.0f;
	int32 RemainingChains = 0;

	UPROPERTY()
	TSet<AActor*> HitActors;

	virtual void HandleActorHit(AActor* HitActor) override;
	virtual bool IsValidTarget(AActor* Actor) const override;

	AActor* FindNextChainTarget(const FVector& CurrentLocation);
	void SpawnChainProjectile(AActor* NextTarget);
};
