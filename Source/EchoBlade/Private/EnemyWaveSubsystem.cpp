﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWaveSubsystem.h"
#include "PlayerFighter.h"
#include "Kismet/GameplayStatics.h"

void UEnemyWaveSubsystem::Init(UDataTable* waves, TSubclassOf<AEnemyFighter> classToSpawn)
{
	if(!waves->IsValidLowLevel())
	{
		UE_LOG(LogTemp,Warning,TEXT("Initializing EnemyWaveSubsystem"));
	}
	EnemyClass = classToSpawn;
	WavesTable = waves;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnLocation::StaticClass(), SpawnLocations);
	SpawnNextWave();
}

void UEnemyWaveSubsystem::SpawnNextWave()
{
	FWaveInfoDataTable* nextWave = WavesTable.LoadSynchronous()->FindRow<FWaveInfoDataTable>(FName(FString::FromInt(WaveNumber)),"");

	if(nextWave)
	{
		SpawnTimers.Empty();
		SpawnNumbers.Empty();
		for (int i = 0; i < nextWave->enemies.Num();i++)
		{
			FTimerHandle tempTimerHandle;
			
			FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject(this, &UEnemyWaveSubsystem::SpawnEnemy, nextWave->enemies[i], tempTimerHandle,i);
			SpawnDelegate.BindUFunction(this,FName("SpawnEnemy"),nextWave->enemies[i],tempTimerHandle,i);
			GetWorld()->GetTimerManager().SetTimer(tempTimerHandle,SpawnDelegate,nextWave->SpawnRate,true);
			SpawnTimers.Add(tempTimerHandle);
			SpawnNumbers.Add(nextWave->enemies[i].enemyNb);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(enemyData.enemyNb));
		}
	}
	
}

void UEnemyWaveSubsystem::SpawnEnemy(FEnemyData spawnData,FTimerHandle SpawnTimerHandle, int i)
{
	if(SpawnNumbers.Num() > i)
	{
		if(SpawnNumbers[i] > 0)
		{
			if(!SpawnLocations.IsEmpty())
			{
				FActorSpawnParameters spawnInfo;
				int locationIndex = FMath::RandRange(0,SpawnLocations.Num() -1);
				FVector location = SpawnLocations[locationIndex]->GetActorLocation();
				FRotator rotation = SpawnLocations[locationIndex]->GetActorRotation();
				AEnemyFighter* newActor = Cast<AEnemyFighter>(GetWorld()->SpawnActor(EnemyClass,&location,&rotation,spawnInfo));
				if(newActor)
				{
					newActor->GetMesh()->SetSkeletalMesh(spawnData.type.EnemyMesh);
					newActor->ProcessUpgrades(spawnData.type.enemyUpgrades);
					SpawnNumbers[i]--;
				}
			
			}
		}
		else
		{
			if(SpawnTimers.Num() > i)
			{
				GetWorld()->GetTimerManager().ClearTimer(SpawnTimers[i]);
				SpawnTimers.RemoveAt(i);
				SpawnTimerHandle.Invalidate();
			}
			
		
		}
	}
	
}

void UEnemyWaveSubsystem::OnFighterDefeated()
{
	
	if(SpawnTimers.IsEmpty())
	{
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemyFighter::StaticClass(),outActors);
		if(outActors.Num() == 1)
		{
			//wave finished
			WaveNumber++;
			SpawnNextWave();
			TArray<AActor*> players;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerFighter::StaticClass(),players);
			for (auto Player : players)
			{
				UPlayerData* thisPlayerData = Cast<UEchoBladeGameInstance>(Player->GetGameInstance())->GetPlayerData(Cast<APlayerController>(Cast<APlayerFighter>(Player)->GetController()));
				thisPlayerData->CurrentPoints += 1;
			}
			//Cast<UEchoBladeGameInstance>(GetWorld()->GetGameInstance())->CurrentPoints += 1;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Start new wave"));
		}
	}
}
