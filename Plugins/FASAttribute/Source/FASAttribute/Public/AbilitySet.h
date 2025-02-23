// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UAbility.h"
#include "Engine/DataAsset.h"
#include "AbilitySet.generated.h"

/**
 * 
 */



UCLASS()
class FASATTRIBUTE_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<UAbility>> AbilityData;
};
