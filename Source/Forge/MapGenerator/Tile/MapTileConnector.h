#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Forge/MapGenerator/Types/MapTypes.h"
#include "MapTileConnector.generated.h"

class AMapTile;

// Connector representing an entry between two rooms.
UCLASS(ClassGroup=(Map), meta=(BlueprintSpawnableComponent))
class FORGE_API UMapTileConnector : public UArrowComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapConnector Connector;
};
