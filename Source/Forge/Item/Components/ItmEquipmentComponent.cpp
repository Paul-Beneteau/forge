#include "ItmEquipmentComponent.h"
#include "AbilitySystemComponent.h"
#include "Forge/Item/Data/ItmItemBase.h"
#include "Forge/Item/Data/ItmItemInstance.h"

void UItmEquipmentComponent::EquipItem(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& Item)
{
	EquippedItems.Add(EquipmentSlot, Item);
	
	ApplyItemAttributes(Item);
}

FItmItemInstance UItmEquipmentComponent::UnequipItem(const EItmEquipmentSlot EquipmentSlot)
{
	FItmItemInstance UnequippedItem = EquippedItems.FindAndRemoveChecked(EquipmentSlot);
	
	RemoveItemAttributes(UnequippedItem);

	return UnequippedItem;
}

FItmItemInstance UItmEquipmentComponent::GetEquippedItem(const EItmEquipmentSlot EquipmentSlot) const
{
	if (const FItmItemInstance* FoundItem = EquippedItems.Find(EquipmentSlot))
		return *FoundItem;

	return FItmItemInstance::Empty; 
}

bool UItmEquipmentComponent::CanEquipItem(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& Item) const
{
	if (!Item.IsValid())
		return false;

	const EItmItemType ItemType = Item.ItemBase.ItemType;

	switch (EquipmentSlot)
	{
	case EItmEquipmentSlot::Helmet:
		return ItemType == EItmItemType::Helmet;

	case EItmEquipmentSlot::Chest:
		return ItemType == EItmItemType::Chest;

	case EItmEquipmentSlot::Gloves:
		return ItemType == EItmItemType::Gloves;

	case EItmEquipmentSlot::Boots:
		return ItemType == EItmItemType::Boots;

	case EItmEquipmentSlot::Belt:
		return ItemType == EItmItemType::Belt;

	case EItmEquipmentSlot::Amulet:
		return ItemType == EItmItemType::Amulet;

	case EItmEquipmentSlot::PrimaryRing:
		return ItemType == EItmItemType::Ring;
		
	case EItmEquipmentSlot::SecondaryRing:
		return ItemType == EItmItemType::Ring;

	case EItmEquipmentSlot::PrimaryWeapon:
		return ItemType == EItmItemType::Weapon;
		
	case EItmEquipmentSlot::SecondaryWeapon:
		return ItemType == EItmItemType::Weapon;

	default:
		return false;
	}
}

bool UItmEquipmentComponent::IsSlotEmpty(const EItmEquipmentSlot EquipmentSlot) const
{
	return !EquippedItems.Contains(EquipmentSlot);
}

void UItmEquipmentComponent::ApplyItemAttributes(const FItmItemInstance& Item)
{
	UAbilitySystemComponent* AbilitySystemComp = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!AbilitySystemComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UItmEquipmentComponent: AbilitySystemComp no found on %s"), *GetOwner()->GetName());
		return;
	}

	for (const FItmItemAttribute& ItemAttribute : Item.Attributes)
	{
		float AttributeValue = AbilitySystemComp->GetNumericAttribute(ItemAttribute.Attribute);

		switch (ItemAttribute.ModifierOp)
		{
		case EGameplayModOp::Additive:
			AttributeValue += ItemAttribute.Magnitude;
			break;

		case EGameplayModOp::Multiplicitive:
			AttributeValue *= (1.f + ItemAttribute.Magnitude);
			break;

		default:
			UE_LOG(LogTemp, Error, TEXT("UItmEquipmentComponent: Wrong item operator attribute"));
			break;
		}

		AbilitySystemComp->SetNumericAttributeBase(ItemAttribute.Attribute, AttributeValue);
	}
}

void UItmEquipmentComponent::RemoveItemAttributes(const FItmItemInstance& Item)
{
	UAbilitySystemComponent* AbilitySystemComp = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!AbilitySystemComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UItmEquipmentComponent: AbilitySystemComp no found on %s"), *GetOwner()->GetName());
		return;
	}

	for (const FItmItemAttribute& ItemAttribute : Item.Attributes)
	{
		float AttributeValue = AbilitySystemComp->GetNumericAttribute(ItemAttribute.Attribute);

		switch (ItemAttribute.ModifierOp)
		{
		case EGameplayModOp::Additive:
			AttributeValue -= ItemAttribute.Magnitude;
			break;

		case EGameplayModOp::Multiplicitive:
			check(ItemAttribute.Magnitude != 0.f);
			AttributeValue /= ItemAttribute.Magnitude;
			break;

		default:
			UE_LOG(LogTemp, Error, TEXT("UItmEquipmentComponent: Wrong item operator attribute"));
			break;
		}

		AbilitySystemComp->SetNumericAttributeBase(ItemAttribute.Attribute, AttributeValue);
	}
}
