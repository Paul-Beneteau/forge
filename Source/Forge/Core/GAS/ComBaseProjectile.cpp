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

	//MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AComBaseProjectile::OnActorOverlap);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AComBaseProjectile::OnActorOverlap);
}

void AComBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HitWorldParticleEffect)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldParticleEffect, GetActorLocation(), GetActorRotation());
	else
		UE_LOG(LogTemp, Warning, TEXT("AComBaseProjectile: HitWorldEffect has not been set"));

	SetLifeSpan(LifeSpawn);
}

// Apply the gameplay effect to the actor hit
void AComBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	UE_LOG(LogTemp, Warning, TEXT("AComBaseProjectile::OnActorOverlap"));
	
	if (OtherActor == GetInstigator() || OtherActor == this)
		return;

	if (HitActorParticleEffect)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitActorParticleEffect, GetActorLocation(), GetActorRotation());
	else
		UE_LOG(LogTemp, Warning, TEXT("AComBaseProjectile: projectile HitActorEffect is null"));

	AActor* InstigatorActor = Cast<ACharacter>(GetInstigator());
	AActor* TargetActor = Cast<ACharacter>(OtherActor);
	
	if (!InstigatorActor || !TargetActor)
		return;

	UAbilitySystemComponent* TargetAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	UAbilitySystemComponent* InstigatorAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);
	if (!TargetAbilitySystemComp || !InstigatorAbilitySystemComp)
		return;
	
	FGameplayEffectContextHandle EffectHandle = InstigatorAbilitySystemComp->MakeEffectContext();
	EffectHandle.SetAbility(InstigatorAbility);

	if (HitActorGameplayEffect)
		InstigatorAbilitySystemComp->ApplyGameplayEffectToTarget(HitActorGameplayEffect->GetDefaultObject<UGameplayEffect>(),
		TargetAbilitySystemComp, 1.0f, EffectHandle);
	else
		UE_LOG(LogTemp, Error, TEXT("AComBaseProjectile: HitActorGameplayEffect has not been set"));

	
	Destroy();
}
