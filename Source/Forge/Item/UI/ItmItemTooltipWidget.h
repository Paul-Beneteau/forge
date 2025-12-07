#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItmItemTooltipWidget.generated.h"

struct FItmInventoryEntry;
struct FItmItemInstance;
class UTextBlock;

// Item tooltip widget when hoovered
UCLASS()
class FORGE_API UItmItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Refresh(const FItmItemInstance& NewItem);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabelWidget;
	// List of item attributes
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AttributesLabelWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> InformationLabelWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Name Color")
	FSlateColor CraftItemColor;
	UPROPERTY(EditDefaultsOnly, Category = "Name Color")
	FSlateColor MagicItemColor;
	UPROPERTY(EditDefaultsOnly, Category = "Name Color")
	FSlateColor RareItemColor;
	
	virtual void NativeConstruct() override;

	void SetAttributesLabel(const FItmItemInstance& Item);
	
	void SetInformationLabel(const FItmItemInstance& Item);
};