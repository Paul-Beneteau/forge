#include "MapPortal.h"

#include "MapGenerator.h"
#include "Components/BoxComponent.h"
#include "Forge/Item/Generation/ItmWorldItem.h"
#include "Kismet/GameplayStatics.h"

AMapPortal::AMapPortal()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(Root);
	PortalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(Root);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AMapPortal::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMapPortal::OnTriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMapPortal::OnTriggerEndOverlap);
}

void AMapPortal::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || bPlayerInside)
		return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn && Pawn->IsPlayerControlled())
	{
		bPlayerInside = true;

		if (AMapGenerator* MapGenerator = Cast<AMapGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapGenerator::StaticClass())))
		{
			MapGenerator->ClearMap();
			MapGenerator->GenerateMap();
			MapGenerator->TeleportPlayerToStart();

			// Destroy inventory world item 
			TArray<AActor*> WorldItems;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItmWorldItem::StaticClass(), WorldItems);
			
			for (AActor* WorldItem : WorldItems)
			{
				if (WorldItem)
					WorldItem->Destroy();
			}
		}
	}
}

void AMapPortal::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn && Pawn->IsPlayerControlled())
	{
		bPlayerInside = false;
		UE_LOG(LogTemp, Log, TEXT("Player exited portal:"));
	}
}

