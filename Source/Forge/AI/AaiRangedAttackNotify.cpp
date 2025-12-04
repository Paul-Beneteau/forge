#include "AaiRangedAttackNotify.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAaiRangedAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if (!Actor)
		return;
		
	UAbilitySystemComponent* AbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!AbilitySystemComp)
		return;

	FGameplayAbilitySpec AbilitySpec(ProjectileAbilityClass, 1, INDEX_NONE, Actor);
	FGameplayAbilitySpecHandle Handle = AbilitySystemComp->GiveAbility(AbilitySpec);
	
	if (Handle.IsValid())
		AbilitySystemComp->TryActivateAbility(Handle);
}
