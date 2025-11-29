#include "Forge/Item/Generation/ItmItemGeneratorSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "ItmWorldItem.h"
#include "Kismet/GameplayStatics.h"

void UItmItemGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// TODO: store asset path in an data asset
	TSoftClassPtr<UUserWidget> SoftWidgetClass { FSoftObjectPath(TEXT("/Game/Item/UI/WBP_WorldItem.WBP_WorldItem_C")) };
	WorldItemWidgetClass = SoftWidgetClass.LoadSynchronous();
	check(WorldItemWidgetClass);

	TSoftObjectPtr<UDataTable> ItemBasePoolAsset{ FSoftObjectPath(TEXT("/Game/Item/Config/DT_ItemBasePool.DT_ItemBasePool")) };
	ItemBasePool = ItemBasePoolAsset.LoadSynchronous();
	check(ItemBasePool);
}

FItmItemInstance UItmItemGeneratorSubsystem::GenerateRandomItem() const
{
	FItmItemBase* ItemBase = GenerateRandomItemBase();
	if (!ItemBase)
	{
		UE_LOG(LogTemp, Error, TEXT("UItmItemGeneratorSubsystem: Failed to generate a random item base"));
		return FItmItemInstance::Empty;
	}
	
	FItmItemInstance RandomItem;
	RandomItem.ItemBase = *ItemBase;

	constexpr int32 MaxAttributesCount { 6 };
	// Generate the number of attributes for the item
	const int32 RandomAttributesCount { FMath::RandRange(1, MaxAttributesCount) };

	TArray<FItmItemAttributeTemplate> AttributeTemplates = RandomItem.ItemBase.AttributeTemplates;
	
	for (int32 AttributeIndex = 0; AttributeIndex < RandomAttributesCount && AttributeTemplates.Num() > 0; ++AttributeIndex)
	{
		const int32 RandomTemplateIndex { FMath::RandRange(0, AttributeTemplates.Num() - 1) };		

		// Add a random attribute to the item generated
		RandomItem.Attributes.Add(AttributeTemplates[RandomTemplateIndex].GenerateRandomAttribute());

		// Remove the attribute template used to not have the same attribute twice in the generated attributes
		AttributeTemplates.RemoveAt(RandomTemplateIndex);
	}
	
	return RandomItem;
}

FItmItemBase* UItmItemGeneratorSubsystem::GenerateRandomItemBase() const
{
	TArray<FName> ItemBaseRows = ItemBasePool->GetRowNames();
	if (ItemBaseRows.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UItmItemGeneratorSubsystem: ItemBaseDataTable has no rows"));
		return nullptr;
	}

	const FName ItemBaseRow = ItemBaseRows[FMath::RandRange(0, ItemBaseRows.Num() - 1)];
	
	return ItemBasePool->FindRow<FItmItemBase>(ItemBaseRow, TEXT("ItemBase"));;
}

AItmWorldItem* UItmItemGeneratorSubsystem::SpawnWorldItem(const FItmItemInstance& Item, const FVector& Location) const
{	
	AItmWorldItem* WorldItem = GetWorld()->SpawnActorDeferred<AItmWorldItem>(AItmWorldItem::StaticClass(),
		FTransform(FRotator::ZeroRotator, Location));
	
	if (WorldItem)
	{
		WorldItem->Initialize(Item, WorldItemWidgetClass);
		UGameplayStatics::FinishSpawningActor(WorldItem, FTransform(FRotator::ZeroRotator, Location));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UItmItemGeneratorSubsystem: Failed to spawn world item"));
	}
	
	return WorldItem;
}