#include "ComMeleeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"
#include "Forge/AI/AaiAiConfig.h"
#include "Forge/AI/AaiNonPlayerController.h"
#include "Forge/Core/Character/ComNonPlayerCharacter.h"
#include "GameFramework/Character.h"

void UComMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	ACharacter* Character = CastChecked<ACharacter>(GetOwningActorFromActorInfo());		
	AAaiNonPlayerController* Controller = CastChecked<AAaiNonPlayerController>(Character->GetController());
	UAaiAiConfig* AiConfig = Controller->AiConfig;
	check(AiConfig && AiConfig->IsValid());

	FVector AttackLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * AiConfig->AttackRange;
	
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	// Sphere trace in front to find targets and apply attack effect
	if (Character->GetWorld()->OverlapMultiByChannel(Overlaps, AttackLocation, FQuat::Identity,ECC_Pawn,
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

			UAbilitySystemComponent* InstigatorAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
			if (!InstigatorAbilitySystemComp)
				return;

			AComNonPlayerCharacter* AiCharacter = Cast<AComNonPlayerCharacter>(Character);
			if (!AiCharacter)
				return;
			
			FGameplayEffectContextHandle Context = InstigatorAbilitySystemComp->MakeEffectContext();
			Context.SetAbility(this);
			
			InstigatorAbilitySystemComp->ApplyGameplayEffectToTarget(AiConfig->AttackEffect->GetDefaultObject<UGameplayEffect>(),
				TargetAbilitySystemComp, 1.0f, Context);
		}
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
