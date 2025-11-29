#include "ComWorldUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"

// Gets The widget screen position from his attached actor world location.
void UComWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (AttachedActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UComWorldUserWidget: Attached Actor is null"));
		return;
	}

	FVector2D ScreenPos;
	const bool bIsOnScreen { UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),
		AttachedActor->GetActorLocation() + OffSet, ScreenPos) };
	
	if (bIsOnScreen)
	{
		// Change the screen position relative to the screen resolution
		const float Scale =  UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPos /= Scale;
		
		if (!ParentSizeBox)
		{
			UE_LOG(LogTemp, Warning, TEXT("UComWorldUserWidget: ParentSizeBox is null"));
			return;
		}
		else
		{
			ParentSizeBox->SetRenderTranslation(ScreenPos);
		}
		
		ParentSizeBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// Hide Widget that still appears in the corner of the screen when it is behind the player in the world and
		// should not be visible.
		ParentSizeBox->SetVisibility(ESlateVisibility::Hidden);
	}
}