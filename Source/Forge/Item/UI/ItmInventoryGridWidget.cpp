#include "ItmInventoryGridWidget.h"

#include "ItmInventoryGridSlotWidget.h"
#include "ItmItemTooltipWidget.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UItmInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(GridSlotClass);

	// Create grid slot widgets
	for (int32 Row = 0; Row < RowCount; ++Row)
	{
		for (int32 Column = 0; Column < ColumnCount; ++Column)
		{
			UItmInventoryGridSlotWidget* GridSlotWidget = CreateWidget<UItmInventoryGridSlotWidget>(GetWorld(), GridSlotClass);

			const bool bIsLastRow = Row == RowCount - 1;
			const bool bIsLastColumn = Column == ColumnCount - 1;

			// Hide the bottom/right border if this is not the last row/column to avoid stacking border with neighbor slot widgets
			GridSlotWidget->Initialize( Row * ColumnCount + Column);	
			GridSlotWidget->SetBorderVisibility(true, bIsLastRow, true, bIsLastColumn);		
			GridSlotWidget->OnClicked.AddDynamic(this, &UItmInventoryGridWidget::HandleGridSlotClicked);
			GridSlotWidget->OnHovered.AddDynamic(this, &UItmInventoryGridWidget::HandleGridSlotHovered);
			GridSlotWidget->OnUnhovered.AddDynamic(this, &UItmInventoryGridWidget::HandleGridSlotUnhovered);
			
			UUniformGridSlot* GridSlot  = GridPanel->AddChildToUniformGrid(GridSlotWidget, Row, Column);
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

	check(ItemTooltipWidgetClass);
	ItemTooltipWidget = CreateWidget<UItmItemTooltipWidget>(GetWorld(), ItemTooltipWidgetClass);
	ItemTooltipWidget->AddToViewport(10);
	// Align widget to have his bottom border anchored above grid slot widget 
	ItemTooltipWidget->SetAlignmentInViewport(FVector2D(0.5f, 1.0f));
}

void UItmInventoryGridWidget::RefreshGridSlot(const int32 SlotIndex, const FItmInventoryEntry& NewItem)
{
	check(SlotIndex >= 0 && SlotIndex < GridPanel->GetChildrenCount());
	
	UItmInventoryGridSlotWidget* GridSlotWidget = Cast<UItmInventoryGridSlotWidget>(GridPanel->GetChildAt(SlotIndex));

	GridSlotWidget->Refresh(NewItem);
}

FVector2D UItmInventoryGridWidget::GetSlotPosition(const int32 SlotIndex)
{
	check(SlotIndex >= 0 && SlotIndex < GridPanel->GetChildrenCount());

	UItmInventoryGridSlotWidget* GridSlotWidget = CastChecked<UItmInventoryGridSlotWidget>(GridPanel->GetChildAt(SlotIndex));
	
	return GridSlotWidget->GetTickSpaceGeometry().GetAbsolutePosition();
}

FVector2D UItmInventoryGridWidget::GetSlotSize(const int32 SlotIndex)
{
	check(SlotIndex >= 0 && SlotIndex < GridPanel->GetChildrenCount());

	UItmInventoryGridSlotWidget* GridSlotWidget = CastChecked<UItmInventoryGridSlotWidget>(GridPanel->GetChildAt(SlotIndex));
	
	return GridSlotWidget->GetTickSpaceGeometry().GetAbsoluteSize();
}

void UItmInventoryGridWidget::HandleGridSlotClicked(const int32 SlotIndex)
{
	OnGridSlotClicked.Broadcast(SlotIndex);
}

void UItmInventoryGridWidget::HandleGridSlotHovered(const int32 SlotIndex)
{
	OnGridSlotHovered.Broadcast(SlotIndex);
}

void UItmInventoryGridWidget::HandleGridSlotUnhovered(const int32 SlotIndex)
{
	OnGridSlotUnhovered.Broadcast(SlotIndex);
}