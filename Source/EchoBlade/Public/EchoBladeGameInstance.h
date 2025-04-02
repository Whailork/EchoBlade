// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "UAbility.h"
//#include "UGameplayEffect.h"
#include "Engine/GameInstance.h"
#include "EchoBladeGameInstance.generated.h"

/**
 * 
 */




UCLASS()
class ECHOBLADE_API UEchoBladeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEchoBladeGameInstance();
	int BestWaveNumber;
	int KillsForLevelUp;
	int CurrentKills;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UPlayerData*> AllPlayersData;
	UFUNCTION(BlueprintCallable)
	UPlayerData* GetPlayerData(APlayerController* playerController);
};
