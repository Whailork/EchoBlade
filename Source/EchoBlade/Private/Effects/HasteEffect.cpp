// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/HasteEffect.h"

#include "GameplayTagsManager.h"
#include "GameFramework/CharacterMovementComponent.h"

UHasteEffect::UHasteEffect()
{
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Haste");
}

void UHasteEffect::OnEffectAdded_Implementation(AActor* instigator)
{
	Super::OnEffectAdded_Implementation(instigator);
	CharacterMovementComponent = instigator->GetComponentByClass<UCharacterMovementComponent>();
	if(CharacterMovementComponent)
	{
		BaseSpeed = CharacterMovementComponent->MaxWalkSpeed;
		CharacterMovementComponent->MaxWalkSpeed = BaseSpeed + SpeedBoost;
	}
}

void UHasteEffect::OnEffectRemoved_Implementation(AActor* instigator)
{
	if(CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = BaseSpeed;
	}
	Super::OnEffectRemoved_Implementation(instigator);
	
}

void UHasteEffect::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag,float effectPower, bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag, effectPower, inLooping,inStoppedByEvent);
	SpeedBoost = effectPower;
}
