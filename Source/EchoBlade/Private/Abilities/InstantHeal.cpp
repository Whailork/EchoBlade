// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/InstantHeal.h"

#include "GameplayTagsManager.h"
#include "Effects/Regeneration.h"
#include "Kismet/KismetMathLibrary.h"

UInstantHeal::UInstantHeal()
{
	ManaCost = 30;
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Buff.InstantHeal");
}

void UInstantHeal::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
	UAttributeSystemComponent* AttributeComp = instigator->GetComponentByClass<UAttributeSystemComponent>();
	float MaxValue = -1;
	float MinValue = -1;
	float Value = -1;
	AttributeComp->GetAttributeMaxValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),MaxValue);
	AttributeComp->GetAttributeMinValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),MinValue);
	AttributeComp->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),Value);

	float EffectPower = UKismetMathLibrary::Clamp(Value + MaxValue/2,MinValue,MaxValue);
	URegeneration* RegenEffect = NewObject<URegeneration>(instigator);
	RegenEffect->InitializeValues(0,0.0001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),EffectPower,false,false);
	AttributeComp->AddEffect(RegenEffect);
	Stop(instigator,false);
}

void UInstantHeal::Stop_Implementation(AActor* instigator, bool WasInterrupted)
{
	Super::Stop_Implementation(instigator, WasInterrupted);
}

bool UInstantHeal::CanStartAbility_Implementation(AActor* instigator)
{
	float outValue = -1;
	instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),outValue);
	if(Super::CanStartAbility_Implementation(instigator) && outValue >= ManaCost)
	{
		return true;
	}
	return false;
}
