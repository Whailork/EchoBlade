// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UAbility.h"
#include "UObject/Object.h"
#include "PlayerData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FUpgradeData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAbility> LinkedAbility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag LinkedAttribute;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCustomGameplayEffect> LinkedEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UpgradeValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimesBought;
	
	
};


UCLASS(BlueprintType)
class ECHOBLADE_API UPlayerData : public UObject
{
	GENERATED_BODY()
public:
	UPlayerData();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CurrentPoints;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SpentPoints;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FGameplayTag,FUpgradeData> PlayerUpgrades;
};
