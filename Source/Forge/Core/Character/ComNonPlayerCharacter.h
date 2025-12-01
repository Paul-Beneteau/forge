#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ComNonPlayerCharacter.generated.h"

class UGameplayEffect;
class UComAbilitySystemComponent;
class UComCombatAttributeSet;

// Non player character used to test combat system
UCLASS()
class FORGE_API AComNonPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AComNonPlayerCharacter();

	virtual void BeginPlay() override;

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UAnimMontage> DeathMontage = nullptr;
	
	UFUNCTION()
	void HandleHealthChanged(AActor* EffectInstigator, float OldValue, float NewValue);

	void Die();
};
