#pragma once

#include "CoreMinimal.h"
#include "Forge/Item/UI/ItmInventoryRootWidget.h"
#include "ItmInventoryManager.generated.h"

class UItmInventoryRootWidget;
class UItmInventoryComponent;
class UItmEquipmentComponent;
class UAbilitySystemComponent;
struct FItmInventoryEntry;
enum class EItmEquipmentSlot : uint8;

// Manage player inventory and associated widgets. Listen for player interaction with widgets and reacts to these events.
UCLASS()
class FORGE_API UItmInventoryManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* InPlayerController, UItmInventoryComponent* InInventoryComp, UItmEquipmentComponent* InEquipmentComp,
		TSubclassOf<UItmInventoryRootWidget> InInventoryWidgetClass);

	UItmInventoryRootWidget* GetInventoryRoot() { return InventoryWidget; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory() { InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible); };
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideInventory() { InventoryWidget->SetVisibility(ESlateVisibility::Hidden); };
	
protected:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY()
	TObjectPtr<UItmInventoryComponent> InventoryComp = nullptr;
	UPROPERTY()
	TObjectPtr<UItmEquipmentComponent> EquipmentComp = nullptr;
	UPROPERTY()
	TObjectPtr<UItmInventoryRootWidget> InventoryWidget = nullptr;
	
	UFUNCTION()
	void HandleGridSlotClicked(int32 SlotIndex);
	UFUNCTION()
	void HandleGridSlotHovered(int32 SlotIndex);
	UFUNCTION()
	void HandleGridSlotUnhovered(int32 SlotIndex);

	UFUNCTION()
	void HandleEquipmentSlotClicked(EItmEquipmentSlot EquipmentSlot);
	UFUNCTION()
	void HandleEquipmentSlotHovered(EItmEquipmentSlot EquipmentSlot);
	UFUNCTION()
	void HandleEquipmentSlotUnhovered(EItmEquipmentSlot EquipmentSlot);

	UFUNCTION()
	void HandleGridSlotItemChanged(int32 SlotIndex, const FItmInventoryEntry& NewItem);

	UFUNCTION()
	void HandleClickedOutsideInventory();
};
