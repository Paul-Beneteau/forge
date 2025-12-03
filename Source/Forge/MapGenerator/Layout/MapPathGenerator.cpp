#include "MapPathGenerator.h"

#include "VectorUtil.h"

FMapPathGenerator::FMapPathGenerator(const FMapPathConfig& InPathConfig, const FMapPathConstraints& InPathConstraints)
	: PathConfig(InPathConfig),
	PathConstraints(InPathConstraints)
{	
}

TArray<FMapSegment> FMapPathGenerator::GeneratePath(const FMapPathConfig& InPathConfig, const FMapPathConstraints& InPathConstraints)
{	
	FMapPathGenerator PathGenerator(InPathConfig, InPathConstraints);

	// Pick a random layout
	if (InPathConfig.Layout == EMapPathLayout::Random)
	{
		TArray<EMapPathLayout> PossibleLayouts = { EMapPathLayout::Straight, EMapPathLayout::L, EMapPathLayout::Stairs, EMapPathLayout::U };
		PathGenerator.PathConfig.Layout = PossibleLayouts[FMath::RandRange(0, PossibleLayouts.Num() - 1)];
	}
		
	switch (PathGenerator.PathConfig.Layout)
	{		
	case EMapPathLayout::Straight:
		return PathGenerator.GenerateStraightPath();
		
	case EMapPathLayout::L:
		return PathGenerator.GenerateLPath();

	case EMapPathLayout::U:
		return PathGenerator.GenerateUPath();

	case EMapPathLayout::Stairs:
		return PathGenerator.GenerateStairsPath();
		
	case EMapPathLayout::Square:
		return PathGenerator.GenerateSquarePath();
		
	default:
		return TArray<FMapSegment>();
	}
}

TArray<FMapSegment> FMapPathGenerator::GenerateStraightPath()
{
	TArray<FMapSegment> Path;
	
	FMapSegment Segment = GenerateSegment(PathConstraints.Start, PathConstraints.StartDirection);	
	if (!Segment.IsValid())
		return TArray<FMapSegment>();
	
	Path.Add(Segment);	
	return Path;
}

TArray<FMapSegment> FMapPathGenerator::GenerateLPath()
{	
	// L Shape is composed of 2 segments
	TArray<FMapSegment> Path;
	
	FMapSegment FirstSegment = GenerateSegment(PathConstraints.Start, PathConstraints.StartDirection);	
	if (!FirstSegment.IsValid())
		return TArray<FMapSegment>();

	// Second segment turns perpendicular from the first segment's end. AnchorOffset = 1 steps one cell in the new direction from the anchor
	// (the corner cell) 
	FMapSegment SecondSegment =	GenerateSegmentFromDirections(
		FirstSegment.End(),
		MapUtils::Perpendicular(FirstSegment.Direction),
		1);
	
	if (!SecondSegment.IsValid())
		return TArray<FMapSegment>();
	
	Path.Add(FirstSegment);
	Path.Add(SecondSegment);
	
	return Path;
}

TArray<FMapSegment> FMapPathGenerator::GenerateUPath()
{
	// U Shape is L Shape with Third segment with opposite direction of first segment
	TArray<FMapSegment> Path = GenerateLPath();	
	if (Path.IsEmpty())
		return TArray<FMapSegment>();

	const FMapSegment& FirstSegment = Path[0];
	const FMapSegment& SecondSegment = Path[1];
	
	FMapSegment ThirdSegment = GenerateSegment(
		SecondSegment.End(),
		MapUtils::Opposite(FirstSegment.Direction),
		1);
	
	if (!ThirdSegment.IsValid())
		return TArray<FMapSegment>();

	Path.Add(ThirdSegment);	
	return Path;
}

TArray<FMapSegment> FMapPathGenerator::GenerateStairsPath()
{
	// Stairs Shape L Shape with a Third segment mirroring first segment with opposite direction
	TArray<FMapSegment> Path = GenerateLPath();	
	if (Path.IsEmpty())
		return TArray<FMapSegment>();

	const FMapSegment& FirstSegment = Path[0];
	const FMapSegment& SecondSegment = Path[1];
	
	FMapSegment ThirdSegment = GenerateSegment(SecondSegment.End(), FirstSegment.Direction,1);	
	if (!ThirdSegment.IsValid())
		return TArray<FMapSegment>();

	Path.Add(ThirdSegment);	
	return Path;
}

