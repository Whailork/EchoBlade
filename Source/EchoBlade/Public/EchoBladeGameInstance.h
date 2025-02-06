// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "UGameplayEffect.h"
#include "Engine/GameInstance.h"
#include "EchoBladeGameInstance.generated.h"

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
	TSubclassOf<UGameplayEffect> LinkedEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UpgradeValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimesBought;
	
	
};

UCLASS()
class ECHOBLADE_API UEchoBladeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEchoBladeGameInstance();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CurrentPoints;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SpentPoints;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FGameplayTag,FUpgradeData> PlayerUpgrades;

	UFUNCTION(BlueprintCallable)
	void UpgradeBought(FGameplayTag upgradeTag);
	
	
};
