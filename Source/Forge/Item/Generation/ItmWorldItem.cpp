#include "ItmWorldItem.h"

#include "Blueprint/UserWidget.h"
#include "Forge/Item/Components/ItmInventoryComponent.h"
#include "Forge/Item/UI/ItmWorldItemWidget.h"

AItmWorldItem::AItmWorldItem()
{
	// Create a root component so that the actor can have a location when spawned
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

void AItmWorldItem::Initialize(const FItmItemInstance& InItem, TSubclassOf<UUserWidget> InWidgetClass)
{
	Item = InItem;
	WorldItemWidgetClass = InWidgetClass;
}

void AItmWorldItem::BeginPlay()
{
	Super::BeginPlay();

	check(WorldItemWidgetClass && Item.IsValid());

	WorldItemWidget = CreateWidget<UItmWorldItemWidget>(GetWorld(), WorldItemWidgetClass);
	
	WorldItemWidget->OnClicked.AddDynamic(this, &AItmWorldItem::HandleWorldItemWidgetClicked);
	WorldItemWidget->OnHovered.AddDynamic(this, &AItmWorldItem::HandleItemWidgetHovered);
	WorldItemWidget->OnUnhovered.AddDynamic(this, &AItmWorldItem::HandleItemWidgetUnhovered);
	WorldItemWidget->Initialize(Item);
	
	WorldItemWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	WorldItemWidget->AddToViewport(10);
}

void AItmWorldItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (WorldItemWidget && WorldItemWidget->IsInViewport())
	{
		WorldItemWidget->RemoveFromParent();
		WorldItemWidget = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AItmWorldItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!WorldItemWidget)
		return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
		return;

	FVector2D WidgetTargetScreenPos;

	if (!PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), WidgetTargetScreenPos))
	{
		WorldItemWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// Smooth widget movement
	WidgetCurrentScreenPos = FMath::Vector2DInterpTo(WidgetCurrentScreenPos, WidgetTargetScreenPos, DeltaSeconds, 10.f);
		
	WorldItemWidget->SetPositionInViewport(WidgetCurrentScreenPos, true);
	WorldItemWidget->SetVisibility(ESlateVisibility::Visible);
}

void AItmWorldItem::HandleItemWidgetHovered()
{	
	WorldItemWidget->RefreshTooltip(Item);
}

void AItmWorldItem::HandleItemWidgetUnhovered()
{
	WorldItemWidget->RefreshTooltip(FItmItemInstance::Empty);
}

void AItmWorldItem::HandleWorldItemWidgetClicked()
{	
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	const APawn* PlayerPawn = PlayerController->GetPawn();
	check(PlayerPawn);

	UItmInventoryComponent* UItmInventoryComp = PlayerPawn->FindComponentByClass<UItmInventoryComponent>();
	
	FItmInventoryEntry InventoryEntry;
	InventoryEntry.Item = Item;
	InventoryEntry.Quantity = 1;
	
	if (UItmInventoryComp->AddItemToFirstAvailableGridSlot(InventoryEntry))
	{
		WorldItemWidget->RefreshTooltip(FItmItemInstance::Empty);
		Destroy();
	}
}

