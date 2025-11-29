#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItmInventoryGridSlotWidget.generated.h"

struct FItmInventoryEntry;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridSlotEvent, int32, SlotIndex);

// Widget displaying an inventory slot that can contain an item inside the inventory grid 
UCLASS()
class FORGE_API UItmInventoryGridSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnClicked;
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnHovered;
	UPROPERTY(BlueprintAssignable)
	FOnGridSlotEvent OnUnhovered;
	
	void Refresh(const FItmInventoryEntry& NewItem);

	void Initialize(const int32 NewSlotIndex) { SlotIndex = NewSlotIndex; };
	
	void SetBorderVisibility(bool bShowTop, bool bShowBottom, bool bShowLeft, bool bShowRight);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotImage { nullptr };

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BottomBorderImage { nullptr };
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TopBorderImage { nullptr };
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> LeftBorderImage { nullptr };
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RightBorderImage { nullptr };

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityLabel { nullptr };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSlateBrush DefaultSlotBrush;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SlotIndex { -1 };
	
	virtual void NativeConstruct() override;

	void SetSlot(const FItmInventoryEntry& Item);
	
	void ClearSlot();
	
	void SetQuantityLabel(const int32 Quantity);

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
};