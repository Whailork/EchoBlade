// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Blocking.h"

#include "..\..\..\..\Plugins\FASAttribute\Source\FASAttribute\Public\CustomAbilitySystem.h"
#include "GameplayTagsManager.h"

UBlocking::UBlocking()
{
    
}

void UBlocking::OnEffectAdded_Implementation(AActor* instigator)
{
	Super::OnEffectAdded_Implementation(instigator);
	float outValue = -1;
	InstigatorActor->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(AttributeModifiers.TargetAttribute,outValue);
	if(outValue < AttributeModifiers.Value)
	{
		InstigatorActor->GetComponentByClass<UCustomAbilitySystem>()->StopAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
		StopPeriodTimer();
	}
	else
	{
		OnEffectTriggered_Implementation();
	}
}


void UBlocking::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag,float effectPower, bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag, effectPower, inLooping,inStoppedByEvent);
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Blocking");
	AttributeModifiers.Operation = EModifierOperation::Subtract;
	AttributeModifiers.Value = effectPower;
	AttributeModifiers.TargetAttribute = AffectedAttributeTag;
}

void UBlocking::OnEffectTriggered_Implementation()
{
	float outValue = -1;
	InstigatorActor->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(AttributeModifiers.TargetAttribute,outValue);
	
	if(outValue >= AttributeModifiers.Value)
	{
		Super::OnEffectTriggered_Implementation();
	}
	else
	{
		InstigatorActor->GetComponentByClass<UCustomAbilitySystem>()->StopAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
		StopPeriodTimer();
	}
}
