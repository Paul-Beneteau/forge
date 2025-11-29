#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItmHeldItemWidget.generated.h"

struct FItmInventoryEntry;
class UImage;

// Widget displaying current held item 
UCLASS()
class FORGE_API UItmHeldItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Refresh(const FItmInventoryEntry& NewItem);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
	
	// TODO: Parametrize size
	FVector2D ItemImageSize { FVector2D(50.f, 50.f) };
	
	virtual void NativeConstruct() override;
};
