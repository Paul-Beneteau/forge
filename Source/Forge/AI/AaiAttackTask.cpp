#include "AaiAttackTask.h"

#include "AaiNonPlayerController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UAaiAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAaiNonPlayerController* Controller { CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner()) };
	check(Controller->AiConfigData);

	Controller->StopMovement();
	
	APawn* Pawn { OwnerComp.GetAIOwner()->GetPawn() };

	// Attack placeholder
	DrawDebugString(GetWorld(),Pawn->GetActorLocation() + FVector(0,0,120),"Attack", nullptr, FColor::Red, 1.0f);

	return EBTNodeResult::Succeeded;
}