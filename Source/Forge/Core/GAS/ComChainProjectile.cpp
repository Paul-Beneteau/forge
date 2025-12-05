#include "ComChainProjectile.h"

#include "Kismet/GameplayStatics.h"

void AComChainProjectile::Initialize(TSubclassOf<UGameplayEffect> InHitActorGameplayEffect, 
                                     TObjectPtr<UGameplayAbility> InInstigatorAbility)
{
    Super::Initialize(InHitActorGameplayEffect, InInstigatorAbility);
    RemainingChains = MaxChains;
}

void AComChainProjectile::SetChainParameters(int32 InMaxChains, float InChainRadius)
{
    MaxChains = InMaxChains;
    ChainRadius = InChainRadius;
    RemainingChains = MaxChains;
}

bool AComChainProjectile::IsValidTarget(AActor* Actor) const
{
    if (!Super::IsValidTarget(Actor))
        return false;

    return !HitActors.Contains(Actor);
}

void AComChainProjectile::HandleActorHit(AActor* HitActor)
{
    if (HitActorEffect)
        UGameplayStatics::SpawnEmitterAtLocation(this, HitActorEffect, GetActorLocation(), GetActorRotation());
    
    HitActors.Add(HitActor);

    ApplyGameplayEffect(HitActor);

    if (RemainingChains > 0)
    {        
        if (AActor* NextTarget = FindNextChainTarget(HitActor->GetActorLocation()))
            SpawnChainProjectile(NextTarget);
    }

    Destroy();
}

AActor* AComChainProjectile::FindNextChainTarget(const FVector& CurrentLocation)
{
    TArray<FHitResult> HitResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(ChainRadius);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetInstigator());
    
    for (AActor* HitActor : HitActors)
        QueryParams.AddIgnoredActor(HitActor);

    GetWorld()->SweepMultiByChannel(HitResults, CurrentLocation, CurrentLocation, FQuat::Identity, ECC_Pawn, Sphere,
        QueryParams);

    AActor* ClosestTarget = nullptr;
    float ClosestDistance = ChainRadius;

    for (const FHitResult& Hit : HitResults)
    {
        if (IsValidTarget(Hit.GetActor()))
        {
            float Distance = FVector::Dist(CurrentLocation, Hit.GetActor()->GetActorLocation());
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestTarget = Hit.GetActor();
            }
        }
    }

    return ClosestTarget;
}

void AComChainProjectile::SpawnChainProjectile(AActor* NextTarget)
{
    if (!GetWorld() || !NextTarget)
        return;

    FVector SpawnLocation = GetActorLocation();
    FVector Direction = (NextTarget->GetActorLocation() - SpawnLocation).GetSafeNormal();
    FRotator SpawnRotation = Direction.Rotation();

    AComChainProjectile* ChainProjectile = GetWorld()->SpawnActorDeferred<AComChainProjectile>(GetClass(),
        FTransform(SpawnRotation, SpawnLocation), GetOwner(), GetInstigator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (ChainProjectile)
    {
        ChainProjectile->Initialize(HitActorGameplayEffect, InstigatorAbility);
        ChainProjectile->SetChainParameters(MaxChains, ChainRadius);
        ChainProjectile->RemainingChains = RemainingChains - 1;        
        ChainProjectile->HitActors = HitActors;

        ChainProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}