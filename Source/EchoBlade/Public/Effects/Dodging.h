// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameplayEffect.h"
#include "UGameplayEffect.h"
#include "Dodging.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UDodging : public UCustomGameplayEffect
{
	GENERATED_BODY()
public:
	UDodging();
	virtual void InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag, float effectPower, bool inLooping, bool inStoppedByEvent) override;

};
