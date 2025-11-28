#include "MapGraphUtils.h"

#include "Graph/MapGraph.h"
#include "Forge/MapGenerator/Types/MapTypes.h"
#include "Layout/MapLayout.h"

namespace MapUtils
{
	// Convert a direction to yaw with the north being 0 for convention
	FORCEINLINE int32 DirectionToYaw(EMapDirection Direction)
	{
		switch(Direction)
		{
		case EMapDirection::North:
			return 0;
			
		case EMapDirection::East:
			return 90;
			
		case EMapDirection::South:
			return 180;
			
		case EMapDirection::West:
			return 270;
			
		default:
			return 0;
		}
	}

	FORCEINLINE EMapRotation YawToRotation(int32 Yaw)
	{
		// Normalize yaw between 0 and 360
		Yaw = Yaw % 360;
		if (Yaw < 0)
			Yaw += 360;
		
		switch(Yaw)
		{
		case 0:
			return EMapRotation::Degree0;
			
		case 90:
			return EMapRotation::Degree90;
			
		case 180:
			return EMapRotation::Degree180;
			
		case 270:
			return EMapRotation::Degree270;
			
		default:
			return EMapRotation::None;
		}
	}

	int32 RotationToYaw(EMapRotation Rotation)
	{
		switch(Rotation)
		{
		case EMapRotation::Degree0:
			return 0;
			
		case EMapRotation::Degree90:
			return 90;
			
		case EMapRotation::Degree180:
			return 180;
			
		case EMapRotation::Degree270:
			return 270;
			
		default:
			return 0;
		}
	}

	// Convert a yaw to a direction with the north being 0 for convention
	FORCEINLINE EMapDirection YawToDirection(int32 Yaw)
	{
		// Normalize yaw between 0 and 360
		Yaw = Yaw % 360;
		if (Yaw < 0)
			Yaw += 360;
        
		switch(Yaw)
		{
		case 0:
			return EMapDirection::North;
			
		case 90:
			return EMapDirection::East;
			
		case 180:
			return EMapDirection::South;
			
		case 270:
			return EMapDirection::West;
			
		default:
			return EMapDirection::None;
		}
	}
	
	EMapDirection RotateClockwise(EMapDirection Direction, EMapRotation Rotation)
	{
		if (Direction == EMapDirection::None)
			return EMapDirection::None;
    
   		const int32 Yaw = DirectionToYaw(Direction) + RotationToYaw(Rotation);
		return YawToDirection(Yaw);
	}
		
	EMapRotation GetRotationBetween(EMapDirection FromDirection, EMapDirection TowardDirection)
	{
		if (FromDirection == EMapDirection::None || TowardDirection == EMapDirection::None)
			return EMapRotation::None;
  
		const int32 Yaw = DirectionToYaw(TowardDirection) - DirectionToYaw(FromDirection);    
		return YawToRotation(Yaw);
	}

	EMapRotation GetRotation(EMapBranchRotation BranchRotation)
	{
		switch (BranchRotation)
		{
		case EMapBranchRotation::Random:
			return FMath::RandBool() ? EMapRotation::Degree90 : EMapRotation::Degree270;
			
		case EMapBranchRotation::Degree90:
			return EMapRotation::Degree90;
			
		case EMapBranchRotation::Degree270:
			return EMapRotation::Degree270;
			
		default:
			return EMapRotation::None;
		}
	}

	EMapDirection Opposite(EMapDirection Direction)
	{
		return RotateClockwise(Direction, EMapRotation::Degree180);
	}

	TArray<EMapDirection> Perpendicular(EMapDirection Direction)
	{
		return { RotateClockwise(Direction, EMapRotation::Degree270), RotateClockwise(Direction, EMapRotation::Degree90) };
	}
	
	EMapDirection GetDirectionToward(const FMapGraphCoord& FromCell, const FMapGraphCoord& TowardCell)
	{
		const int32 RowDelta = TowardCell.Row - FromCell.Row;
		const int32 ColumnDelta = TowardCell.Column - FromCell.Column;

		if (RowDelta == 0 && ColumnDelta != 0)
			return ColumnDelta > 0 ? EMapDirection::East : EMapDirection::West;
		
		if (ColumnDelta == 0 && RowDelta != 0)
			return RowDelta > 0 ? EMapDirection::South : EMapDirection::North;
		
		return EMapDirection::None;
	}

	EMapDirection GetInwardDirection(const FMapGraphCoord& Coord, int32 Rows, int32 Columns)
	{
		if (Coord.Row == 0)
			return EMapDirection::South;
		if (Coord.Row == Rows - 1)
			return EMapDirection::North;
		if (Coord.Column == 0)
			return EMapDirection::East;
		if (Coord.Column == Columns - 1)
			return EMapDirection::West;

		return EMapDirection::None;
	}

	bool IsInsideBounds(int32 Rows, int32 Columns, const FMapGraphCoord& Coord)
	{
		return Coord.Row >= 0 && Coord.Row < Rows	&& Coord.Column >= 0 && Coord.Column < Columns;
	}
	
	int32 GetPathLength(const TArray<FMapSegment>& Path)
	{
		int32 PathLength = 0;
		
		for (const FMapSegment& Segment : Path)
			PathLength += Segment.Length;

		return PathLength;
	}
}
