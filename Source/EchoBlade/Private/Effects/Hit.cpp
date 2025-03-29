// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Hit.h"

#include "GameplayTagsManager.h"

UHit::UHit()
{
}

void UHit::OnEffectAdded_Implementation(AActor* instigator)
{
	Super::OnEffectAdded_Implementation(instigator);
	
}

void UHit::OnEffectRemoved_Implementation(AActor* instigator)
{
	Super::OnEffectRemoved_Implementation(instigator);
}

void UHit::OnEffectTriggered_Implementation()
{
	Super::OnEffectTriggered_Implementation();
}

void UHit::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag, float effectPower,bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag,effectPower, inLooping, inStoppedByEvent);
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit");
	BlockingTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Dodging"));
	BlockingTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Blocking"));
	AttributeModifiers.Operation = EModifierOperation::Subtract;
	AttributeModifiers.Value = effectPower;
	AttributeModifiers.TargetAttribute = AffectedAttributeTag;
}


