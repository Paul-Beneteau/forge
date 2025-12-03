#include "MapTypes.h"

// Sentinel value representing an invalid/uninitialized coordinate
const FMapGraphCoord FMapGraphCoord::None(-1, -1);

FMapGraphCoord FMapGraphCoord::Stepped(const EMapDirection Direction, const int32 Distance) const
{
	int32 RowOffset = 0;
	int32 ColumnOffset = 0;
	
	switch (Direction)
	{
	case EMapDirection::North:
		RowOffset -= Distance;
		break;
		
	case EMapDirection::South:
		RowOffset += Distance;
		break;
		
	case EMapDirection::East:
		ColumnOffset += Distance;
		break;
		
	case EMapDirection::West:
		ColumnOffset -= Distance;
		break;

	default:
		break;
	}

	return FMapGraphCoord(Row + RowOffset, Column + ColumnOffset);
}