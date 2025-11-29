#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Engine/DataAsset.h"
#include "AaiAiConfigData.generated.h"

UCLASS()
class FORGE_API UAaiAiConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	// NonPlayerCharacter sight
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	float SightRadius { 800.f };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	float LoseSightRadius { 900.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange { 100.f };
	
	// Blackboard key name used to access blackboard data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName TargetActorKeyName { "TargetActor" };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName SpawnLocationKeyName { "SpawnLocation" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName AttackRangeKeyName { "AttackRange" };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName IsInAttackRangeKeyName { "IsInAttackRange" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard")
	FName RangeQueryCircleRadiusKeyName { "RangeQueryCircleRadius" };
	
	UPROPERTY(EditDefaultsOnly, Category="Blackboard")
	FName MoveToLocationKeyName { "MoveToLocation" };
	
	UPROPERTY(EditDefaultsOnly, Category="Blackboard")
	FName IsMoveToLocationBlockedKeyName { "IsMoveToLocationBlocked" };

	UPROPERTY(EditDefaultsOnly, Category="Blackboard")
	FName IsMeleeCharacterKeyName { "IsMeleeCharacter" };
};
