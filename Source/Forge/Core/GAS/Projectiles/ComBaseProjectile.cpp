#include "ComBaseProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Forge/Core/Character/ComPlayerCharacter.h"
#include "Forge/Core/Character/ComNonPlayerCharacter.h"

AComBaseProjectile::AComBaseProjectile()
{
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>("CollisionComp");
	RootComponent = CollisionComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1900.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

void AComBaseProjectile::Initialize(TSubclassOf<UGameplayEffect> InHitActorGameplayEffect, TObjectPtr<UGameplayAbility> InInstigatorAbility)
{
	HitActorGameplayEffect = InHitActorGameplayEffect;
	InstigatorAbility = InInstigatorAbility;
}

void AComBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AComBaseProjectile::OnActorOverlap);
}

void AComBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HitWorldParticleEffect)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldParticleEffect, GetActorLocation(), GetActorRotation());

	SetLifeSpan(LifeSpawn);
}

// Apply the gameplay effect to the actor hit
void AComBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (!IsValidTarget(OtherActor))
		return;

	HandleActorHit(OtherActor);
}

bool AComBaseProjectile::IsValidTarget(AActor* Actor) const
{
	if (!Actor || Actor == GetInstigator() || Actor == this)
		return false;

	APawn* OtherPawn = Cast<APawn>(Actor);
	if (!OtherPawn)
		return false;

	// Ignore if this is an AI character attacking another AI character
	if (!OtherPawn->IsPlayerControlled() && GetInstigator() && !GetInstigator()->IsPlayerControlled())
		return false;

	return true;
}

void AComBaseProjectile::HandleActorHit(AActor* HitActor)
{
	if (HitActorParticleEffect)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitActorParticleEffect, GetActorLocation(), GetActorRotation());

	ApplyGameplayEffect(HitActor);

	Destroy();
}

void AComBaseProjectile::ApplyGameplayEffect(AActor* TargetActor)
{
	AActor* InstigatorActor = GetInstigator();
	if (!InstigatorActor || !TargetActor)
		return;

	UAbilitySystemComponent* TargetAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	UAbilitySystemComponent* InstigatorAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);
    
	if (!TargetAbilitySystemComp || !InstigatorAbilitySystemComp)
		return;

	if (!HitActorGameplayEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("AComBaseProjectile: HitActorGameplayEffect has not been set"));
		return;
	}

	FGameplayEffectContextHandle EffectHandle = InstigatorAbilitySystemComp->MakeEffectContext();
	EffectHandle.SetAbility(InstigatorAbility);

	InstigatorAbilitySystemComp->ApplyGameplayEffectToTarget(HitActorGameplayEffect->GetDefaultObject<UGameplayEffect>(), TargetAbilitySystemComp, 
		1.0f, EffectHandle);
}