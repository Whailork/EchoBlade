// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "FlamingSwordPassive.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UFlamingSwordPassive : public UAbility
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UNiagaraComponent* FireTrail;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UNiagaraSystem* FlamingParticles;
	
	UFlamingSwordPassive();
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
};
