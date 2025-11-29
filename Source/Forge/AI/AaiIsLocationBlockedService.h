#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AaiIsLocationBlockedService.generated.h"

// Check if the move to location is blocked
UCLASS()
class FORGE_API UAaiIsLocationBlockedService : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
