#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AaiAiConfig.generated.h"

class UGameplayEffect;
class UBehaviorTree;

// AI Character configuration
UCLASS()
class FORGE_API UAaiAiConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	// Target detection radius
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	float SightRadius = 800.f;
	// Loosing target detection radius.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	float LoseSightRadius = 900.f;

	// AI Character Attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 100.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRadius = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<UGameplayEffect> AttackEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	// Blackboard key name used to access blackboard data. It has to be key same name from the blackboard used.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName TargetActorKeyName = "TargetActor";	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName SpawnLocationKeyName = "SpawnLocation";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName AttackRangeKeyName = "AttackRange";	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName IsInAttackRangeKeyName = "IsInAttackRange";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName RangeQueryCircleRadiusKeyName = "RangeQueryCircleRadius";	
	UPROPERTY(EditDefaultsOnly, Category="Blackboard")
	FName MoveToLocationKeyName = "MoveToLocation";	
	UPROPERTY(EditDefaultsOnly, Category="Blackboard")
	FName IsMoveToLocationBlockedKeyName = "IsMoveToLocationBlocked";
	UPROPERTY(EditDefaultsOnly, Category="Blackboard")
	FName IsMeleeCharacterKeyName = "IsMeleeCharacter";

	FORCEINLINE bool IsValid() { return BehaviorTree != nullptr; };
};
