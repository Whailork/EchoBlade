// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBladeGameInstance.h"

UEchoBladeGameInstance::UEchoBladeGameInstance()
{
	CurrentPoints = 0;
	SpentPoints = 0;
}

void UEchoBladeGameInstance::UpgradeBought(FGameplayTag upgradeTag)
{
	FUpgradeData* UpgradeData = PlayerUpgrades.Find(upgradeTag);
	if(UpgradeData)
	{
		UpgradeData->TimesBought++;
	}
		
}
