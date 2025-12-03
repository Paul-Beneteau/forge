#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "ItmEquipmentSlotWidget.generated.h"

class UTextBlock;
class UItmInventoryRootWidget;
enum class EItmItemType : uint8;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotEvent, EItmEquipmentSlot, EquipmentSlot);

// Represent an equipment slot in the inventory. It has a type EItmEquipmentSlot (chest, primary ring, etc.) to differentiate
// each slot and to represent which items can be equipped in the slot
UCLASS()
class FORGE_API UItmEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotEvent OnClicked;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotEvent OnHovered;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotEvent OnUnhovered;
	
	void Refresh(const FItmItemInstance& NewItem);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotImage = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> EquipmentLabel = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItmEquipmentSlot EquipmentSlotType = EItmEquipmentSlot::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSlateBrush DefaultSlotBrush;
	
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
};
