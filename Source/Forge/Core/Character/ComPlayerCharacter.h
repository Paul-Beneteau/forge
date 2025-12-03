#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ComPlayerConfig.h"
#include "ComPlayerCharacter.generated.h"

class UItmInventoryRootWidget;
class UItmInventoryManager;
class UItmInventoryComponent;
class UItmEquipmentComponent;
class UGameplayEffect;
class UComAbilitySystemComponent;
class UComDamageModifierAttributeSet;
class UComCombatAttributeSet;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawn);

// Player Character. Handles inputs, GAS components, inventory components and character death.
UCLASS()
class FORGE_API AComPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// GAS Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComDamageModifierAttributeSet> DamageAttributeSet;
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	UPROPERTY(BlueprintAssignable, Category="Respawn")
	FOnPlayerRespawn OnPlayerRespawn;
	
	AComPlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> AbilityClass);

	// Remove the current ability bound to the input action and binds the new ability
	UFUNCTION(BlueprintCallable)
	void SetInputActionAbility(UInputAction* InputAction, TSubclassOf<UGameplayAbility> Ability);
	
protected:
	// Camera components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;

	// Inventory components
	UPROPERTY()
	TObjectPtr<UItmEquipmentComponent> EquipmentComp;
	UPROPERTY()
	TObjectPtr<UItmInventoryComponent> InventoryComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TObjectPtr<UItmInventoryManager> InventoryManager;
	
	/** Time Threshold to know if it was a  short click to set destination */
	float ClickToDestinationThreshold = 0.3f;
	float SetDestinationTriggerDuration = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UComPlayerConfig> PlayerConfig;

	// Saves input action handles to dynamically modify input ability binding.
	UPROPERTY()
	TMap<TObjectPtr<UInputAction>, uint32> InputHandleMap;	
	// Saves input action handles to dynamically modify input ability binding.
	UPROPERTY(BlueprintReadOnly)
	TMap<TObjectPtr<UInputAction>, TSubclassOf<UGameplayAbility>> InputAbilityMap;

	virtual void BeginPlay() override;
	
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	UFUNCTION()
	void HandleHealthChanged(AActor* EffectInstigator, float OldValue, float NewValue);

	void Die();
	
	void PlayDeathMontage();
	
	UFUNCTION()
	void Respawn();
};
