#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "Engine/DataAsset.h"
#include "ItmItemBase.generated.h"

// Item type used in item data to create an item base
UENUM(BlueprintType)
enum class EItmItemType : uint8
{
	None			UMETA(DisplayName = "None"),
	Helmet			UMETA(DisplayName = "Helmet"),
	Chest			UMETA(DisplayName = "Chest"),
	Boots			UMETA(DisplayName = "Boots"),
	Gloves			UMETA(DisplayName = "Gloves"),
	Ring			UMETA(DisplayName = "Ring"),
	Amulet			UMETA(DisplayName = "Amulet"),
	Belt			UMETA(DisplayName = "Belt"),
	Weapon			UMETA(DisplayName = "Weapon"),
	Currency		UMETA(DisplayName = "Currency"),
};

// Enum used to differentiate each equipment widget.  Can be associated with a widget in a TMap.
UENUM(BlueprintType)
enum class EItmEquipmentSlot : uint8
{
	None            UMETA(DisplayName = "None"),
	Helmet          UMETA(DisplayName = "Helmet"),
	Chest           UMETA(DisplayName = "Chest"),
	Boots           UMETA(DisplayName = "Boots"),
	Gloves          UMETA(DisplayName = "Gloves"),
	PrimaryWeapon   UMETA(DisplayName = "Primary Weapon"),
	SecondaryWeapon UMETA(DisplayName = "Secondary Weapon"),
	PrimaryRing     UMETA(DisplayName = "Primary Ring"),
	SecondaryRing   UMETA(DisplayName = "Secondary Ring"),
	Amulet          UMETA(DisplayName = "Amulet"),
	Belt            UMETA(DisplayName = "Belt")
};

// Attributes of an instanced item that have been rolled.
USTRUCT(BlueprintType)
struct FItmItemAttribute
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Magnitude = 0.f;
};

// Attributes templates used to generate attributes of an instanced item
USTRUCT(BlueprintType)
struct FItmItemAttributeTemplate
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinMagnitude = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMagnitude = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.f;
	
	// Generate an attribute from the template with a random magnitude between MinMagnitude and MaxMagnitude
	FItmItemAttribute GenerateRolledAttribute()	{ return FItmItemAttribute(Attribute, ModifierOp, FMath::RandRange(MinMagnitude, MaxMagnitude)); }
};

// Contains the data to generate an item (name, type, attributes, etc.) 
USTRUCT(BlueprintType)
struct FItmItemBase : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = "NAME_None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItmItemType ItemType = EItmItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItmItemAttributeTemplate> AttributeTemplates;

	// Chance of base to be picked 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 0.f;
	
	bool IsValid() const { return Name != NAME_None && ItemType != EItmItemType::None; }
};