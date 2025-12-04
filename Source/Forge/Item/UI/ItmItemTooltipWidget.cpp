#include "Forge/Item/UI/ItmItemTooltipWidget.h"

#include "Components/TextBlock.h"
#include "Forge/Item/Data/ItmItemInstance.h"

void UItmItemTooltipWidget::Refresh(const FItmItemInstance& NewItem)
{	
	if (NewItem.IsValid())
	{
		NameLabelWidget->SetText(FText::FromName(NewItem.ItemBase.Name));
		
		if (NewItem.Attributes.Num() > 2)
			NameLabelWidget->SetColorAndOpacity(RareItemColor);
		else
			NameLabelWidget->SetColorAndOpacity(MagicItemColor);
		
		SetAttributesLabel(NewItem);
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

namespace
{
	FString FormatAttributeName(const FString& AttributeName)
	{
		if (AttributeName.IsEmpty())
			return AttributeName;

		FString FormattedName;
    
		for (int32 Index = 0; Index < AttributeName.Len(); ++Index)
		{
			const TCHAR CurrentChar = AttributeName[Index];
        
			// Add space before uppercase letters (AddedLightningDamage become Added Lightning Damage)
			if (Index > 0 && FChar::IsUpper(CurrentChar))
				FormattedName += TEXT(" ");
        
			FormattedName += CurrentChar;
		}
    
		return FormattedName;
	}
}

void UItmItemTooltipWidget::SetAttributesLabel(const FItmItemInstance& Item)
{
	FString AttributesText;
	const int32 AttributesLineCount = Item.Attributes.Num();

	for (int32 i = 0; i < AttributesLineCount; ++i)
	{
		const FItmItemAttribute& ItemAttribute = Item.Attributes[i];

		const FString Operator = ItemAttribute.ModifierOp == EGameplayModOp::Additive ? TEXT("+") : TEXT("x");
		const FString AttributeName = FormatAttributeName(Item.Attributes[i].Attribute.GetName());

		const FString AttributeLine = FString::Printf(TEXT("%s%.0f %s"), *Operator, ItemAttribute.Magnitude, *AttributeName);

		AttributesText += AttributeLine;
		
		if (i < AttributesLineCount - 1)
		{
			AttributesText += TEXT("\n");
		}
	}

	AttributesLabelWidget->SetText(FText::FromString(AttributesText));
}

void UItmItemTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}
