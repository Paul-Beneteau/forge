#pragma once

#include "CoreMinimal.h"
#include "AiSpawner/MapAiSpawner.h"
#include "Engine/DataAsset.h"
#include "MapGeneratorConfig.generated.h"

class UMapTileTemplate;

UCLASS(Blueprintable)
class FORGE_API UMapGeneratorConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Graph Generation")
	int32 Rows { 7 };
	UPROPERTY(EditDefaultsOnly, Category = "Graph Generation")
	int32 Columns { 7 };

	// Configuration for layout generation (layout type, segment lengths, themes, etc.)
	UPROPERTY(EditDefaultsOnly, Category = "Graph Generation")
	TObjectPtr<UDataTable> LayoutConfigTable;
	
	// maximum number of retries if the layout generation fails
	UPROPERTY(EditDefaultsOnly, Category = "Graph Generation")
	int32 MaxLayoutGenerationRetries { 25 };

	// Tile templates available for selection.
	UPROPERTY(EditDefaultsOnly, Category = "Tile Selection")
	TArray<UMapTileTemplate*> TileTemplates;

	UPROPERTY(EditDefaultsOnly, Category = "AI Spawn")
	FMapAiSpawnerConfig AiSpawnerConfig;
};
