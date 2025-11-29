// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AaiNonPlayerController.generated.h"

class UAaiAiConfigData;
class UAISenseConfig_Sight;
struct FAIStimulus;

UCLASS()
class FORGE_API AAaiNonPlayerController : public AAIController
{
	GENERATED_BODY()

public:
	AAaiNonPlayerController(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	const TObjectPtr<UAaiAiConfigData> AiConfigData;
	
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

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
