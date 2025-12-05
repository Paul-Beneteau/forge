#include "AaiIsLocationBlockedService.h"

#include "AaiAiConfig.h"
#include "AaiNonPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"

// Do a sphere sweep to check if the location is blocked and update the associated blackboard key
void UAaiIsLocationBlockedService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAaiNonPlayerController* Controller = CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner());
	check(Controller->AiConfig && Controller->AiConfig->IsValid());

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Controller->AiConfig->TargetActorKeyName));
	if (!Target)
		return;
		
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Pawn);
	Params.AddIgnoredActor(Target);
	Params.bTraceComplex = false;
	
	TArray<FOverlapResult> Overlaps;
	
	FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Controller->AiConfig->MoveToLocationKeyName);

	// Sphere sweep to check if the location is blocked
	GetWorld()->OverlapMultiByChannel(Overlaps, MoveToLocation,FQuat::Identity,ECC_Pawn,	FCollisionShape::MakeSphere(25.f), Params);

	bool bIsBlocked = false;
	
	// If there is a pawn inside the sweep, the location is blocked
	for (const FOverlapResult& Hit : Overlaps)
	{
		if (Hit.GetActor() && Hit.GetActor()->IsA(APawn::StaticClass()))
		{
			bIsBlocked = true;
			break;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(Controller->AiConfig->IsMoveToLocationBlockedKeyName, bIsBlocked);
}
