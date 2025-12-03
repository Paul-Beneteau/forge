#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AaiIsInAttackRangeService.generated.h"

// Update blackboard key indicating if AI character is in attack range of target
UCLASS()
class FORGE_API UAaiIsInAttackRangeService : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
