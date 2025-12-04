#include "Forge/Item/Generation/ItmItemGeneratorSubsystem.h"
#include "ItmWorldItem.h"
#include "Forge/MapGenerator/MapGraphUtils.h"
#include "Kismet/GameplayStatics.h"

void UItmItemGeneratorSubsystem::TrySpawnItem(const FVector& Location)
{
	if (FMath::FRand() > ItemGeneratorConfig->ChangeToSpawnItem)
		return;

	SpawnWorldItem(GenerateRandomItem(), Location);
}

void UItmItemGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TSoftObjectPtr<UItmItemGeneratorConfig> SoftPtr(FSoftObjectPath(TEXT("/Game/Item/Config/DA_ItemGeneratorConfig.DA_ItemGeneratorConfig")));
	ItemGeneratorConfig = SoftPtr.LoadSynchronous();
	
	check(ItemGeneratorConfig && ItemGeneratorConfig->IsValid());
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

	constexpr int32 MaxAttributesCount = 6;
	// Generate the number of attributes for the item
	const int32 RandomAttributesCount = GenerateAttributeCount();

	TArray<FItmItemAttributeTemplate> RemainingTemplates = RandomItem.ItemBase.AttributeTemplates;
    
	for (int32 AttributeIndex = 0; AttributeIndex < RandomAttributesCount && !RemainingTemplates.IsEmpty(); ++AttributeIndex)
	{
		FItmItemAttributeTemplate* SelectedTemplate = PickWeightedRandomTemplate(RemainingTemplates);
		if (!SelectedTemplate)
			break;

		// Generate attribute with a rolled value
		RandomItem.Attributes.Add(SelectedTemplate->GenerateRolledAttribute());

		// Remove the used template to avoid duplicates
		RemainingTemplates.RemoveAll([SelectedTemplate](const FItmItemAttributeTemplate& Template)
		{
			return Template.Attribute == SelectedTemplate->Attribute;
		});
	}
    
	return RandomItem;
}

int32 UItmItemGeneratorSubsystem::GenerateAttributeCount() const
{
	TArray<float> Weights = { 10, 8, 5, 3, 2, 1 };

	float TotalWeight = 0.0f;
	for (float Weight : Weights)
		TotalWeight += Weight;

	const float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (int32 i = 0; i < Weights.Num(); ++i)
	{
		CurrentWeight += Weights[i];
		if (RandomWeight <= CurrentWeight)
			return i + 1;
	}

	return 1;
}

FItmItemBase* UItmItemGeneratorSubsystem::GenerateRandomItemBase() const
{
	TArray<FName> ItemBaseRows = ItemGeneratorConfig->ItemBasePool->GetRowNames();
	if (ItemBaseRows.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UItmItemGeneratorSubsystem: ItemBaseDataTable has no rows"));
		return nullptr;
	}

	TArray<FItmItemBase*> ItemBases;
	for (const FName& RowName : ItemBaseRows)
	{
		if (FItmItemBase* ItemBase = ItemGeneratorConfig->ItemBasePool->FindRow<FItmItemBase>(RowName, TEXT("ItemBase")))
			ItemBases.Add(ItemBase);
	}

	if (ItemBases.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UItmItemGeneratorSubsystem: No valid item bases found"));
		return nullptr;
	}

	float TotalWeight = 0.0f;
	for (const FItmItemBase* ItemBase : ItemBases)
		TotalWeight += ItemBase->Weight;

	if (TotalWeight <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UItmItemGeneratorSubsystem: Total weight is 0, picking random item"));
		return ItemBases[FMath::RandRange(0, ItemBases.Num() - 1)];
	}

	const float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (FItmItemBase* ItemBase : ItemBases)
	{
		CurrentWeight += ItemBase->Weight;
		if (RandomWeight <= CurrentWeight)
			return ItemBase;
	}

	return ItemBases.Last();
}

FItmItemAttributeTemplate* UItmItemGeneratorSubsystem::PickWeightedRandomTemplate(TArray<FItmItemAttributeTemplate>& Templates) const
{
	if (Templates.IsEmpty())
		return nullptr;

	float TotalWeight = 0.0f;
	for (const FItmItemAttributeTemplate& Template : Templates)
		TotalWeight += Template.Weight;

	if (TotalWeight <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UItmItemGeneratorSubsystem: Total weight is 0, picking random template"));
		return &Templates[FMath::RandRange(0, Templates.Num() - 1)];
	}

	const float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (FItmItemAttributeTemplate& Template : Templates)
	{
		CurrentWeight += Template.Weight;
		if (RandomWeight <= CurrentWeight)
			return &Template;
	}

	return &Templates.Last();
}

AItmWorldItem* UItmItemGeneratorSubsystem::SpawnWorldItem(const FItmItemInstance& Item, const FVector& Location) const
{	
	AItmWorldItem* WorldItem = GetWorld()->SpawnActorDeferred<AItmWorldItem>(AItmWorldItem::StaticClass(),
		FTransform(FRotator::ZeroRotator, Location));
	
	if (WorldItem)
	{
		WorldItem->Initialize(Item, ItemGeneratorConfig->WorldItemWidgetClass);
		UGameplayStatics::FinishSpawningActor(WorldItem, FTransform(FRotator::ZeroRotator, Location));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UItmItemGeneratorSubsystem: Failed to spawn world item"));
	}
	
	return WorldItem;
}