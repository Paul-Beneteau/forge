#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItmClickCatcherWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedEvent);

// Widget that catches player click. Used to know when a player clicks outside inventory to remove an item
UCLASS()
class FORGE_API UItmClickCatcherWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnClickedEvent OnClicked;
	
protected:	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
