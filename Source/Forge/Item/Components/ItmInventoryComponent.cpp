#include "Forge/Item/Components/ItmInventoryComponent.h"

UItmInventoryComponent::UItmInventoryComponent()
{
	GridItems.SetNum(ColumnCount * RowCount);
}

bool UItmInventoryComponent::AddItemToGridSlot(const FItmInventoryEntry& Item, const int32 SlotIndex)
{
	if (!Item.IsValid() || !GridItems.IsValidIndex(SlotIndex))
		return false;

	GridItems[SlotIndex] = Item;
	
	OnGridSlotItemChanged.Broadcast(SlotIndex, Item);
	
	return true;
}

bool UItmInventoryComponent::AddItemToFirstAvailableGridSlot(const FItmInventoryEntry& Item)
{
	if (!Item.IsValid())
		return false;

	for (int32 SlotIndex = 0; SlotIndex < GridItems.Num(); ++SlotIndex)
	{
		if (!GridItems[SlotIndex].IsValid())
		{
			GridItems[SlotIndex] = Item;
			OnGridSlotItemChanged.Broadcast(SlotIndex, Item);
			return true;
		}
	}
	
	return false;
}
