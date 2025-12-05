#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComBaseProjectile.generated.h"

class UCapsuleComponent;
class UGameplayAbility;
struct FGameplayTagContainer;
class UGameplayEffect;
class USphereComponent;
class UBoxComponent;
class UProjectileMovementComponent;

// Base projectile actor. Apply HitActorGameplayEffect when a player actor is hit. HitActorGameplayEffect and InstigatorAbility has to be set by
// the instigator
UCLASS()
class FORGE_API AComBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	AComBaseProjectile();
	
	virtual void Initialize(TSubclassOf<UGameplayEffect> InHitActorGameplayEffect, TObjectPtr<UGameplayAbility> InInstigatorAbility);
	
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCapsuleComponent> CollisionComp;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LifeSpawn = 3.0;
	
	// Effect when projectile is spawned
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> HitWorldParticleEffect;
	// Effect when projectile hit an actor
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> HitActorParticleEffect;

	TSubclassOf<UGameplayEffect> HitActorGameplayEffect;
	UPROPERTY()
	TObjectPtr<UGameplayAbility> InstigatorAbility;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Virtual method that child classes can override for custom hit behavior
	virtual void HandleActorHit(AActor* HitActor);    
	// Helper method to check if an actor is a valid target
	virtual bool IsValidTarget(AActor* Actor) const;
	// Helper method to apply the gameplay effect
	void ApplyGameplayEffect(AActor* TargetActor);
};
