#pragma once

#include "CoreMinimal.h"
#include "MapTypes.generated.h"

UENUM(BlueprintType)
enum class EMapDirection : uint8
{
	None,
	North,
	East,
	South,
	West
};

// Possible rotation of EMapDirection
UENUM(BlueprintType)
enum class EMapRotation : uint8
{
	None,
	Degree0,
	Degree90,
	Degree180,
	Degree270
};

// Graph 2d coordinates
USTRUCT(BlueprintType)
struct FMapGraphCoord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Column = 0;

	// Default constructor needed by Unreal engine
	FMapGraphCoord() = default;	
	FMapGraphCoord(int32 InRow, int32 InColumn)
	: Row(InRow), Column(InColumn)
	{ }

	bool operator==(const FMapGraphCoord& Other) const { return Row == Other.Row && Column == Other.Column; }

	// Returns a new coordinate moved by Distance steps in the given direction (e.g., (0,0).Stepped(East, 2) returns (0,2))
	FMapGraphCoord Stepped(const EMapDirection Direction, const int32 Distance) const;

	// Sentinel value representing an invalid/uninitialized coordinate
	static const FMapGraphCoord None;
};

// Role of the tile (Corridor, river, wall, etc.)
UENUM(BlueprintType)
enum class EMapConnectorType : uint8
{
	None,
	Path,
	Wall,	
	River,
	Bridge,
};

// Connector linking a cell to an adjacent cell.
USTRUCT(BlueprintType)
struct FMapConnector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapDirection Direction = EMapDirection::None;
    
	// Type of the connection (Path, Wall, River, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapConnectorType Type = EMapConnectorType::None;
    
	// Visual theme (Grass, Rocks, Trees, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Theme = NAME_None;

	FMapConnector() = default;
    
	FMapConnector(EMapDirection InDirection, EMapConnectorType InType, FName InTheme)
		: Direction(InDirection)
		, Type(InType)
		, Theme(InTheme)
	{}

	FORCEINLINE bool IsValid() const { return Direction != EMapDirection::None && Type != EMapConnectorType::None; }

	bool operator==(const FMapConnector& Other) const { return Direction == Other.Direction && Type == Other.Type && Theme == Other.Theme; }
};