#pragma once

#include "CoreMinimal.h"
#include "ItmItemBase.h"
#include "ItmItemInstance.generated.h"

// Runtime instance of an item created from item data FItmItemBase
USTRUCT(BlueprintType)
struct FItmItemInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGuid Id = FGuid::NewGuid();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItmItemBase ItemBase;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItmItemAttribute> Attributes;
	
	// Empty entry used for initialization
	static const FItmItemInstance Empty;
	
	bool IsValid() const { return Id.IsValid() && ItemBase.IsValid(); }
};

// Represent an inventory item. 
USTRUCT(BlueprintType)
struct FItmInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItmItemInstance Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity = 1;

	// Empty entry used for initialization
	static const FItmInventoryEntry Empty;
	
	bool IsValid() const { return Item.IsValid() && Quantity >= 0; }
};