#pragma once

#include "CoreMinimal.h"
#include "MapTileConnector.h"
#include "GameFramework/Actor.h"
#include "MapTile.generated.h"

// Small chunk of a map which can be connected to other room to create a larger level
UCLASS(Blueprintable)
class FORGE_API AMapTile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMapTileConnector>> Connectors;
	
	AMapTile();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
};
