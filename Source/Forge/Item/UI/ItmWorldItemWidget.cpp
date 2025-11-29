#include "Forge/Item/UI/ItmWorldItemWidget.h"

#include "ItmItemTooltipWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Forge/Item/Data/ItmItemInstance.h"

void UItmWorldItemWidget::Initialize(const FItmItemInstance& InItem)
{
	if (!InItem.IsValid())
		return;
	
	NameLabelWidget->SetText(FText::FromName(InItem.ItemBase.Name));
}

void UItmWorldItemWidget::RefreshTooltip(const FItmItemInstance& Item)
{
	const FVector2D WidgetPosition = GetTickSpaceGeometry().GetAbsolutePosition();
	const FVector2D WidgetSize = GetTickSpaceGeometry().GetAbsoluteSize();
	
	// Tooltip position just below the ItemTooltipWidget
	const FVector2D AbsoluteTooltipPosition = WidgetPosition + FVector2D(WidgetSize.X * 0.5f, WidgetSize.Y);

	// convert absolute to viewport coords for DPI scaling
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), AbsoluteTooltipPosition, PixelPosition, ViewportPosition);

	// Add a small offset to separate the tooltip from the item widget
	FVector2D Offset = FVector2D(0.f, 15.f);
	ItemTooltipWidget->SetPositionInViewport(ViewportPosition + Offset, false);
	
	ItemTooltipWidget->Refresh(Item);
}

void UItmWorldItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ItemTooltipWidgetClass);
	ItemTooltipWidget = CreateWidget<UItmItemTooltipWidget>(GetWorld(), ItemTooltipWidgetClass);
	ItemTooltipWidget->AddToViewport(10);
	// Align widget to have his bottom border anchored above grid slot widget 
	ItemTooltipWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.f));
}

FReply UItmWorldItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnClicked.Broadcast();
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

void UItmWorldItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnHovered.Broadcast();
}

void UItmWorldItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnUnhovered.Broadcast();
}
