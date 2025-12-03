#include "AaiIsInAttackRangeService.h"

#include "AIController.h"
#include "AaiAiConfig.h"
#include "AaiNonPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UAaiIsInAttackRangeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAaiNonPlayerController* Controller = CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner());
	check(Controller->AiConfig && Controller->AiConfig->IsValid());
	
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Controller->AiConfig->TargetActorKeyName));
	if (!Target)
		return;

	const double DistanceToTarget = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	bool bInRange = DistanceToTarget <= Controller->AiConfig->AttackRange;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(Controller->AiConfig->IsInAttackRangeKeyName, bInRange);
}