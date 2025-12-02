#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataAsset.h"
#include "ItmItemGeneratorConfig.generated.h"

UCLASS()
class FORGE_API UItmItemGeneratorConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Data table containing item bases used to generate items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item generation")
	TObjectPtr<UDataTable> ItemBasePool = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item generation")
	float ChangeToSpawnItem = 0.2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item generation")
	TSubclassOf<UUserWidget> WorldItemWidgetClass = nullptr;

	FORCEINLINE bool IsValid() const { return ItemBasePool && WorldItemWidgetClass; }
};
