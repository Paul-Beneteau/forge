#include "AaiAttackTask.h"

#include "AaiNonPlayerController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UAaiAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAaiNonPlayerController* Controller { CastChecked<AAaiNonPlayerController>(OwnerComp.GetAIOwner()) };
	check(Controller->AiConfigData);

	Controller->StopMovement();
	
	ACharacter* Character { OwnerComp.GetAIOwner()->GetCharacter() };
	if (!Character)
		return EBTNodeResult::Failed;
	
	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{        
		if (AttackAnim)
			AnimInstance->Montage_Play(AttackAnim);
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UAaiAttackTask: AttackAnim has not been set"));
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}
