#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBlackboardComponent* BlackboardComp;
	
	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> Sight;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
