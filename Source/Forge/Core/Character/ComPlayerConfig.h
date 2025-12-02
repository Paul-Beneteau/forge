#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Containers/Map.h"
#include "Forge/Item/UI/ItmInventoryRootWidget.h"
#include "ComPlayerConfig.generated.h"

class UInputMappingContext;
class UInputAction;
class UGameplayAbility;

USTRUCT()
struct FComAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TOptional<TObjectPtr<UInputAction>> InputAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

UCLASS()
class FORGE_API UComPlayerConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TArray<FComAbilityInput> InitialAbilities;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Move;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<UItmInventoryRootWidget> InventoryWidgetClass;
	
	FORCEINLINE bool IsValid()
	{
		return DefaultInputContext && Move && DeathMontage && InventoryWidgetClass;
	}
};
