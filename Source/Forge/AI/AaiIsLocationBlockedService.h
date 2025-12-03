#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AaiIsLocationBlockedService.generated.h"

// Update blackboard key indicating if the location from "AiConfig->MoveToLocationKeyName" is blocked
UCLASS()
class FORGE_API UAaiIsLocationBlockedService : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
