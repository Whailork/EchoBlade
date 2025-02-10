// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UGameplayEffect.h"
#include "Blocking.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UBlocking : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UBlocking();

	virtual void OnEffectAdded_Implementation(AActor* instigator) override;
	virtual void InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag, float effectPower, bool inLooping, bool inStoppedByEvent) override;
	virtual void OnEffectTriggered_Implementation() override;
};
