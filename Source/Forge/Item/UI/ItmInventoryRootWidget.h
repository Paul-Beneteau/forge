// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItmEquipmentSlotWidget.h"
#include "ItmInventoryGridSlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "ItmInventoryRootWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedOutsideInventory);

class UItmClickCatcherWidget;
class UUniformGridPanel;
class UItmItemTooltipWidget;
class UItmEquipmentSlotWidget;
class UItmInventoryGridWidget;
class UItmHeldItemWidget;

// Root widget used to layout every inventory widgets
UCLASS()
class FORGE_API UItmInventoryRootWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnGridSlotClicked;
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnGridSlotHovered;	
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnGridSlotUnhovered;
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotEvent OnEquipmentSlotClicked;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotEvent OnEquipmentSlotHovered;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotEvent OnEquipmentSlotUnhovered;

	UPROPERTY(BlueprintAssignable)
	FOnClickedOutsideInventory OnClickedOutsideInventory;
	
	void RefreshGridSlot(const int32 SlotIndex, const FItmInventoryEntry& NewItem);

	void RefreshEquipmentSlot(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& NewItem);
	
	void RefreshHeldItem(const FItmInventoryEntry& NewItem);

	void RefreshGridSlotTooltip(const int32 SlotIndex, const FItmInventoryEntry& NewItem);
	
	void RefreshEquipmentSlotTooltip(const EItmEquipmentSlot EquipmentSlot, const FItmItemInstance& NewItem);

	// Callback to bind with player left click. Used to remove items from inventory
	UFUNCTION()
	void HandleClickOutside();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmInventoryGridWidget> InventoryGrid;

	// Equipment slot widgets stored in TMap<EItmEquipmentSlot, UItmEquipmentSlotWidget*> EquipmentSlotWidgets;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> ChestSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> BeltSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> BootsSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> GlovesSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> HelmetSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> AmuletSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> PrimaryRingSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> SecondaryRingSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> PrimaryWeaponSlotWidget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UItmEquipmentSlotWidget> SecondaryWeaponSlotWidget;
	
	UPROPERTY()
	TMap<EItmEquipmentSlot, UItmEquipmentSlotWidget*> EquipmentSlotWidgets;

	UPROPERTY()
	TObjectPtr<UItmHeldItemWidget> HeldItemWidget { nullptr };
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItmHeldItemWidget> HeldItemWidgetClass { nullptr };
	
	UPROPERTY()
	TObjectPtr<UItmItemTooltipWidget> ItemTooltipWidget { nullptr };
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItmItemTooltipWidget> ItemTooltipWidgetClass { nullptr };
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void HandleGridSlotClicked(const int32 SlotIndex);
	UFUNCTION()
	void HandleGridSlotHovered(const int32 SlotIndex);
	UFUNCTION()
	void HandleGridSlotUnhovered(const int32 SlotIndex);
	
	UFUNCTION()
	void HandleEquipmentSlotClicked(EItmEquipmentSlot EquipmentSlot);
	UFUNCTION()
	void HandleEquipmentSlotHovered(EItmEquipmentSlot EquipmentSlot);
	UFUNCTION()
	void HandleEquipmentSlotUnhovered(EItmEquipmentSlot EquipmentSlot);
};
