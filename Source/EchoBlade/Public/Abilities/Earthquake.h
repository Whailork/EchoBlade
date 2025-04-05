// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "UAbility.h"
#include "Earthquake.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UEarthquake : public UAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraSystem* VFX;
	UNiagaraComponent* SpawnedVFX;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float EarthquakeRange;
	float EffectPower;
	UEarthquake();

	virtual void Start_Implementation(AActor* instigator) override;
	virtual void Stop_Implementation(AActor* instigator, bool WasInterrupted) override;
	virtual bool CanStartAbility_Implementation(AActor* instigator) override;

	UFUNCTION(BlueprintCallable)
	void TriggerEarthquake(AActor* instigator);
};
