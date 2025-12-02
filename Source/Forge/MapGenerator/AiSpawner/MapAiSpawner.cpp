#include "MapAiSpawner.h"

#include "Forge/Core/Character/ComNonPlayerCharacter.h"


TArray<AComNonPlayerCharacter*> UMapAiSpawner::SpawnAiPack(const FMapAiSpawnerConfig& AiSpawnerConfig, const FVector& TileLocation, int32 TileSize)
{
	if (FMath::FRand() > AiSpawnerConfig.ChanceToSpawnPerTile)
		return TArray<AComNonPlayerCharacter*>();
	
	TArray<AComNonPlayerCharacter*> AiSpawned;

	const float SpawnRadius = TileSize * 0.30f;

	check(GetWorld());
	check(AiSpawnerConfig.PackClass);

	int32 PackSize = FMath::RandRange(AiSpawnerConfig.MinPackSize, AiSpawnerConfig.MaxPackSize);
	
	for (int32 Index = 0; Index < PackSize; Index++)
	{
		// Retry if the random location selected is not valid
		for (int32 Retries = 0; Retries < 5; Retries++)
		{
			FVector RandomOffset(FMath::RandRange(-SpawnRadius, SpawnRadius),FMath::RandRange(-SpawnRadius, SpawnRadius),0.f);
			FVector SpawnLocation = TileLocation + RandomOffset;
		
			AComNonPlayerCharacter* AiCharacter = GetWorld()->SpawnActor<AComNonPlayerCharacter>(AiSpawnerConfig.PackClass,	SpawnLocation,
				FRotator::ZeroRotator);
			if (AiCharacter)
			{
				AiSpawned.Add(AiCharacter);
				break;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UMapAiSpawner: Failed to spawn Ai"));
			}
		}
	}

	return AiSpawned;
}
