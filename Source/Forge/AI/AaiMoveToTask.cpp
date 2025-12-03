#include "AaiMoveToTask.h"

#include "AaiAiConfig.h"
#include "AaiNonPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UAaiMoveToTask::UAaiMoveToTask()
{
	// Create task instance for each NonPlayerCharacter so that class member like CachedMoveRequest are not shared with every NonPlayerCharacter. 
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UAaiMoveToTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedBehaviorTree = &OwnerComp;	
	CachedController = CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner());	
	check(CachedController->AiConfig && CachedController->AiConfig->IsValid() && OwnerComp.GetBlackboardComponent());
	
	FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(CachedController->AiConfig->MoveToLocationKeyName);
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(MoveToLocation);	
	MoveRequest.SetAcceptanceRadius(10.f);
	MoveRequest.SetReachTestIncludesAgentRadius(true);
	MoveRequest.SetReachTestIncludesGoalRadius(true);	
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);
	MoveRequest.SetCanStrafe(true);
	
	UPathFollowingComponent* PathComp = CachedController->GetPathFollowingComponent();
	if (!PathComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UAaiMoveToTask: AI Character doesn't have path following component "));
		return EBTNodeResult::Failed;
	}

	// Remove callback binding if move to is called before the previous move to task has finished
	if (OnMoveCompletedHandle.IsValid())
	{
		PathComp->OnRequestFinished.Remove(OnMoveCompletedHandle);
		OnMoveCompletedHandle.Reset();
	}

	FPathFollowingRequestResult MoveToResult = CachedController->MoveTo(MoveRequest);	
	if (MoveToResult.Code == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Error, TEXT("UAaiMoveToTask: fail to move- no valid path"));
		return EBTNodeResult::Failed;
	}

	CachedMoveRequest = MoveToResult.MoveId;
	
	// Bind Callback to end task when move is completed
	OnMoveCompletedHandle = PathComp->OnRequestFinished.AddUObject(this, &UAaiMoveToTask::OnMoveCompleted);

	return EBTNodeResult::InProgress;
}

void UAaiMoveToTask::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	// If the requested move that ended is not the current requested move, don't end the task as the requested move path has been updated
	if (RequestID != CachedMoveRequest)
		return;
	
	if (UPathFollowingComponent* PathComp = CachedController->GetPathFollowingComponent())
	{
		// Remove Callback bind to OnRequestFinished
		PathComp->OnRequestFinished.Remove(OnMoveCompletedHandle);
		OnMoveCompletedHandle.Reset();
	}

	// End the task
	FinishLatentTask(*CachedBehaviorTree, Result.IsSuccess() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}
