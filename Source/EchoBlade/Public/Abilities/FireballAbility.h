// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "FireballAbility.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UFireballAbility : public UAbility
{
	GENERATED_BODY()

	public:
	virtual void Start_Implementation(AActor* Instigator) override;
	virtual void Stop_Implementation(AActor* Instigator,bool WasInterrupted) override;
	virtual void OnAbilityAdded_Implementation(AActor* Instigator) override;
	virtual void OnAbilityRemoved_Implementation(AActor* Instigator) override;
	virtual void OnAbilityStarted_Implementation(AActor* Instigator) override;
	virtual void OnAbilityStopped_Implementation(AActor* Instigator) override;
	virtual bool CanStartAbility_Implementation(AActor* Instigator) override;
	virtual bool CanAddAbility_Implementation(AActor* Instigator) override;
};
