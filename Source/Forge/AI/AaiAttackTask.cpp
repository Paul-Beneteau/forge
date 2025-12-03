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

	ACharacter* Character = OwnerComp.GetAIOwner()->GetCharacter();
	if (!Character)
		return EBTNodeResult::Failed;
	
	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{        
		if (Controller->AiConfig && !Controller->AiConfig->AttackMontage)
		{
			UE_LOG(LogTemp, Warning, TEXT("UAaiAttackTask: AttackMontage has not been set"));
			return EBTNodeResult::Failed;
		}
		
		AnimInstance->Montage_Play(Controller->AiConfig->AttackMontage);
	}

	return EBTNodeResult::Succeeded;
}
