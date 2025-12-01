#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Graph/MapGraph.h"
#include "MapGeneratorConfig.h"
#include "MapGenerator.generated.h"

class UMapGeneratorConfig;
class AMapTile;
struct FMapTileSpawnData;
class UMapTileSelector;
class UMapGraphGenerator;
struct FMapGraph;

// Procedural map generator. Generate a graph with GraphGenerator, then select tile to spawn with TileSelector, then spawned tiles.
UCLASS(Blueprintable)
class FORGE_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapGenerator();

	// Generate a procedural map from layout config data table assigned to GraphGenerator.
	UFUNCTION(BlueprintCallable)
	void GenerateMap();
	// Destroy spawned tiles
	UFUNCTION(BlueprintCallable)
	void ClearMap();

	UFUNCTION(BlueprintCallable)
	const FMapGraph& GetCachedGraph() { return CachedMapGraph; }; 
	
	void MovePlayerToStart();
	
	// Show the graph used to generate the current map
	UFUNCTION(BlueprintCallable)
	void ShowGraph() const;
	UFUNCTION(BlueprintCallable)
	void HideGraph() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Map Generation")
	TObjectPtr<UMapGeneratorConfig> GeneratorConfig;
	
	UPROPERTY()
	TObjectPtr<UMapGraphGenerator> GraphGenerator;
    
	UPROPERTY()
	TObjectPtr<UMapTileSelector> TileSelector;

	UPROPERTY(EditDefaultsOnly, Category = "Map Generation")
	int32 TileSize = 1000;

	UPROPERTY()
	TArray<TObjectPtr<AMapTile>> SpawnedTiles;

	FMapGraph CachedMapGraph;

	virtual void BeginPlay() override;	
};
