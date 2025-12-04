#include "AaiAttackTask.h"

#include "AaiAiConfig.h"
#include "AaiNonPlayerController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UAaiAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAaiNonPlayerController* Controller = CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner());

	Controller->StopMovement();

	CachedCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (!CachedCharacter)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(Controller->AiConfig->TargetActorKeyName));
	if (!Target)
		return EBTNodeResult::Failed;

	TargetRotation = (Target->GetActorLocation() - CachedCharacter->GetActorLocation()).Rotation();
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;
    
	CachedCharacter->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAaiAttackTask::OnCharacterRotating, 0.01f, true);
	
	return EBTNodeResult::Succeeded;
}

void UAaiAttackTask::OnCharacterRotating()
{
	if (!CachedCharacter)
		return;

	FRotator CurrentRotation = CachedCharacter->GetActorRotation();
    
	// Interpolate to target rotation
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, 0.01f, 10.0f);
	CachedCharacter->SetActorRotation(NewRotation);
    
	// If we're within 2 degrees of the rotation target
	float YawDifference = FMath::Abs(NewRotation.Yaw - TargetRotation.Yaw);
	if (YawDifference < 2.0f || YawDifference > 358.0f)
	{
		CachedCharacter->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		if (UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance())
		{
			AController* Control = CachedCharacter->GetController();
	
			AAaiNonPlayerController* Controller = Cast<AAaiNonPlayerController>(Control);
			if (!Controller)
				return;
			
			if (Controller->AiConfig && !Controller->AiConfig->AttackMontage)
			{
				UE_LOG(LogTemp, Warning, TEXT("UAaiAttackTask: AttackMontage has not been set"));
				return;
			}
		
			AnimInstance->Montage_Play(Controller->AiConfig->AttackMontage);
		}
	}
}
