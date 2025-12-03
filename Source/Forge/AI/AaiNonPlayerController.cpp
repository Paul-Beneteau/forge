#include "AaiNonPlayerController.h"

#include "AaiAiConfig.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAaiNonPlayerController::AAaiNonPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	// Set the vision circular
	Sight->PeripheralVisionAngleDegrees = 180.f;
	
	PerceptionComp->ConfigureSense(*Sight);
	PerceptionComp->SetDominantSense(Sight->GetSenseImplementation());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAaiNonPlayerController::OnTargetPerceptionUpdated);
}

void AAaiNonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Set AI Config Data
	check(AiConfig && AiConfig->IsValid());
	
	Sight->SightRadius = AiConfig->SightRadius;
	Sight->LoseSightRadius = AiConfig->LoseSightRadius;

	PerceptionComp->RequestStimuliListenerUpdate();

	// Set Actor as Friendly for perception
	SetGenericTeamId(FGenericTeamId(1));
	
	if (UseBlackboard(AiConfig->BehaviorTree->BlackboardAsset, BlackboardComp))
	{
		RunBehaviorTree(AiConfig->BehaviorTree);
		
		BlackboardComp->SetValueAsFloat(AiConfig->AttackRangeKeyName, AiConfig->AttackRange);

		// Generate donuts for the range query move to location with a circle radius little less than the attack range.
		// Otherwise, NonPlayerCharacter would stop right before the location (because of move to acceptance radius) and
		// not be in range to attack target.
		check(AiConfig->AttackRange - 50.f > 0.f);
		BlackboardComp->SetValueAsFloat(AiConfig->RangeQueryCircleRadiusKeyName, AiConfig->AttackRange - 50.f);
		
		// Set character as melee character if his range his below 200. It will make behavior tree move to target melee range
		BlackboardComp->SetValueAsBool(AiConfig->IsMeleeCharacterKeyName, AiConfig->AttackRange <= 200.f);
		
	}
}

void AAaiNonPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ACharacter* PossessedCharacter = CastChecked<ACharacter>(InPawn))
	{
		if (UCharacterMovementComponent* MovementComp = PossessedCharacter->GetCharacterMovement())
		{			
			// Fix Jittering
			MovementComp->bOrientRotationToMovement = false;
			MovementComp->bUseControllerDesiredRotation = true;
			MovementComp->RotationRate = FRotator(0.f, 720.f, 0.f);

			MovementComp->BrakingDecelerationWalking = 0.f;
			MovementComp->BrakingFrictionFactor = 0.f;
		}

		if (UCrowdFollowingComponent* CrowdFollowingComp = FindComponentByClass<UCrowdFollowingComponent>())
		{
			CrowdFollowingComp->SetCrowdSeparation(true);
			CrowdFollowingComp->SetCrowdSeparationWeight(80.0f);
			CrowdFollowingComp->SetCrowdAvoidanceRangeMultiplier(1.2f);
			CrowdFollowingComp->SetCrowdAnticipateTurns(true);
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
		}
	}

	if (UseBlackboard(AiConfig->BehaviorTree->BlackboardAsset, BlackboardComp))
		BlackboardComp->SetValueAsVector(AiConfig->SpawnLocationKeyName, GetPawn()->GetActorLocation());
}

void AAaiNonPlayerController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !BlackboardComp)
		return;

	// Set target if target is in sight range
	if (Stimulus.WasSuccessfullySensed())
	{		
		BlackboardComp->SetValueAsObject(AiConfig->TargetActorKeyName, Actor);
	}
	else
	{
		BlackboardComp->ClearValue(AiConfig->TargetActorKeyName);
		StopMovement();
	}
}

ETeamAttitude::Type AAaiNonPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	
	if (!OtherPawn)
		return ETeamAttitude::Neutral;

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	
	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;

	if (OtherTeamAgent->GetGenericTeamId() == GetGenericTeamId())
		return ETeamAttitude::Friendly;

	return ETeamAttitude::Hostile;
}
