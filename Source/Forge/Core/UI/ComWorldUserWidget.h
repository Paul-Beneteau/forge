#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class FORGE_API UComWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	TObjectPtr<AActor> AttachedActor;
		
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))		
	TObjectPtr<USizeBox> ParentSizeBox;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FVector OffSet;
	
	// Render the widget to the screen from his attached actor world location.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
