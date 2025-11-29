#pragma once

#include "CoreMinimal.h"
#include "ItmInventoryGridSlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "ItmInventoryGridWidget.generated.h"

class UItmItemTooltipWidget;
class UItmInventoryRootWidget;
class UItmInventoryGridSlotWidget;
struct FItmInventoryEntry;
class UUniformGridPanel;

// Widget representing the inventory grid containing items
UCLASS()
class FORGE_API UItmInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnGridSlotClicked;
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnGridSlotHovered;	
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnGridSlotUnhovered;
	
	void RefreshGridSlot(const int32 SlotIndex, const FItmInventoryEntry& NewItem);

	FVector2D GetSlotPosition(const int32 SlotIndex);
	FVector2D GetSlotSize(const int32 SlotIndex);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel { nullptr };
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItmInventoryGridSlotWidget> GridSlotClass { nullptr };
	
	int32 ColumnCount { 12 };	
	int32 RowCount { 5 };

	UPROPERTY()
	TObjectPtr<UItmItemTooltipWidget> ItemTooltipWidget { nullptr };
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItmItemTooltipWidget> ItemTooltipWidgetClass { nullptr };
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleGridSlotClicked(const int32 SlotIndex);
	UFUNCTION()
	void HandleGridSlotHovered(const int32 SlotIndex);
	UFUNCTION()
	void HandleGridSlotUnhovered(const int32 SlotIndex);
};
