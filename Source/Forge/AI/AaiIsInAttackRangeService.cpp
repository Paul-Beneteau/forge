#include "AaiIsInAttackRangeService.h"

#include "AIController.h"
#include "AaiAiConfigData.h"
#include "AaiNonPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Update blackboard key indicating if owner character is in attack range of target
void UAaiIsInAttackRangeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAaiNonPlayerController* Controller = CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner());
	check(Controller->AiConfigData);
	
	APawn* Pawn { OwnerComp.GetAIOwner()->GetPawn() };
	check(Pawn);
	
	AActor* Target { Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Controller->AiConfigData->TargetActorKeyName)) };

	// Target can be null when behavior tree just starts
	if (!Target)
	{
		return;
	}

	const double DistanceToTarget { FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation()) };
	
	bool bInRange { DistanceToTarget <= Controller->AiConfigData->AttackRange};
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(Controller->AiConfigData->IsInAttackRangeKeyName, bInRange);
}