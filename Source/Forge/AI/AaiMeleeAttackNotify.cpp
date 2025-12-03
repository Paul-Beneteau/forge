#include "AaiMeleeAttackNotify.h"

#include "AaiAiConfig.h"
#include "AaiNonPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"

void UAaiMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
		return;

	ACharacter* AiCharacter = CastChecked<ACharacter>(MeshComp->GetOwner());		
	AAaiNonPlayerController* Controller = CastChecked<AAaiNonPlayerController>(AiCharacter->GetController());
	UAaiAiConfig* AiConfig = Controller->AiConfig;
	check(AiConfig && AiConfig->IsValid());

	FVector AttackLocation = AiCharacter->GetActorLocation() + AiCharacter->GetActorForwardVector() * AiConfig->AttackRange;
	
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AiCharacter);

	// Sphere trace in front to find targets and apply attack effect
	if (AiCharacter->GetWorld()->OverlapMultiByChannel(Overlaps, AttackLocation, FQuat::Identity,ECC_Pawn,
		FCollisionShape::MakeSphere(AiConfig->AttackRadius), QueryParams))
	{		
		for (const FOverlapResult& Overlap : Overlaps)
		{			
			APawn* HitPawn = Cast<APawn>(Overlap.GetActor());			
			if (!HitPawn || !HitPawn->IsPlayerControlled())
				continue;

			UAbilitySystemComponent* TargetAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitPawn);
			if (!TargetAbilitySystemComp)
				return;

			FGameplayEffectContextHandle Context = TargetAbilitySystemComp->MakeEffectContext();
			TargetAbilitySystemComp->ApplyGameplayEffectToSelf(AiConfig->AttackEffect.GetDefaultObject(), 1.0f, Context);
		}
	}
}