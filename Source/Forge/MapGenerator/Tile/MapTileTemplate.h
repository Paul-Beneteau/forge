#pragma once

#include "CoreMinimal.h"
#include "Forge/MapGenerator/Graph/MapGraph.h"
#include "Engine/DataAsset.h"
#include "MapTileTemplate.generated.h"

class UMapTileConnector;
enum class EMapCellType : uint8;
enum class EMapDirection : uint8;
class AMapTile;

UCLASS(Blueprintable)
class FORGE_API UMapTileTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMapTile> TileClass;

	// Chance to be selected for the map generation
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.f;

	// Visual theme
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Theme;
	
	bool HasConnectors(const TArray<EMapDirection>& Connectors) const;

	TArray<FMapConnector> GetConnectors() const;
}; 
