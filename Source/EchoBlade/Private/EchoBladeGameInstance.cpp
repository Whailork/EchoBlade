// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBladeGameInstance.h"

#include "Kismet/GameplayStatics.h"

UEchoBladeGameInstance::UEchoBladeGameInstance()
{
	KillsForLevelUp = 25;
	CurrentKills = 0;
}

UPlayerData* UEchoBladeGameInstance::GetPlayerData(APlayerController* playerController)
{
	int controllerId = UGameplayStatics::GetPlayerControllerID(playerController);
	
	//if player data already exists on load les upgrades
	if (AllPlayersData.Num() > controllerId)
	{
		return AllPlayersData[controllerId];
	}
	//else create new empty data
	else
	{
		UPlayerData* newPlayerData = NewObject<UPlayerData>();
		AllPlayersData.Add(newPlayerData);
		return newPlayerData;
	}

}

