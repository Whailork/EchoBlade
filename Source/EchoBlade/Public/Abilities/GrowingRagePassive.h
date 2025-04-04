// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "GrowingRagePassive.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UGrowingRagePassive : public UAbility
{
	GENERATED_BODY()
public:
	FAttributeChangedDelegate HealthChangedDelegate;
	UPROPERTY()
	UAttributeSystemComponent* AttributeComp;
	float BaseDamage = -1;
	UGrowingRagePassive();
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
	UFUNCTION()
	void OnHealthChanged(FGameplayTag tag, float min, float current, float max);
};
