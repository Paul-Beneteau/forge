#include "ItmCrafterComponent.h"

#include "Forge/Item/Data/ItmItemBase.h"
#include "Forge/Item/Data/ItmItemInstance.h"

bool UItmCrafterComponent::Craft(FItmItemInstance& CraftedItem, const FItmItemInstance& CrafterItem)
{
	switch (CrafterItem.ItemBase.ItemType)
	{
	case EItmItemType::AugmentOrb:
		return ExaltedOrbCraft(CraftedItem, CrafterItem);
		
	case EItmItemType::ReforgeOrb:
		return ChaosOrbCraft(CraftedItem, CrafterItem);
		
	case EItmItemType::RefineOrb:
		return DivineOrbCraft(CraftedItem, CrafterItem);
		
	default:
		return false;
	}
}

bool UItmCrafterComponent::ExaltedOrbCraft(FItmItemInstance& CraftedItem, const FItmItemInstance& CrafterItem)
{
	if (CraftedItem.Attributes.Num() >= 6)
		return false;
	
	TArray<FItmItemAttributeTemplate> RemainingTemplates = CraftedItem.ItemBase.AttributeTemplates;
	for (FItmItemAttribute ItemAttribute : CraftedItem.Attributes)
	{
		RemainingTemplates.RemoveAll([ItemAttribute](const FItmItemAttributeTemplate& Template)
		{
			return Template.Attribute == ItemAttribute.Attribute;
		});
	}
	
	FItmItemAttributeTemplate* SelectedTemplate = PickWeightedRandomTemplate(RemainingTemplates);
	if (!SelectedTemplate)
		return false;

	CraftedItem.Attributes.Add(SelectedTemplate->GenerateRolledAttribute());

	return true;
}

bool UItmCrafterComponent::ChaosOrbCraft(FItmItemInstance& CraftedItem, const FItmItemInstance& CrafterItem)
{
	CraftedItem.Attributes.Reset();	
	GenerateItemAttributes(CraftedItem);
	
	return true;
}

bool UItmCrafterComponent::DivineOrbCraft(FItmItemInstance& CraftedItem, const FItmItemInstance& CrafterItem)
{
	for (FItmItemAttribute& ItemAttribute : CraftedItem.Attributes)
	{
		// Find Template of ItemAttribute and reroll attribute
		for (const FItmItemAttributeTemplate& Template : CraftedItem.ItemBase.AttributeTemplates)
		{
			if (Template.Attribute == ItemAttribute.Attribute && Template.ModifierOp == ItemAttribute.ModifierOp)
			{
				ItemAttribute = Template.GenerateRolledAttribute();
				break;
			}
		}
	}

	return true;
}

int32 UItmCrafterComponent::GenerateAttributeCount()
{
	TArray<float> Weights = { 10, 8, 5, 3, 2, 1 };

	float TotalWeight = 0.0f;
	for (float Weight : Weights)
		TotalWeight += Weight;

	const float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (int32 i = 0; i < Weights.Num(); ++i)
	{
		CurrentWeight += Weights[i];
		if (RandomWeight <= CurrentWeight)
			return i + 1;
	}

	return 1;
}

void UItmCrafterComponent::GenerateItemAttributes(FItmItemInstance& Item)
{
	constexpr int32 MaxAttributesCount = 6;
	
	// Generate the number of attributes for the item
	const int32 RandomAttributesCount = GenerateAttributeCount();
	
	TArray<FItmItemAttributeTemplate> RemainingTemplates = Item.ItemBase.AttributeTemplates;
    
	for (int32 AttributeIndex = 0; AttributeIndex < RandomAttributesCount && !RemainingTemplates.IsEmpty(); ++AttributeIndex)
	{
		FItmItemAttributeTemplate* SelectedTemplate = PickWeightedRandomTemplate(RemainingTemplates);
		if (!SelectedTemplate)
			break;

		// Generate attribute with a rolled value
		Item.Attributes.Add(SelectedTemplate->GenerateRolledAttribute());

		// Remove the used template to avoid duplicates
		RemainingTemplates.RemoveAll([SelectedTemplate](const FItmItemAttributeTemplate& Template)
		{
			return Template.Attribute == SelectedTemplate->Attribute;
		});
	}
}

FItmItemAttributeTemplate* UItmCrafterComponent::PickWeightedRandomTemplate(TArray<FItmItemAttributeTemplate>& Templates)
{
	if (Templates.IsEmpty())
		return nullptr;

	float TotalWeight = 0.0f;
	for (const FItmItemAttributeTemplate& Template : Templates)
		TotalWeight += Template.Weight;

	if (TotalWeight <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UItmItemGeneratorSubsystem: Total weight is 0, picking random template"));
		return &Templates[FMath::RandRange(0, Templates.Num() - 1)];
	}

	const float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (FItmItemAttributeTemplate& Template : Templates)
	{
		CurrentWeight += Template.Weight;
		if (RandomWeight <= CurrentWeight)
			return &Template;
	}

	return &Templates.Last();
}
