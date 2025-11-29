#include "ItmInventoryGridSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Forge/Item/Data/ItmItemInstance.h"

void UItmInventoryGridSlotWidget::Refresh(const FItmInventoryEntry& NewItem)
{	
	if (NewItem.IsValid())
		SetSlot(NewItem);
	else
		ClearSlot();
}

void UItmInventoryGridSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlotImage->SetBrush(DefaultSlotBrush);
	SetQuantityLabel(0);
}

void UItmInventoryGridSlotWidget::SetSlot(const FItmInventoryEntry& Item)
{
	check(Item.Item.ItemBase.Icon);

	FSlateBrush SlotBrush;
	SlotBrush.SetResourceObject(Item.Item.ItemBase.Icon);
	SlotBrush.TintColor = FSlateColor(FLinearColor::White);

	SlotImage->SetBrush(SlotBrush);
	
	SetQuantityLabel(Item.Quantity);
}

void UItmInventoryGridSlotWidget::ClearSlot()
{
	SlotImage->SetBrush(DefaultSlotBrush);
	SetQuantityLabel(0);
}

void UItmInventoryGridSlotWidget::SetBorderVisibility(bool bShowTop, bool bShowBottom, bool bShowLeft, bool bShowRight)
{
	TopBorderImage->SetVisibility(bShowTop ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	BottomBorderImage->SetVisibility(bShowBottom ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	LeftBorderImage->SetVisibility(bShowLeft ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	RightBorderImage->SetVisibility(bShowRight ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UItmInventoryGridSlotWidget::SetQuantityLabel(const int32 Quantity)
{
	check(Quantity >= 0);
	QuantityLabel->SetText(FText::AsNumber(Quantity));
	
	if (Quantity == 0)
		QuantityLabel->SetVisibility(ESlateVisibility::Hidden);
	else
		QuantityLabel->SetVisibility(ESlateVisibility::HitTestInvisible);
}

FReply UItmInventoryGridSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnClicked.Broadcast(SlotIndex);
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

void UItmInventoryGridSlotWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);
	OnHovered.Broadcast(SlotIndex);
}

void UItmInventoryGridSlotWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);
	OnUnhovered.Broadcast(SlotIndex);
}
