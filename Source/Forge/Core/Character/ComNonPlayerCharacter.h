#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ComNonPlayerCharacter.generated.h"

class UGameplayEffect;
class UComAbilitySystemComponent;
class UComCombatAttributeSet;

// AI Character class. Handle GAS components and character death
UCLASS()
class FORGE_API AComNonPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AComNonPlayerCharacter();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;

	// TODO: Evaluate if a data asset config is needed for these three members
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Death")
	TObjectPtr<UAnimMontage> DeathMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Death")
	float DeathDelay = 4.f;
	
	UFUNCTION()
	void HandleHealthChanged(AActor* EffectInstigator, float OldValue, float NewValue);

	void Die();
	
	void PlayDeathMontage();
	void TrySpawnItem();
};
