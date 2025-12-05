#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapAiSpawner.generated.h"


class AComNonPlayerCharacter;

USTRUCT(BlueprintType)
struct FMapAiPackClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	TSubclassOf<AComNonPlayerCharacter> PackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	float Weight = 1.0f;
};

USTRUCT(BlueprintType)
struct FMapAiSpawnerConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	 TArray<FMapAiPackClass> PackClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	int32 MinPackSize = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	int32 MaxPackSize = 8;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawner")
	float ChanceToSpawnPerTile = 0.2f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORGE_API UMapAiSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	void SpawnAiPack(const FMapAiSpawnerConfig& AiSpawnerConfig, const FVector& TileLocation, int32 TileSize);

	void RemoveSpawnedPacks();

protected:
	
	UPROPERTY()
	TArray<AComNonPlayerCharacter*> SpawnedPack;
};
