// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/HealthAttributeSet.h"
#include "AttributeSet.h"
#include "Net/UnrealNetwork.h"

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldHealth);
}
void UHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldMaxHealth);
}
void UHealthAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Mana, OldMana);
}
void UHealthAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxMana, OldMaxMana);
}
void UHealthAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Shield, OldShield);
}
void UHealthAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxShield, OldMaxShield);
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, MaxHealth.GetCurrentValue());
	}
	else if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, MaxMana.GetCurrentValue());
	}
	else if(Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, MaxShield.GetCurrentValue());
	}
	else
	{
		
	}

}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	/*
	 * PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data) only triggers after changes to the BaseValue of an Attribute from an instant GameplayEffect.
	 * This is a valid place to do more Attribute manipulation when they change from a GameplayEffect.
	 * For example, in the Sample Project we subtract the final damage Meta Attribute from the health Attribute here.
	 * If there was a shield Attribute, we would subtract the damage from it first before subtracting the remainder from health.
	 * The Sample Project also uses this location to apply hit react animations, show floating Damage Numbers, and assign experience and gold bounties to the killer.
	 * By design, the damage Meta Attribute will always come through an instant GameplayEffect and never the Attribute setter.
	 * Other Attributes that will only have their BaseValue changed from instant GameplayEffects like
		mana and stamina can also be clamped to their maximum value counterpart Attributes here.

	 * Note: When PostGameplayEffectExecute() is called, changes to the Attribute have already happened, but they have not replicated back to clients
		yet so clamping values here will not cause two network updates to clients. Clients will only receive the update after clamping.
	 */
}

//void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const { Super::GetLifetimeReplicatedProps(OutLifetimeProps); DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always); }

UHealthAttributeSet::UHealthAttributeSet()
{
}