#include "AaiAttackNotify.h"

#include "AaiNonPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Forge/Core/Character/ComNonPlayerCharacter.h"

void UAaiAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AComNonPlayerCharacter* AiCharacter = Cast<AComNonPlayerCharacter>(MeshComp->GetOwner());
	if (!AiCharacter)
		return;
	AAaiNonPlayerController* Controller = Cast<AAaiNonPlayerController>(AiCharacter->GetController());
	if (!Controller)
		return;		
	UAbilitySystemComponent* AbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AiCharacter);
	if (!AbilitySystemComp)
		return;

	if (Controller->GetAbilityHandle().IsValid())
		AbilitySystemComp->TryActivateAbility(Controller->GetAbilityHandle());
}
