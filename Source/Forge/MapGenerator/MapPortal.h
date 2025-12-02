#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapPortal.generated.h"

class UBoxComponent;

UCLASS()
class FORGE_API AMapPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapPortal();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PortalMesh;

	bool bPlayerInside = false;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
