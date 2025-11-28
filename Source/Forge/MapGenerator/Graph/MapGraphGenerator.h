#pragma once

#include "CoreMinimal.h"
#include "MapGraph.h"
#include "Forge/MapGenerator/Layout/MapPathGenerator.h"
#include "GameFramework/Actor.h"
#include "MapGraphGenerator.generated.h"

struct FMapBranchConfig;

/**
 * Generates a procedural map graph from a layout configuration.
 * 1. PickLayoutConfig() - Pick a random LayoutConfig from LayoutConfigTable data table
 * 2. CreateMainPath() - Generate the main path and places it into the graph
 * 3. AddBranchesToPath() - Generate branches from the main path
 */
UCLASS(ClassGroup=(Map), meta=(BlueprintSpawnableComponent))
class FORGE_API UMapGraphGenerator : public UActorComponent
{
	GENERATED_BODY()

public:
	// Must be called before generating graphs
	void Initialize(int32 InRows, int32 InColumns, const TObjectPtr<UDataTable>& InLayoutConfigTable, int32 InMaxLayoutRetries);
	
	// Pick a random layout config from LayoutConfigTable, then build the main path his branches.
	FMapGraph GenerateMapGraph();
	
protected:
	int32 Rows { 0 };
	int32 Columns { 0 };

	// Map graph generated
	FMapGraph CachedMapGraph;
	
	// Configuration for layout generation (layout type, segment lengths, themes, etc.)
	UPROPERTY()
	TObjectPtr<UDataTable> LayoutConfigTable;
	
	// maximum number of retries if the layout generation fails
	int32 MaxLayoutGenerationRetries { 25 };

	// Pick a random layout config from LayoutConfigTable
	FMapLayoutConfig PickLayoutConfig();

	// Generate and place the main path on the graph.
	TArray<FMapSegment> CreateMainPath(const FMapPathConfig& PathConfig);
	// Generate the start of the main path on the graph edges
	FMapGraphCoord GenerateMainPathStart() const;

	// Generate and place branches using BranchConfigs to the path
	void AddBranchesToPath(const TArray<FMapSegment>& Path, const TArray<FMapBranchConfig>& BranchConfigs);
	void AddBranchesForConfig(const TArray<FMapSegment>& Path, const FMapBranchConfig& BranchConfig);

	// Fill empty cells in the graph with a FMapTileConfig EmptyCellFillConfig from the LayoutConfig.
	void FillEmptyCells(const FMapLayoutConfig& LayoutConfig);
	
	// Use FMapPathGenerator to generate a path and then place it in the graph
	TArray<FMapSegment> GenerateAndPlacePath(const FMapPathConfig& PathConfig, const FMapPathConstraints& PathConstraints);

	void PlacePath(const TArray<FMapSegment>& Path, const FMapConnectorsConfig& ConnectorsConfig);
	// Place a segment in CachedMapGraph
	void PlaceSegment(const FMapSegment& Segment, const FMapConnectorsConfig& ConnectorsConfig);
	// Place a cell in CachedMapGraph
	void PlaceCell(const FMapGraphCoord Coord, const FMapGraphCell& Cell);

	// Add connectors between the two cells using ConnectorsConfig
	void ConnectCells(const FMapGraphCoord FirstCellCoord, const FMapGraphCoord SecondCellCoord, const FMapConnectorsConfig& ConnectorsConfig);
};
