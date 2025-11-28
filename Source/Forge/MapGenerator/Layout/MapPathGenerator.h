#pragma once

#include "Forge/MapGenerator/Layout/MapLayout.h"
#include "MapPathGenerator.generated.h"

// Generate a map layout composed of the main path and his branches based on FMapLayoutConfig.
USTRUCT(Blueprintable)
struct FMapPathGenerator
{
	GENERATED_BODY()

public:
	FMapPathGenerator() = default;
	FMapPathGenerator(const FMapPathConfig& InPathConfig, const FMapPathConstraints& InPathConstraints);
	
	static TArray<FMapSegment> GeneratePath(const FMapPathConfig& InPathConfig, const FMapPathConstraints& InPathConstraints);

protected:
	FMapPathConfig PathConfig;	
	FMapPathConstraints PathConstraints;
	
	// Layout generators for each shape
	TArray<FMapSegment> GenerateStraightPath();
	TArray<FMapSegment> GenerateLPath();	
	TArray<FMapSegment> GenerateUPath();
	TArray<FMapSegment> GenerateStairsPath();
	TArray<FMapSegment> GenerateSquarePath();

	
	// Generate a segment with one of the direction of PossibleDirections. The segment starting coordinate are the anchor coordinates stepped by the
	// AnchorOffset in the segment direction. If the Length parameter is not specified, a random length is chosen.
	FMapSegment GenerateSegmentFromDirections(
		const FMapGraphCoord Anchor,
		const TArray<EMapDirection>& PossibleDirections,
		const int32 AnchorOffset = 0,
		const int32 Length = -1);

	FMapSegment GenerateSegment(
		const FMapGraphCoord Anchor,
		const EMapDirection Direction,
		const int32 AnchorOffset  = 0 ,
		const int32 Length = -1);
};

