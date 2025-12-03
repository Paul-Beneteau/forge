#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "ItmInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGridSlotItemChanged, int32, SlotIndex, const FItmInventoryEntry&, NewItem);


// Manage inventory item stored in the inventory grid and the held item
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORGE_API UItmInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotItemChanged OnGridSlotItemChanged;
	
	UItmInventoryComponent();

	const FItmInventoryEntry& GetHeldItem() const { return HeldItem; }

	void SetHeldItem(const FItmInventoryEntry& NewHeldItem) { HeldItem = NewHeldItem; };

	bool HasHeldItem() const { return HeldItem.IsValid(); }
	
	const FItmInventoryEntry& GetGridSlotItem(const int32 SlotIndex) const { return GridItems[SlotIndex]; }

	void SetGridSlotItem(const int32 SlotIndex, const FItmInventoryEntry& NewItem) { GridItems[SlotIndex] = NewItem; }

	UFUNCTION(BlueprintCallable)
	bool AddItemToGridSlot(const FItmInventoryEntry& Item, const int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	bool AddItemToFirstAvailableGridSlot(const FItmInventoryEntry& Item);
	
protected:
	TArray<FItmInventoryEntry> GridItems;

	int32 ColumnCount = 12;

	int32 RowCount = 5;
	
	FItmInventoryEntry HeldItem;
};
