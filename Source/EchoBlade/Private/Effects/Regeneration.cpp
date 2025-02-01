// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Regeneration.h"

#include "GameplayTagsManager.h"

void URegeneration::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag,float effectPower, bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag, effectPower, inLooping,inStoppedByEvent);
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Regeneration");
	AttributeModifiers.Operation = EModifierOperation::Add;
	AttributeModifiers.Value = effectPower;
	AttributeModifiers.TargetAttribute = AffectedAttributeTag;	
}