TArray<FMapSegment> FMapPathGenerator::GenerateSquarePath()
{
	// L Shape is composed of 2 segments
	TArray<FMapSegment> Path;
	
	FMapSegment FirstSegment = GenerateSegment(PathConstraints.Start, PathConstraints.StartDirection);	
	if (!FirstSegment.IsValid())
		return TArray<FMapSegment>();

	// Second segment turns perpendicular from the first segment's end. AnchorOffset = 1 steps one cell in the new direction from the anchor
	// (the corner cell) 
	FMapSegment SecondSegment =	GenerateSegmentFromDirections(
		FirstSegment.End(),
		MapUtils::Perpendicular(FirstSegment.Direction),
		1,
		FirstSegment.Length - 1);
	
	if (!SecondSegment.IsValid())
		return TArray<FMapSegment>();

	FMapSegment ThirdSegment =	GenerateSegmentFromDirections(
	SecondSegment.End(),
	{ MapUtils::Opposite(FirstSegment.Direction) },
	1,
	FirstSegment.Length - 1);
	
	if (!ThirdSegment.IsValid())
		return TArray<FMapSegment>();

	FMapSegment FourthSegment =	GenerateSegmentFromDirections(
	ThirdSegment.End(),
	{ MapUtils::Opposite(SecondSegment.Direction) },
	1,
	FirstSegment.Length - 2);
	
	if (!FourthSegment.IsValid())
		return TArray<FMapSegment>();
	
	Path.Add(FirstSegment);
	Path.Add(SecondSegment);
	Path.Add(ThirdSegment);
	Path.Add(FourthSegment);
	
	return Path;
}
FMapSegment FMapPathGenerator::GenerateSegmentFromDirections(
	const FMapGraphCoord Anchor,
	const TArray<EMapDirection>& PossibleDirections,
	const int32 AnchorOffset /* = 0 */,
	const int32 Length /* = -1 */)
{
	FMapSegment Segment;
	Segment.Length = Length > 0 ? Length : FMath::RandRange(PathConfig.SegmentMinLength, PathConfig.SegmentMaxLength);
	Segment.Theme = PathConfig.Theme;
    
	TArray<EMapDirection> ValidDirections;

	// Fill ValidDirections
	for (EMapDirection Direction : PossibleDirections)
	{
		FMapGraphCoord PossibleStart = Anchor.Stepped(Direction, AnchorOffset);
		int32 MaxSegmentLength = PathConstraints.GetMaxSegmentLength(PossibleStart, Direction);

		// If maximum segment length is superior to the segment length or if the segment can be truncated
		if (MaxSegmentLength >= Segment.Length || (PathConfig.bCanBeTruncated && MaxSegmentLength > 0))
			ValidDirections.Add(Direction);		
	}
    
	if (ValidDirections.IsEmpty())
		return FMapSegment();

	Segment.Direction = ValidDirections[FMath::RandRange(0, ValidDirections.Num() - 1)];
	Segment.Start = Anchor.Stepped(Segment.Direction, AnchorOffset);

	// Truncate segment if needed
	int32 MaxSegmentLength = PathConstraints.GetMaxSegmentLength(Segment.Start, Segment.Direction);
	if (MaxSegmentLength < Segment.Length)
	{
		check(PathConfig.bCanBeTruncated && MaxSegmentLength > 0);
		Segment.Length = MaxSegmentLength;
	}
	
	return Segment;
}

FMapSegment FMapPathGenerator::GenerateSegment(
	const FMapGraphCoord Anchor,
	const EMapDirection Direction,
	const int32 AnchorOffset /* = 0 */,
	const int32 Length /* = -1 */)
{
	return GenerateSegmentFromDirections(Anchor, { Direction }, AnchorOffset, Length );
}