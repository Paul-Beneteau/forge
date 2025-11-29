#include "ItmEquipmentSlotWidget.h"

#include "ItmInventoryRootWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "Forge/Item/Data/ItmItemBase.h"

void UItmEquipmentSlotWidget::Refresh(const FItmItemInstance& NewItem)
{
	if (NewItem.IsValid())
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(NewItem.ItemBase.Icon);
		Brush.TintColor = FSlateColor(FLinearColor::White);

		SlotImage->SetBrush(Brush);
		EquipmentLabel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SlotImage->SetBrush(DefaultSlotBrush);
		EquipmentLabel->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UItmEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(EquipmentSlotType != EItmEquipmentSlot::None);
	
	SlotImage->SetBrush(DefaultSlotBrush);
}

FReply UItmEquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnClicked.Broadcast(EquipmentSlotType);
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

void UItmEquipmentSlotWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	OnHovered.Broadcast(EquipmentSlotType);
}

void UItmEquipmentSlotWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);

	OnUnhovered.Broadcast(EquipmentSlotType);
}