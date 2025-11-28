#include "MapTile.h"

AMapTile::AMapTile()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GetComponents<UMapTileConnector>(Connectors);
}

