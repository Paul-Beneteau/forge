#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "ItmWorldItem.generated.h"

class UItmWorldItemWidget;

// Represent an item in the world.
UCLASS()
class FORGE_API AItmWorldItem : public AActor
{
	GENERATED_BODY()
	
public:
	AItmWorldItem();
	
	void Initialize(const FItmItemInstance& InItem, TSubclassOf<UUserWidget> InWidgetClass);

protected:
	UPROPERTY(BlueprintReadOnly, Category="Item")
	FItmItemInstance Item;
	
	UPROPERTY()
	TObjectPtr<UItmWorldItemWidget> WorldItemWidget = nullptr;

	TSubclassOf<UItmWorldItemWidget> WorldItemWidgetClass = nullptr;

	FVector2D WidgetCurrentScreenPos = FVector2D::ZeroVector;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleItemWidgetHovered();	
	UFUNCTION()
	void HandleItemWidgetUnhovered();
	UFUNCTION()
	void HandleWorldItemWidgetClicked();
};
