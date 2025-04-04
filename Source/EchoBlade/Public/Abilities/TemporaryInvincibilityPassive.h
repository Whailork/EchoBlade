// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "TemporaryInvincibilityPassive.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UTemporaryInvincibilityPassive : public UAbility
{
	GENERATED_BODY()
public:
	FOnEffectAdded OnHitDelegate;
	UPROPERTY()
	UAttributeSystemComponent* AttributeComp;
	UTemporaryInvincibilityPassive();
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
	UFUNCTION()
	void OnHit(AActor* instigator);
};
