#pragma once

#include "CoreMinimal.h"
#include "Forge/MapGenerator/Types/MapTypes.h"
#include "GameFramework/Actor.h"
#include "MapTileSelector.generated.h"

struct FMapGraphCell;
struct FMapGraph;
class AMapTile;
class UMapTileTemplate;

// Tile class and world location to spawn the tile
USTRUCT()
struct FMapTileSpawnData
{
	GENERATED_BODY()
    
	UPROPERTY()
	TSubclassOf<AMapTile> TileClass = nullptr;
	
	FMapGraphCoord Coord = FMapGraphCoord::None;

	FRotator Rotation = FRotator::ZeroRotator;

	FORCEINLINE FVector GetWorldLocation(int32 TileSize) const 
	{
		return FVector(-Coord.Row * TileSize, Coord.Column * TileSize, 0);
	}
};

// Selects tile to spawn that match graph cell connection requirements. Uses weighted random selection from candidates tile templates. 
UCLASS(ClassGroup=(Map), meta=(BlueprintSpawnableComponent))
class FORGE_API UMapTileSelector : public UActorComponent
{
	GENERATED_BODY()

public:
	void Initialize(const TArray<UMapTileTemplate*>& InTileTemplates);
	
	// Selects a random valid tile for each used cell in the graph.
	TArray<FMapTileSpawnData> SelectTiles(const FMapGraph& Graph);
	
protected:
	// Tile templates available for selection.
	UPROPERTY()
	TArray<UMapTileTemplate*> TileTemplates;

	// Picks a tile template using weighted random selection from valid tile templates
	UMapTileTemplate* PickTemplateForCell(const FMapGraphCell& Cell);
	// Pick a random matching tile rotation for the cell
	FRotator PickTemplateRotationForCell(const UMapTileTemplate& Template, const FMapGraphCell& Cell) const;
	
	// Get possible valid tile templates for the graph cell
	TArray<UMapTileTemplate*> GetMatchingTemplates(const FMapGraphCell& Cell);

	// Tests template against cell by checking all 4 rotations (0°, 90°, 180°, 270°). Returns true if any rotation has matching connectors
	bool DoesTemplateMatchCell(const UMapTileTemplate* Template, const FMapGraphCell& Cell) const;
	
	// Get template matching rotations with the cell
	TArray<FRotator> GetMatchingRotations(const UMapTileTemplate* Template, const FMapGraphCell& Cell) const;

};
