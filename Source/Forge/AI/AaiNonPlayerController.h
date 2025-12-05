#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayAbilitySpecHandle.h"
#include "AaiNonPlayerController.generated.h"

class UAaiAiConfig;
class UAISenseConfig_Sight;
struct FAIStimulus;

UCLASS()
class FORGE_API AAaiNonPlayerController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	const TObjectPtr<UAaiAiConfig> AiConfig;
	
	AAaiNonPlayerController(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	FGameplayAbilitySpecHandle GetAbilityHandle() { return AbilityHandle; };
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBlackboardComponent* BlackboardComp;
	
	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> Sight;

	// GAS Ability handle for AiConfig->AttackClass ability given
	FGameplayAbilitySpecHandle AbilityHandle;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
