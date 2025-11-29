#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "ItmEquipmentComponent.generated.h"

enum class EItmEquipmentSlot : uint8;

// Manage equipment possessed by the owner and apply their attributes
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORGE_API UItmEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Equip the item in the associated equipment slot and apply his attributes to the owner
	void EquipItem(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& Item);

	// Remove the item from the associated equipment slot and remove his attributes applied to the owner
	FItmItemInstance UnequipItem(const EItmEquipmentSlot EquipmentSlot);
	
	// Get the currently equipped item
	FItmItemInstance GetEquippedItem(const EItmEquipmentSlot EquipmentSlot) const;

	// Check if the item type matches the equipment slot type
	bool CanEquipItem(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& Item) const;

	bool IsSlotEmpty(const EItmEquipmentSlot EquipmentSlot) const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EItmEquipmentSlot, FItmItemInstance> EquippedItems;

	// Apply item attributes when an item is equipped
	void ApplyItemAttributes(const FItmItemInstance& Item);

	// Remove item attributes when an item is unequipped
	void RemoveItemAttributes(const FItmItemInstance& Item);
};
