// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "Fireball.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UFireball : public UAbility
{
	GENERATED_BODY()

public:
	UFireball();
	virtual void Start_Implementation(AActor* instigator) override;
	virtual void Stop_Implementation(AActor* instigator,bool WasInterrupted) override;
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
	virtual void OnAbilityRemoved_Implementation(AActor* instigator) override;
	virtual void OnAbilityStarted_Implementation(AActor* instigator) override;
	virtual void OnAbilityStopped_Implementation(AActor* instigator) override;
	virtual bool CanStartAbility_Implementation(AActor* instigator) override;
	virtual bool CanAddAbility_Implementation(AActor* instigator) override;
};
