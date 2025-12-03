#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AaiMoveToTask.generated.h"

class AAaiNonPlayerController;
struct FPathFollowingResult;

// Task to move to the location given by the blackboard key "AiConfig->MoveToLocationKeyName"
UCLASS()
class FORGE_API UAaiMoveToTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAaiMoveToTask();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CachedBehaviorTree;	
	UPROPERTY()
	TObjectPtr<AAaiNonPlayerController> CachedController;
	FAIRequestID CachedMoveRequest;
	
	FDelegateHandle OnMoveCompletedHandle;

	// Callback when move to task is completed.
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) ;	
};
