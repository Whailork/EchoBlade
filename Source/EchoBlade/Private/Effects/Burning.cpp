// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Burning.h"

#include "GameplayTagsManager.h"

void UBurning::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag,float effectPower, bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag, effectPower, inLooping,inStoppedByEvent);

	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Burning");
	AttributeModifiers.Operation = EModifierOperation::Subtract;
	AttributeModifiers.Value = effectPower;
	AttributeModifiers.TargetAttribute = AffectedAttributeTag;
}

void UBurning::OnEffectAdded_Implementation(AActor* instigator)
{
	Super::OnEffectAdded_Implementation(instigator);
	//spawn particles on actor
	
}

void UBurning::OnEffectRemoved_Implementation(AActor* instigator)
{
	Super::OnEffectRemoved_Implementation(instigator);
	
	//remove particles from actor
}
