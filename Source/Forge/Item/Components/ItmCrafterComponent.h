#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItmCrafterComponent.generated.h"


struct FItmItemAttributeTemplate;
struct FItmItemInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORGE_API UItmCrafterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	bool Craft(FItmItemInstance&  CraftedItem, const FItmItemInstance&  CrafterItem);

	// Pick a random number of attributes using these weight from 1 to 6 attributes: { 10, 8, 5, 3, 2, 1 }
	static int32 GenerateAttributeCount();

	// Pick a random number of attributes and roll them
	static void GenerateItemAttributes(FItmItemInstance& Item);
	
	// Pick a random template using Templates.Weight.
	static FItmItemAttributeTemplate* PickWeightedRandomTemplate(TArray<FItmItemAttributeTemplate>& Templates);
	
protected:
	bool ExaltedOrbCraft(FItmItemInstance&  CraftedItem, const FItmItemInstance&  CrafterItem);

	bool ChaosOrbCraft(FItmItemInstance&  CraftedItem, const FItmItemInstance&  CrafterItem);
	
	bool DivineOrbCraft(FItmItemInstance&  CraftedItem, const FItmItemInstance&  CrafterItem);
};
