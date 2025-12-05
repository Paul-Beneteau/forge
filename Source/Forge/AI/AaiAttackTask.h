#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AaiAttackTask.generated.h"

// Attack task for behavior tree.
UCLASS()
class FORGE_API UAaiAttackTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY()
	TObjectPtr<ACharacter> CachedCharacter;
	
	FTimerHandle TimerHandle;

	FRotator TargetRotation;
    
	void OnCharacterRotating();
};
