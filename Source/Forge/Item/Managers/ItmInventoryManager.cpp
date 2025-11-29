#include "ItmInventoryManager.h"

#include "Blueprint/UserWidget.h"
#include "Forge/Item/Components/ItmEquipmentComponent.h"
#include "Forge/Item/Components/ItmInventoryComponent.h"
#include "Forge/Item/Generation/ItmItemGeneratorSubsystem.h"
#include "Forge/Item/UI/ItmInventoryRootWidget.h"

void UItmInventoryManager::Initialize(APlayerController* InPlayerController, UItmInventoryComponent* InInventoryComp,
	UItmEquipmentComponent* InEquipmentComp, TSubclassOf<UItmInventoryRootWidget> InInventoryWidgetClass)
{
	check(InPlayerController && InInventoryComp && InEquipmentComp && InInventoryWidgetClass);

	PlayerController = InPlayerController;
	InventoryComp = InInventoryComp;
	EquipmentComp = InEquipmentComp;

	InventoryWidget = CreateWidget<UItmInventoryRootWidget>(PlayerController, InInventoryWidgetClass);
	InventoryWidget->AddToViewport(1);

	InventoryWidget->OnGridSlotClicked.AddDynamic(this, &UItmInventoryManager::HandleGridSlotClicked);
	InventoryWidget->OnGridSlotHovered.AddDynamic(this, &UItmInventoryManager::HandleGridSlotHovered);
	InventoryWidget->OnGridSlotUnhovered.AddDynamic(this, &UItmInventoryManager::HandleGridSlotUnhovered);

	InventoryWidget->OnEquipmentSlotClicked.AddDynamic(this, &UItmInventoryManager::HandleEquipmentSlotClicked);
	InventoryWidget->OnEquipmentSlotHovered.AddDynamic(this, &UItmInventoryManager::HandleEquipmentSlotHovered);
	InventoryWidget->OnEquipmentSlotUnhovered.AddDynamic(this, &UItmInventoryManager::HandleEquipmentSlotUnhovered);

	InventoryWidget->OnClickedOutsideInventory.AddDynamic(this, &UItmInventoryManager::HandleClickedOutsideInventory);
	
	InInventoryComp->OnGridSlotItemChanged.AddDynamic(this, &UItmInventoryManager::HandleGridSlotItemChanged);
}

void UItmInventoryManager::HandleGridSlotClicked(int32 SlotIndex)
{
	FItmInventoryEntry NewGridSlotItem { InventoryComp->GetHeldItem() };
	FItmInventoryEntry NewHeldItem { InventoryComp->GetGridSlotItem(SlotIndex) };
	
	InventoryComp->SetHeldItem(NewHeldItem);
	InventoryComp->SetGridSlotItem(SlotIndex, NewGridSlotItem);
	
	InventoryWidget->RefreshGridSlot(SlotIndex, NewGridSlotItem);
	InventoryWidget->RefreshHeldItem(NewHeldItem);
}

void UItmInventoryManager::HandleGridSlotHovered(int32 SlotIndex)
{
	InventoryWidget->RefreshGridSlotTooltip(SlotIndex, InventoryComp->GetGridSlotItem(SlotIndex));
}

void UItmInventoryManager::HandleGridSlotUnhovered(int32 SlotIndex)
{
	InventoryWidget->RefreshGridSlotTooltip(SlotIndex, FItmInventoryEntry::Empty);
}

void UItmInventoryManager::HandleEquipmentSlotClicked(EItmEquipmentSlot EquipmentSlot)
{
	const FItmItemInstance HeldItem = InventoryComp->GetHeldItem().Item;

	// Check that item has same type as the equipment slot
	if (HeldItem.IsValid() && !EquipmentComp->CanEquipItem(EquipmentSlot, HeldItem))
		return;

	// Unequip item
	FItmItemInstance UnequippedItem { FItmItemInstance::Empty };
	if (!EquipmentComp->IsSlotEmpty(EquipmentSlot))
		UnequippedItem = EquipmentComp->UnequipItem(EquipmentSlot);	

	// Equip held item
	EquipmentComp->EquipItem(EquipmentSlot, HeldItem);

	// Change held item to the previous equipment slot item
	FItmInventoryEntry HeldEntry;
	HeldEntry.Item = UnequippedItem;
	HeldEntry.Quantity = UnequippedItem.IsValid() ? 1 : 0;	
	InventoryComp->SetHeldItem(HeldEntry);

	// Refresh UI
	InventoryWidget->RefreshEquipmentSlot(EquipmentSlot, HeldItem);
	InventoryWidget->RefreshHeldItem(HeldEntry);
}

void UItmInventoryManager::HandleEquipmentSlotHovered(EItmEquipmentSlot EquipmentSlot)
{
	InventoryWidget->RefreshEquipmentSlotTooltip(EquipmentSlot, EquipmentComp->GetEquippedItem(EquipmentSlot));
}

void UItmInventoryManager::HandleEquipmentSlotUnhovered(EItmEquipmentSlot EquipmentSlot)
{
	InventoryWidget->RefreshEquipmentSlotTooltip(EquipmentSlot, FItmItemInstance::Empty);
}

void UItmInventoryManager::HandleGridSlotItemChanged(int32 SlotIndex, const FItmInventoryEntry& NewItem)
{
	InventoryWidget->RefreshGridSlot(SlotIndex, NewItem);
}

void UItmInventoryManager::HandleClickedOutsideInventory()
{
	if (InventoryComp->HasHeldItem())
	{
		// Place the item in the world
		if (UItmItemGeneratorSubsystem* ItemGenerator = PlayerController->GetGameInstance()->GetSubsystem<UItmItemGeneratorSubsystem>())
			ItemGenerator->SpawnWorldItem(InventoryComp->GetHeldItem().Item, PlayerController->GetPawn()->GetActorLocation());
		
		InventoryComp->SetHeldItem(FItmInventoryEntry::Empty);
		InventoryWidget->RefreshHeldItem(FItmInventoryEntry::Empty);		
	}
}