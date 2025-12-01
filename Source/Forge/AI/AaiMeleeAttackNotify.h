#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AaiMeleeAttackNotify.generated.h"

class UGameplayEffect;

UCLASS()
class FORGE_API UAaiMeleeAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
