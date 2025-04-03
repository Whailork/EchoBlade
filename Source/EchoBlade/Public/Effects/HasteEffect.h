// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HasteEffect.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UHasteEffect : public UCustomGameplayEffect
{
	GENERATED_BODY()
	float BaseSpeed;
	float SpeedBoost;
	UCharacterMovementComponent* CharacterMovementComponent;
public:
	UHasteEffect();
	virtual void OnEffectAdded_Implementation(AActor* instigator) override;
	virtual void OnEffectRemoved_Implementation(AActor* instigator) override;
	virtual void InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag, float effectPower, bool inLooping, bool inStoppedByEvent) override;
};
