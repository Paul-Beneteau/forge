#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AaiRangedAttackNotify.generated.h"

class UGameplayAbility;
class UComProjectileAbility;
class UGameplayEffect;
class AComBaseProjectile;
// Handle AI character ranged attack logic
UCLASS()
class FORGE_API UAaiRangedAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayAbility> ProjectileAbilityClass;
};
