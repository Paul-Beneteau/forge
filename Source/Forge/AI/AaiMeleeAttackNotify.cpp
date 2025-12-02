#include "AaiMeleeAttackNotify.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"

void UAaiMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	FVector AttackLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * AttackRange;
	
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	// Sphere trace in front to find targets
	if (Owner->GetWorld()->OverlapMultiByChannel(Overlaps, AttackLocation, FQuat::Identity,
		ECC_Pawn, FCollisionShape::MakeSphere(AttackRadius), QueryParams))
	{		
		for (const FOverlapResult& Overlap : Overlaps)
		{			
			APawn* HitPawn = Cast<APawn>(Overlap.GetActor());
			
			if (!HitPawn || !HitPawn->IsPlayerControlled())
				continue;

			UAbilitySystemComponent* TargetAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitPawn);
			if (!TargetAbilitySystemComp)
				return;
			
			if (!DamageEffect)
			{
				UE_LOG(LogTemp, Display, TEXT("UAaiMeleeAttackNotify: DamageEffect has not been set"));
				return;
			}
			
			TargetAbilitySystemComp->ApplyGameplayEffectToSelf(DamageEffect.GetDefaultObject(), 1.0f, TargetAbilitySystemComp->MakeEffectContext());
		}
	}
}
