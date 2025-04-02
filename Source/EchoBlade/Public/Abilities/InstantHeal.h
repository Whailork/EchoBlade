// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "InstantHeal.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UInstantHeal : public UAbility
{
	GENERATED_BODY()
public :
	UInstantHeal();

	virtual void Start_Implementation(AActor* instigator) override;
	virtual void Stop_Implementation(AActor* instigator, bool WasInterrupted) override;
	virtual bool CanStartAbility_Implementation(AActor* instigator) override;
	
};
