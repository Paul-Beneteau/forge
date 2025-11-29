#include "Forge/Item/UI/ItmClickCatcherWidget.h"

#include "Forge/Item/Components/ItmInventoryComponent.h"

FReply UItmClickCatcherWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);
	APawn* Pawn = PlayerController->GetPawn();
	check(Pawn);

	UItmInventoryComponent* InventoryComp = Pawn->FindComponentByClass<UItmInventoryComponent>();
	if (InventoryComp && InventoryComp->HasHeldItem())
	{
		// If an item is held, the item will be placed in the world. FReply::Handled() allow to consume the input click
		// avoiding player to make another action at the same time
		// TODO: Find a way to handle this logic outside the widget
		OnClicked.Broadcast();
		return FReply::Handled();
	}

	// Do not consume player input
	return FReply::Unhandled();
}
