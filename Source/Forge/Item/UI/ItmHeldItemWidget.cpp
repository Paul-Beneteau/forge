#include "ItmHeldItemWidget.h"

#include "Components/Image.h"
#include "Forge/Item/Data/ItmItemInstance.h"

void UItmHeldItemWidget::Refresh(const FItmInventoryEntry& NewItem)
{
	if (NewItem.IsValid())
	{
		check(NewItem.Item.ItemBase.Icon);
		ItemImage->SetBrushFromTexture(NewItem.Item.ItemBase.Icon);
		SetVisibility(ESlateVisibility::HitTestInvisible);	
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItmHeldItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ItemImage);
	
	FSlateBrush Brush = ItemImage->GetBrush();
	Brush.ImageSize = ItemImageSize;
	Brush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	
	ItemImage->SetBrush(Brush);
	ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}