#include "ItmInventoryRootWidget.h"

#include "ItmClickCatcherWidget.h"
#include "ItmEquipmentSlotWidget.h"
#include "ItmHeldItemWidget.h"
#include "ItmInventoryGridWidget.h"
#include "ItmItemTooltipWidget.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Button.h"

void UItmInventoryRootWidget::RefreshGridSlot(const int32 SlotIndex, const FItmInventoryEntry& NewItem)
{
	InventoryGrid->RefreshGridSlot(SlotIndex, NewItem);
}

void UItmInventoryRootWidget::RefreshEquipmentSlot(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& NewItem)
{
	UItmEquipmentSlotWidget* EquipmentSlotWidget = *EquipmentSlotWidgets.Find(EquipmentSlot);

	EquipmentSlotWidget->Refresh(NewItem);
}

void UItmInventoryRootWidget::RefreshHeldItem(const FItmInventoryEntry& NewItem)
{
	HeldItemWidget->Refresh(NewItem);
}

void UItmInventoryRootWidget::RefreshGridSlotTooltip(const int32 SlotIndex, const FItmInventoryEntry& NewItem)
{	
	const FVector2D GridSlotWidgetPosition { InventoryGrid->GetSlotPosition(SlotIndex) };
	const FVector2D GridSlotWidgetSize { InventoryGrid->GetSlotSize(SlotIndex)};

	// Tooltip position is just above the grid slot widget with tooltip lower border just above grid slot upper border
	const FVector2D AbsoluteTooltipPosition { GridSlotWidgetPosition + FVector2D(GridSlotWidgetSize.X * 0.5f, 0.f) };

	// convert absolute to viewport coords for DPI scaling
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), AbsoluteTooltipPosition, PixelPosition, ViewportPosition);

	ItemTooltipWidget->SetPositionInViewport(ViewportPosition, false);	
	ItemTooltipWidget->Refresh(NewItem.Item);
}

void UItmInventoryRootWidget::RefreshEquipmentSlotTooltip(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& NewItem)
{
	UItmEquipmentSlotWidget* EquipmentSlotWidget = *EquipmentSlotWidgets.Find(EquipmentSlot);
	check(EquipmentSlotWidget);
	
	const FVector2D WidgetPosition = EquipmentSlotWidget->GetTickSpaceGeometry().GetAbsolutePosition();
	const FVector2D WidgetSize = EquipmentSlotWidget->GetTickSpaceGeometry().GetAbsoluteSize();
	
	// Tooltip position is just above the grid slot widget with tooltip lower border just above grid slot upper border
	const FVector2D AbsoluteTooltipPosition = WidgetPosition + FVector2D(WidgetSize.X * 0.5f, 0.f);

	// convert absolute to viewport coords for DPI scaling
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), AbsoluteTooltipPosition, PixelPosition, ViewportPosition);

	ItemTooltipWidget->SetPositionInViewport(ViewportPosition, false);

	ItemTooltipWidget->Refresh(NewItem);
}

void UItmInventoryRootWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryGrid->OnGridSlotClicked.AddDynamic(this, &UItmInventoryRootWidget::HandleGridSlotClicked);
	InventoryGrid->OnGridSlotHovered.AddDynamic(this, &UItmInventoryRootWidget::HandleGridSlotHovered);
	InventoryGrid->OnGridSlotUnhovered.AddDynamic(this, &UItmInventoryRootWidget::HandleGridSlotUnhovered);

	// Initialize and save EquipmentSlotWidget in a TMap EquipmentSlotWidgets
	EquipmentSlotWidgets =
	{
		{ EItmEquipmentSlot::Chest,			ChestSlotWidget },
		{ EItmEquipmentSlot::Belt,			BeltSlotWidget },
		{ EItmEquipmentSlot::Boots,			BootsSlotWidget },
		{ EItmEquipmentSlot::Gloves,			GlovesSlotWidget },
		{ EItmEquipmentSlot::Helmet,			HelmetSlotWidget },
		{ EItmEquipmentSlot::Amulet,			AmuletSlotWidget },
		{ EItmEquipmentSlot::PrimaryRing,		PrimaryRingSlotWidget },
		{ EItmEquipmentSlot::SecondaryRing,	SecondaryRingSlotWidget },
		{ EItmEquipmentSlot::PrimaryWeapon,	PrimaryWeaponSlotWidget },
		{ EItmEquipmentSlot::SecondaryWeapon,	SecondaryWeaponSlotWidget }
	};

	for (const TPair<EItmEquipmentSlot, UItmEquipmentSlotWidget*>& Pair : EquipmentSlotWidgets)
	{
		if (UItmEquipmentSlotWidget* EquipmentSlotWidget = Pair.Value)
		{
			EquipmentSlotWidget->OnClicked.AddDynamic(this, &UItmInventoryRootWidget::HandleEquipmentSlotClicked);
			EquipmentSlotWidget->OnHovered.AddDynamic(this, &UItmInventoryRootWidget::HandleEquipmentSlotHovered);
			EquipmentSlotWidget->OnUnhovered.AddDynamic(this, &UItmInventoryRootWidget::HandleEquipmentSlotUnhovered);
		}
	}

	check(HeldItemWidgetClass);
	HeldItemWidget = CreateWidget<UItmHeldItemWidget>(GetWorld(), HeldItemWidgetClass);
	HeldItemWidget->SetVisibility(ESlateVisibility::Hidden);
	HeldItemWidget->SetAlignmentInViewport(FVector2D(0.f, 0.f));
	HeldItemWidget->AddToViewport(1000);
	
	check(ItemTooltipWidgetClass);
	ItemTooltipWidget = CreateWidget<UItmItemTooltipWidget>(GetWorld(), ItemTooltipWidgetClass);
	ItemTooltipWidget->AddToViewport(10);
	// Align widget to have his bottom border anchored above grid slot widget 
	ItemTooltipWidget->SetAlignmentInViewport(FVector2D(0.5f, 1.0f));

	check(ClickCatcher);
	ClickCatcher->OnClicked.AddDynamic(this, &UItmInventoryRootWidget::HandleClickOutside);
}

// HeldItemWidget follow cursor position when holding an item
void UItmInventoryRootWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HeldItemWidget)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		check(PlayerController);

		FVector2D MousePosition;
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

		// Add a small padding to separate cursor from held widget
		HeldItemWidget->SetPositionInViewport(MousePosition + 5.f);
	}
}

void UItmInventoryRootWidget::HandleGridSlotClicked(const int32 SlotIndex)
{
	OnGridSlotClicked.Broadcast(SlotIndex);
}

void UItmInventoryRootWidget::HandleGridSlotHovered(const int32 SlotIndex)
{
	OnGridSlotHovered.Broadcast(SlotIndex);
}

void UItmInventoryRootWidget::HandleGridSlotUnhovered(const int32 SlotIndex)
{
	OnGridSlotUnhovered.Broadcast(SlotIndex);
}

void UItmInventoryRootWidget::HandleEquipmentSlotClicked(EItmEquipmentSlot EquipmentSlot)
{
	OnEquipmentSlotClicked.Broadcast(EquipmentSlot);
}

void UItmInventoryRootWidget::HandleEquipmentSlotHovered(EItmEquipmentSlot EquipmentSlot)
{
	OnEquipmentSlotHovered.Broadcast(EquipmentSlot);
}

void UItmInventoryRootWidget::HandleEquipmentSlotUnhovered(EItmEquipmentSlot EquipmentSlot)
{
	OnEquipmentSlotUnhovered.Broadcast(EquipmentSlot);
}

void UItmInventoryRootWidget::HandleClickOutside()
{
	OnClickedOutsideInventory.Broadcast();
}