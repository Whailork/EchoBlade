// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameplayEffect.h"
#include "UGameplayEffect.h"
#include "Hit.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UHit : public UCustomGameplayEffect
{
	GENERATED_BODY()
public:
	UHit();
	virtual void OnEffectAdded_Implementation(AActor* instigator) override;
	virtual void OnEffectRemoved_Implementation(AActor* instigator) override;
	virtual void OnEffectTriggered_Implementation() override;
	UFUNCTION(BlueprintCallable)
	virtual void InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag,float effectPower, bool inLooping, bool inStoppedByEvent) override;
	
};
