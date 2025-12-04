// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Forge/Item/Data/ItmItemInstance.h"
#include "ItmWorldItemWidget.generated.h"

struct FItmItemInstance;
class UItmItemTooltipWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldItemEvent);

class UTextBlock;
/**
 * 
 */
UCLASS()
class FORGE_API UItmWorldItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnWorldItemEvent OnClicked;	
	UPROPERTY(BlueprintAssignable)
	FOnWorldItemEvent OnHovered;
	UPROPERTY(BlueprintAssignable)
	FOnWorldItemEvent OnUnhovered;
	
	void Initialize(const FItmItemInstance& InItem);

	void RefreshTooltip(const FItmItemInstance& InItem);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabelWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UItmItemTooltipWidget> ItemTooltipWidget = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItmItemTooltipWidget> ItemTooltipWidgetClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	FItmItemInstance Item;
	
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
