// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Dodging.h"

#include "GameplayTagsManager.h"

UDodging::UDodging()
{
}

void UDodging::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag,float effectPower, bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag, effectPower, inLooping,inStoppedByEvent);
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Dodging");
	AttributeModifiers.Operation = EModifierOperation::Subtract;
	AttributeModifiers.Value = effectPower;
	AttributeModifiers.TargetAttribute = AffectedAttributeTag;
}

