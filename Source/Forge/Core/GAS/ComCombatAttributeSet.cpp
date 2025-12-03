#include "ComCombatAttributeSet.h"
#include "GameplayEffectExtension.h"

UComCombatAttributeSet::UComCombatAttributeSet()
{
	OnHealthRegenChanged.AddDynamic(this, &UComCombatAttributeSet::UpdateActiveEffect);
	OnManaRegenChanged.AddDynamic(this, &UComCombatAttributeSet::UpdateActiveEffect);
}

void UComCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Health value must be between 0 and MaxHealth
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		// Mana value must be between 0 and MaxMana
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxHealthAttribute() || Attribute == GetMaxManaAttribute())
	{
		// MaxMana value can't be below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetHealthRegenAttribute() || Attribute == GetManaRegenAttribute() || Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, NewValue);
	}
}

bool UComCombatAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	HealthBeforeChange = GetHealth();
	ManaBeforeChange = GetMana();
	
	return true;
}

void UComCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Rounding to int so there isn't a float damage value
		SetHealth(FMath::Clamp<int32>(FMath::RoundToInt(GetHealth() - GetDamage()), 0.0f, GetMaxHealth()));
		SetDamage(0.0f);
	}	
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		SetHealth(FMath::Clamp<int32>(FMath::RoundToInt(GetHealth() + GetHeal()), 0.0f, GetMaxHealth()));
		SetHeal(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{		
		SetMana(FMath::Clamp<int32>(FMath::RoundToInt(GetMana()), 0.0f, GetMaxMana()));
	}
}

void UComCombatAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetHealthAttribute() && OldValue != NewValue)
	{
		OnHealthChanged.Broadcast(nullptr, OldValue, NewValue);
	}
	else if (Attribute == GetManaAttribute() && OldValue != NewValue)
	{
		OnManaChanged.Broadcast(nullptr, OldValue, NewValue);
	}
	else if (Attribute == GetHealthRegenAttribute())
	{
		OnHealthRegenChanged.Broadcast(GetHealthAttribute(), GetHealthRegen(), FName("HealthRegenEffect"));
	}
	else if (Attribute == GetManaRegenAttribute())
	{
		OnManaRegenChanged.Broadcast(GetManaAttribute(), GetManaRegen(), FName("ManaRegenEffect"));
	}
}

// Apply health/mana regen active effect
void UComCombatAttributeSet::InitPeriodicAttributes()
{
	UpdateActiveEffect(GetHealthAttribute(), GetHealthRegen(), FName("HealthRegenEffect"));
	UpdateActiveEffect(GetManaAttribute(), GetManaRegen(), FName("ManaRegenEffect"));
}

// create a periodic gameplay effect spec and apply it to owning ability system component
void UComCombatAttributeSet::UpdateActiveEffect(FGameplayAttribute TargetAttribute, float EffectMagnitude, FName EffectName)
{
	// If there is already an active effect, remove it
	if (FActiveGameplayEffectHandle* EffectHandle = ActivePeriodicEffectMap.Find(TargetAttribute))
	{
		GetOwningAbilitySystemComponent()->RemoveActiveGameplayEffect(*EffectHandle);
	}
	
	UGameplayEffect* PeriodicEffect = NewObject<UGameplayEffect>(GetTransientPackage(), EffectName);

	// Set effect duration and period
	PeriodicEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	PeriodicEffect->Period = AttributePeriod;	

	// Attribute modifier
	FGameplayModifierInfo Modifier;
	Modifier.Attribute = TargetAttribute;
	Modifier.ModifierOp = EGameplayModOp::Additive;
	// Multiply magnitude by period so the magnitude value correspond to a period of 1 second
	Modifier.ModifierMagnitude = FScalableFloat(EffectMagnitude * AttributePeriod);
	PeriodicEffect->Modifiers.Add(Modifier);

	FGameplayEffectSpec EffectSpec = FGameplayEffectSpec(PeriodicEffect, GetOwningAbilitySystemComponent()->MakeEffectContext(), 1.f);

	FActiveGameplayEffectHandle EffectHandle = GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(EffectSpec);

	// Save the active effect
	ActivePeriodicEffectMap.Add(TargetAttribute, EffectHandle);
}