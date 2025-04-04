// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "ThornsPassive.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UThornsPassive : public UAbility
{
	GENERATED_BODY()
public:
	UThornsPassive();
	FOnEffectAdded OnHitDelegate;
	UPROPERTY()
	UAttributeSystemComponent* AttributeComp;
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
	UFUNCTION()
	void OnHit(AActor* instigator);
};
