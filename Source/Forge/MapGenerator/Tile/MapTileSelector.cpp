#include "MapTileSelector.h"

#include "MapTileTemplate.h"
#include "Forge/MapGenerator/MapGraphUtils.h"
#include "Forge/MapGenerator/Graph/MapGraph.h"

void UMapTileSelector::Initialize(const TArray<UMapTileTemplate*>& InTileTemplates)
{
	TileTemplates = InTileTemplates;
}

TArray<FMapTileSpawnData> UMapTileSelector::SelectTiles(const FMapGraph& Graph)
{
	TArray<FMapTileSpawnData> TileSpawnData;
	TileSpawnData.Reserve(Graph.Rows * Graph.Columns);
	
	for (int32 Row = 0; Row < Graph.Rows; ++Row)
	{
		for (int32 Column = 0; Column < Graph.Columns; ++Column)
		{			
			const FMapGraphCoord CurrentCoord(Row, Column);
			const FMapGraphCell CurrentCell = Graph.At(CurrentCoord);
			
			if (!CurrentCell.IsUsed())
				continue;
			
			const UMapTileTemplate* Template = PickTemplateForCell(CurrentCell);
			if (!Template)
			{
				UE_LOG(LogTemp, Error, TEXT("UMapTileSelector: couldn't find a valid template for the cell: %d-%d"), Row, Column);
				continue;
			}

			const FRotator TileRotation = PickTemplateRotationForCell(*Template, CurrentCell);
			
			TileSpawnData.Add(FMapTileSpawnData(Template->TileClass, CurrentCoord, TileRotation));
		}
	}

	return TileSpawnData;
}

UMapTileTemplate* UMapTileSelector::PickTemplateForCell(const FMapGraphCell& Cell)
{
	TArray<UMapTileTemplate*> Templates = GetMatchingTemplates(Cell);
	if (Templates.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("UMapTileSelector: No valid tile templates found for cell"));
		return nullptr;
	}	

	auto GetWeight = [](const UMapTileTemplate* Template) -> float { return Template ? Template->Weight : 0.f; };    
	return MapUtils::PickWeightedRandom<UMapTileTemplate>(Templates, GetWeight);
}

FRotator UMapTileSelector::PickTemplateRotationForCell(const UMapTileTemplate& Template, const FMapGraphCell& Cell) const
{
	const TArray<FRotator> MatchingRotations = GetMatchingRotations(&Template, Cell);
	return MatchingRotations[FMath::RandRange(0, MatchingRotations.Num() - 1)];
}

TArray<UMapTileTemplate*> UMapTileSelector::GetMatchingTemplates(const FMapGraphCell& Cell)
{
	TArray<UMapTileTemplate*> Matches;

	for (UMapTileTemplate* Template : TileTemplates)
	{		
		if (DoesTemplateMatchCell(Template, Cell))
			Matches.Add(Template);
	}

	return Matches;
}

namespace 
{
	TArray<FMapConnector> RotateConnectors(TArray<FMapConnector> Connectors, EMapRotation Rotation)
	{
		TArray<FMapConnector> RotatedConnectors;
		
		for (FMapConnector& Connector : Connectors)
		{
			Connector.Direction = MapUtils::RotateClockwise(Connector.Direction, Rotation);
			RotatedConnectors.Add(Connector);
		}

		return RotatedConnectors;
	}
	
	// Check if Connectors array has all RequiredConnectors.
	bool HasAllConnectors(const TArray<FMapConnector>& Connectors, const TArray<FMapConnector>& RequiredConnectors)
	{		
		for (const FMapConnector& Connector : RequiredConnectors)
		{			
			if (!Connectors.Contains(Connector))
				return false;
		}

		return true;
	}
}

bool UMapTileSelector::DoesTemplateMatchCell(const UMapTileTemplate* Template, const FMapGraphCell& Cell) const
{
	// If template theme doesn't match cell theme or if they don't have the same number of connectors
	if (!Template || Template->Theme != Cell.Theme || Cell.Connectors.Num() != Template->GetConnectors().Num())
		return false;
	
	// For each possible rotation of the template tile
	for (EMapRotation Rotation : { EMapRotation::Degree0, EMapRotation::Degree90, EMapRotation::Degree180, EMapRotation::Degree270 })
	{		
		TArray<FMapConnector> RotatedTemplateConnectors = RotateConnectors(Template->GetConnectors(), Rotation);

		// check if the tile has cell connectors
		if (HasAllConnectors(RotatedTemplateConnectors, Cell.Connectors))
			return true;
	}

	return false;
}

TArray<FRotator> UMapTileSelector::GetMatchingRotations(const UMapTileTemplate* Template, const FMapGraphCell& Cell) const
{
	TArray<FRotator> MatchingRotations;

	// If template theme doesn't match cell theme or if they don't have the same number of connectors
	if (!Template || Template->Theme != Cell.Theme || Cell.Connectors.Num() != Template->GetConnectors().Num())
		return MatchingRotations;
	
	// Check if the candidate can fit with the 4 possible clockwise rotation (0°, 90°, 180°; 270°) of the tile.
	for (const EMapRotation Rotation : { EMapRotation::Degree0, EMapRotation::Degree90, EMapRotation::Degree180, EMapRotation::Degree270 })
	{
		TArray<FMapConnector> RotatedTemplateConnectors = RotateConnectors(Template->GetConnectors(), Rotation);

		// If rotated template has all required cell connectors
		if (HasAllConnectors(RotatedTemplateConnectors, Cell.Connectors))
		{
			// We rotated the template clockwise, so apply that rotation to the tile
			FRotator TileRotation(0.f, MapUtils::RotationToYaw(Rotation), 0.f);
			MatchingRotations.Add(TileRotation);
		}
	}

	return MatchingRotations;
}