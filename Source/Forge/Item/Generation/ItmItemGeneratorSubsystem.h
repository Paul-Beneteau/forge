#pragma once

#include "CoreMinimal.h"
#include "ItmItemGeneratorConfig.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItmItemGeneratorSubsystem.generated.h"

class AItmWorldItem;
class UItmLootConfigData;
struct FItmInventoryEntry;

// Generate random item in the world using a data table containing item bases
UCLASS()
class FORGE_API UItmItemGeneratorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TrySpawnItem(const FVector& Location);

	// Generate a random item using ItemBasePool
	UFUNCTION(BlueprintCallable)
	FItmItemInstance GenerateRandomItem() const;
	
	// Spawn an item in the world
	UFUNCTION(BlueprintCallable)
	AItmWorldItem* SpawnWorldItem(const FItmItemInstance& Item, const FVector& Location) const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItmItemGeneratorConfig> ItemGeneratorConfig = nullptr;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FItmItemAttributeTemplate* PickWeightedRandomTemplate(TArray<FItmItemAttributeTemplate>& Templates) const;

	int32 GenerateAttributeCount() const;

	
	// Generate a random item base from the data table ItemBasePool
	FItmItemBase*  GenerateRandomItemBase() const;
};
