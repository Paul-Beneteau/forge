# pragma once

#include "CoreMinimal.h"
#include "Forge/MapGenerator/Types/MapTypes.h"
#include "Layout/MapLayout.h"

struct FMapSegment;
struct FMapGraph;
enum class EMapBranchRotation : uint8;

namespace MapUtils
{
	int32 RotationToYaw(EMapRotation Rotation);
	
	// Rotate direction clockwise by converting rotation to yaw angles: North=0°, East=90°, South=180°, West=270°
	// Example: RotateClockwise(North, Degree90) -> East
	EMapDirection RotateClockwise(EMapDirection Direction, EMapRotation Rotation);
	// Get the rotation between two direction
	EMapRotation GetRotationBetween(EMapDirection FromDirection, EMapDirection TowardDirection);
	// Get rotation the EMapBranchRotation enum
	EMapRotation GetRotation(EMapBranchRotation BranchRotation);
	
	// Get opposite direction
	EMapDirection Opposite(EMapDirection Direction);	
	// Get both perpendicular direction
	TArray<EMapDirection> Perpendicular(EMapDirection Direction);
	
	// Get the direction of a cell toward another cell
	EMapDirection GetDirectionToward(const FMapGraphCoord& FromCell, const FMapGraphCoord& TowardCell);
	// Get the direction toward the center of the graph
	EMapDirection GetInwardDirection(const FMapGraphCoord& Coord, int32 Rows, int32 Columns);

	bool IsInsideBounds(int32 Rows, int32 Columns, const FMapGraphCoord& Coord);

	int32 GetPathLength(const TArray<FMapSegment>& Path);

	void PrintGraph(FMapGraph& MapGraph, const UWorld* InWorld, const int32 CellSize);

	// Pick a random element using their weight
	template<typename T>
	T* PickWeightedRandom(const TArray<T*>& Items, TFunction<float(const T*)> GetWeight)
	{
		float TotalWeight = 0.f;		
		for (const T* Item : Items)
		{
			if (Item)
				TotalWeight += GetWeight(Item);
		}

		if (TotalWeight <= 0.0f)
		{
			UE_LOG(LogTemp, Error, TEXT("PickWeightedRandom: Total weight is 0"));
			return nullptr;
		}
		
		float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
		float CurrentWeight = 0.0f;
    
		for (T* Item : Items)
		{
			CurrentWeight += GetWeight(Item);
			if (RandomWeight <= CurrentWeight)
				return Item;
		}
    
		return nullptr;
	}

	template<typename T>
	T PickWeightedRandom(const TArray<T>& Items, TFunction<float(const T&)> GetWeight)
	{
		float TotalWeight = 0.f;		
		for (const T& Item : Items)
			TotalWeight += GetWeight(Item);

		if (TotalWeight <= 0.0f)
		{
			UE_LOG(LogTemp, Error, TEXT("PickWeightedRandom: Total weight is 0"));
			return T();
		}
		
		float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
		float CurrentWeight = 0.0f;
    
		for (const T& Item : Items)
		{
			CurrentWeight += GetWeight(Item);
			if (RandomWeight <= CurrentWeight)
				return Item;
		}
    
		return T();
	}	
}

