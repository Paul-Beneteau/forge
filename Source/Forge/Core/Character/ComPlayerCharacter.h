#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ComPlayerDataAsset.h"
#include "ComPlayerCharacter.generated.h"

class UGameplayEffect;
class UComAbilitySystemComponent;
class UComDamageModifierAttributeSet;
class UComCombatAttributeSet;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawn);

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
	
	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> AbilityClass);

	// Remove the current ability bound to the input action and binds the new ability
	UFUNCTION(BlueprintCallable)
	void SetInputActionAbility(UInputAction* InputAction, TSubclassOf<UGameplayAbility> Ability);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;

	/** Time Threshold to know if it was a  short click to set destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ClickToDestinationThreshold { 0.3f };	
	float SetDestinationTriggerDuration { 0.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UComPlayerDataAsset> PlayerData;

	// Saves input action handles to dynamically modify input ability binding.
	UPROPERTY()
	TMap<TObjectPtr<UInputAction>, uint32> InputHandleMap;
	
	// Saves input action handles to dynamically modify input ability binding.
	UPROPERTY(BlueprintReadOnly)
	TMap<TObjectPtr<UInputAction>, TSubclassOf<UGameplayAbility>> InputAbilityMap;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay = 4.0f;
	
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	UFUNCTION()
	void HandleHealthChanged(AActor* EffectInstigator, float OldValue, float NewValue);

	void Die();

	UFUNCTION()
	void Respawn();
};
