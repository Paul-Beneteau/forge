#pragma once

#include "CoreMinimal.h"
#include "Forge/MapGenerator/MapGraphUtils.h"
#include "Forge/MapGenerator/Types/MapTypes.h"
#include "MapLayout.generated.h"

class UMapTileTemplate;

// =============================================
// Layout structs (Path is an array of segments)
// =============================================

// Represents a straight line of cells in the map graph with a start coordinate, direction, length. It has a type defining what it represents
// (Corridor, river, wall, etc.) and a theme defining visuals.
USTRUCT(BlueprintType)
struct FMapSegment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapGraphCoord Start = FMapGraphCoord::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Length = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapDirection Direction = EMapDirection::None;
	
	// Define visuals
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Theme = "None";
	
	// Get the coordinate of the call at the segment index. Index represent the cell of the segment
	FORCEINLINE FMapGraphCoord GetCoordAt(const int32 Index) const { return Start.Stepped(Direction, Index); }
	
	FORCEINLINE FMapGraphCoord End() const { return Start.Stepped(Direction, Length - 1); }
	
	FORCEINLINE bool IsValid() const
	{
		return Start != FMapGraphCoord::None
		&& Length > 0
		&& Direction != EMapDirection::None;
	};
};

// =======================================
// Layout generation configuration structs
// =======================================

// Shape of the Path
UENUM(BlueprintType)
enum class EMapPathLayout : uint8
{
	None,
	// Pick a random layout
	Random,
	Straight,
	L,
	U,	
	Stairs,
	Square
};

// Configuration to generate connectors
USTRUCT(BlueprintType)
struct FMapConnectorsConfig
{
	GENERATED_BODY()
    
	// Type of the connection (Path, Wall, River, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapConnectorType Type = EMapConnectorType::None;
    
	// Visual theme (Grass, Rocks, Trees, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Theme = NAME_None;
};

// Configuration to generate a path
USTRUCT(BlueprintType)
struct FMapPathConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapPathLayout Layout = EMapPathLayout::None;

	// Length for each segments in the path is randomly picked between those values
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SegmentMinLength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SegmentMaxLength = 0;

	// Visual theme of tiles (Grass, water, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Theme = NAME_None;

	// Configuration to generate connectors between tiles (path, river, wall connectors)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapConnectorsConfig ConnectorsConfig;

	// Can truncate segments if it doesn't fit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeTruncated = false;

	FORCEINLINE bool IsValid() const
	{
		return Layout != EMapPathLayout::None && SegmentMinLength > 0 && SegmentMaxLength >= SegmentMinLength;
	}
};

// Possible branch rotation
UENUM(BlueprintType)
enum class EMapBranchRotation : uint8
{
	None,
	// Pick randomly between Degree90 and Degree270
	Random,
	Degree90,
	Degree270
};

// Configuration to generate a branch from a path
USTRUCT(BlueprintType)
struct FMapBranchConfig
{
	GENERATED_BODY()

	// RotationFromMainPath relative to the main path direction
	UPROPERTY(EditAnywhere)
	EMapBranchRotation RotationFromMainPath = EMapBranchRotation::None;

	// Minimum distance between two branches
	UPROPERTY(EditAnywhere)
	int32 DistanceBetweenBranches = 1;

	// Percent probability of spawn from 0 to 1
	UPROPERTY(EditAnywhere)
	float SpawnProbability = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapPathConfig PathConfig;
};

// Configurations to generate a layout.
USTRUCT(BlueprintType)
struct FMapLayoutConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapPathConfig MainPathConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapBranchConfig> BranchConfigs;

	// Fill empty cell with tiles matching the theme and no connectors.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TOptional<FName> FillTheme;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;

	FORCEINLINE bool IsValid() const { return MainPathConfig.IsValid();	}
};

// Constraint for generating the path.
USTRUCT(BlueprintType)
struct FMapPathConstraints
{
	GENERATED_BODY()

	// Start call of the first segment
	FMapGraphCoord Start;

	// Direction of the first segment
	EMapDirection StartDirection;

	// Bounds of the path. Should be set with the rows - 1 and columns - 1 of the graph. For a graph with 3 rows and 3 columns, bound will be (2, 2)
	// because it goes from index 0 to index 2
	FMapGraphCoord Bounds = FMapGraphCoord(0, 0);

	// Lambda checking is the cell is used. Used in GetMaxSegmentLength()
	TFunction<bool(const FMapGraphCoord&)> IsCellUsed;
	
	bool IsInBounds(const FMapGraphCoord& Coord) const
	{
		return Coord.Row >= 0 && Coord.Row <= Bounds.Row && Coord.Column >= 0 && Coord.Column <= Bounds.Column;
	}
	
	bool IsCellValid(const FMapGraphCoord& Coord) const
	{
		if (!IsInBounds(Coord))
			return false;
        
		if (IsCellUsed)
			return !IsCellUsed(Coord);
        
		return true;
	}

	// Returns maximum valid segment length by stepping in direction until hitting bounds/used cell
	int32 GetMaxSegmentLength(const FMapGraphCoord& StartCoord, EMapDirection Direction)
	{
		int32 Length = 0;
		
		while (IsCellValid(StartCoord.Stepped(Direction, Length)))
			Length++;

		return Length;
	}
};