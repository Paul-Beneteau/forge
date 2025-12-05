#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFramework/Character.h"
#include "ComNonPlayerCharacter.generated.h"

class UGameplayAbility;
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
