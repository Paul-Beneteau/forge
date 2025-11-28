#pragma once

#include "CoreMinimal.h"
#include "Forge/MapGenerator/Layout/MapLayout.h"
#include "Forge/MapGenerator/Types/MapTypes.h"
#include "MapGraph.generated.h"

struct FMapGraphCoord;
enum class EMapDirection : uint8;

// Cell of the map graph. It has a TileType (What the cell represent, corridor, etc.), a theme (visual style) and connectors to links adjacent cells
USTRUCT(BlueprintType)
struct FMapGraphCell
{
	GENERATED_BODY()
	
	// Visual theme (grass, water, etc.)
	UPROPERTY(BlueprintReadWrite)
	FName Theme = NAME_None;
	
	// Links with adjacent cells (path, river, wall connectors)
	UPROPERTY(BlueprintReadWrite)
	TArray<FMapConnector> Connectors;
	
	bool IsUsed() const { return !Theme.IsNone(); };
};

// Represents the map graph as a 2D grid of cells.
USTRUCT(BlueprintType)
struct FMapGraph
{
	GENERATED_BODY()

public:
	TArray<FMapGraphCell> Cells;

	UPROPERTY(BlueprintReadOnly)
	int32 Rows = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Columns = 0;

	FMapGraphCoord MainPathStart = FMapGraphCoord(0, 0);
	FMapGraphCoord MainPathEnd = FMapGraphCoord(0, 0);

	// Metadata used to display information on the generated graph for widgets
	UPROPERTY(BlueprintReadOnly)
	EMapPathLayout MainPathLayout = EMapPathLayout::None;
	UPROPERTY(BlueprintReadOnly)
	int32 MainPathLength = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 BranchesCount = 0;
	
	FMapGraph() = default;	
	FMapGraph(const int32 InRows, const int32 InColumns) : Rows(InRows), Columns(InColumns) { Cells.SetNum(Rows * Columns); };
	
	FORCEINLINE FMapGraphCell& At(const FMapGraphCoord& Coord)	{ return Cells[Coord.Row * Columns + Coord.Column]; }
	FORCEINLINE const FMapGraphCell& At(const FMapGraphCoord& Coord) const { return Cells[Coord.Row * Columns + Coord.Column]; }
	
	FORCEINLINE void Reset() { Cells.Reset(); }
};