#include "MapGenerator.h"

#include "Graph/MapGraphGenerator.h"
#include "Tile/MapTile.h"
#include "Tile/MapTileSelector.h"

AMapGenerator::AMapGenerator()
{
	GraphGenerator = CreateDefaultSubobject<UMapGraphGenerator>("GraphGenerator");
	TileSelector = CreateDefaultSubobject<UMapTileSelector>("TileSelector");			
}

// Generates a graph, select tiles for each graph cells and spawn them.
void AMapGenerator::GenerateMap()
{
	ClearMap();
	
	CachedMapGraph = GraphGenerator->GenerateMapGraph();
	
	TArray<FMapTileSpawnData> TilesToSpawn = TileSelector->SelectTiles(CachedMapGraph);
	
	for (const FMapTileSpawnData& TileSpawnData : TilesToSpawn)
	{
		AMapTile* Tile = GetWorld()->SpawnActor<AMapTile>(
			TileSpawnData.TileClass,
			TileSpawnData.GetWorldLocation(TileSize),
			TileSpawnData.Rotation);

		if (!Tile)
			continue;

		SpawnedTiles.Add(Tile);
	}
	
	MovePlayerToStart(CachedMapGraph);
}

void AMapGenerator::ClearMap()
{
	for (AMapTile* Tile : SpawnedTiles)
	{
		if (Tile)
			Tile->Destroy();
	}
    
	SpawnedTiles.Empty();
	CachedMapGraph.Reset();
}

namespace 
{	
	FColor GetCellColor(const FMapGraphCell& Cell, const FMapGraph& MapGraph, FMapGraphCoord CellCoord)
	{
		if (!Cell.IsUsed())
			return FColor::Black;

		if (CellCoord == MapGraph.MainPathStart)
			return FColor::Green;
			
		if (CellCoord == MapGraph.MainPathEnd)
			return FColor::Red;

		// The cell is part of a path, show a brown color
		for (FMapConnector Connector: MapGraph.At(CellCoord).Connectors)
		{
			if (Connector.Type == EMapConnectorType::Path)
				return FColor::Blue;				
		}

		// The cell is part of a branch, show a dark brown color
		if (MapGraph.At(CellCoord).Connectors.Num() > 0)
			return FColor(200, 180, 100);
			
		return FColor::White;
	}

	FVector GetConnectorOffset(EMapDirection Direction, float CellSize)
	{
		const float Offset = CellSize * 0.9f / 2.0f;
	        
		switch (Direction)
		{
		case EMapDirection::North:
			return FVector(Offset, 0, 0);
		case EMapDirection::South:
			return FVector(-Offset, 0, 0);
		case EMapDirection::West:
			return FVector(0, -Offset, 0);
		case EMapDirection::East:
			return FVector(0, Offset, 0);
		default:
			return FVector::ZeroVector;
		}
	}
	    
	void DrawCell(const UWorld* World, const FVector& Location, const FMapGraphCell& Cell, float Size, const FMapGraph& MapGraph, FMapGraphCoord CellCoord)
	{
		const FColor Color = GetCellColor(Cell, MapGraph, CellCoord);
		const FVector HalfSize(Size * 0.99f / 2.0f, Size * 0.99f / 2.0f, 1.0f);
		DrawDebugBox(World, Location, HalfSize, Color, true, 0.f, 0, 10.f);

		FString DebugString = FString::Printf(TEXT("%d-%d"), CellCoord.Row, CellCoord.Column);
		const FVector DebugStringOffset = FVector(0, -50, 0);
		DrawDebugString(World, Location + DebugStringOffset , DebugString, nullptr, Color, -1.f, false, 2);
	}
	    
	void DrawConnectors(const UWorld* World, const FVector& Location, const FMapGraphCell& Cell, float Size)
	{
		for (FMapConnector Connector : Cell.Connectors)
		{
			const FVector Offset = GetConnectorOffset(Connector.Direction, Size);
			DrawDebugLine(World, Location + Offset, Location + (Offset * 1.2f), FColor::Purple, true, 0.f,
				0, 10.f);
		}
	}
}

void AMapGenerator::ShowGraph() const
{
	for (int32 Row = 0; Row < CachedMapGraph.Rows; ++Row)
	{
		for (int32 Column = 0; Column < CachedMapGraph.Columns; ++Column)
		{
			const FMapGraphCell Cell = CachedMapGraph.At(FMapGraphCoord(Row, Column));				
			FVector Location((-Row) * TileSize, Column * TileSize, 10.0f);
			
			DrawCell(GetWorld(), Location, Cell, TileSize, CachedMapGraph, FMapGraphCoord(Row, Column));
            
			if (Cell.IsUsed())
				DrawConnectors(GetWorld(), Location, Cell, TileSize);
		}
	}	
}

void AMapGenerator::HideGraph() const
{
	FlushPersistentDebugLines(GetWorld());
	FlushDebugStrings(GetWorld());
}

void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (GeneratorConfig)
	{
		GraphGenerator->Initialize(
			GeneratorConfig->Rows,
			GeneratorConfig->Columns,
			GeneratorConfig->LayoutConfigTable,
			GeneratorConfig->MaxLayoutGenerationRetries);
		
		TileSelector->Initialize(GeneratorConfig->TileTemplates);
	}
}

void AMapGenerator::MovePlayerToStart(const FMapGraph& Graph)
{
	FMapGraphCoord StartCoord = Graph.MainPathStart;	
	FVector StartLocation(-StartCoord.Row * TileSize, StartCoord.Column * TileSize, 150);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PlayerController->GetPawn())
			Pawn->SetActorLocation(StartLocation);
	}
}
