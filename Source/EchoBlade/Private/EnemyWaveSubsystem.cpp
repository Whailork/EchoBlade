// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWaveSubsystem.h"

#include "GameplayTagsManager.h"
#include "PlayerFighter.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(WaveLog);
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
	//fire delagates for new wave spawning
	for(FNewWaveDelegate delegate : NewWaveDelegates)
	{
		delegate.ExecuteIfBound();
	}
	FWaveInfoDataTable* Wave = WavesTable.LoadSynchronous()->FindRow<FWaveInfoDataTable>(FName(FString::FromInt(WaveNumber)),"");

	if(Wave)
	{
		nextWave = *Wave;
		AutoWaves = false;
		PremadeWavesNumber = WaveNumber;
	}
	else
	{
		AutoWaves = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "AutoWaves");
	}
	
		SpawnTimers.Empty();
		SpawnNumbers.Empty();
		for (int i = 0; i < nextWave.enemies.Num();i++)
		{
			FTimerHandle tempTimerHandle;
			
			FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject(this, &UEnemyWaveSubsystem::SpawnEnemy, nextWave.enemies[i], tempTimerHandle,i);
			SpawnDelegate.BindUFunction(this,FName("SpawnEnemy"),nextWave.enemies[i],tempTimerHandle,i);
			GetWorld()->GetTimerManager().SetTimer(tempTimerHandle,SpawnDelegate,nextWave.SpawnRate,true);
			SpawnTimers.Add(tempTimerHandle);
			SpawnNumbers.Add(nextWave.enemies[i].enemyNb);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(enemyData.enemyNb));
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
					//add health and damage if in auto waves mode
					if(AutoWaves)
					{
						spawnData.type.enemyUpgrades.Add(FUpgradeData(nullptr,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),nullptr,5,WaveNumber-PremadeWavesNumber));
						spawnData.type.enemyUpgrades.Add(FUpgradeData(nullptr,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Damage"),nullptr,2.5,WaveNumber-PremadeWavesNumber));
					}
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
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerFighter::StaticClass(),players);

	//add point with kill number
	UEchoBladeGameInstance* GameInstance = Cast<UEchoBladeGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->CurrentKills++;
	if(GameInstance->CurrentKills == GameInstance->KillsForLevelUp)
	{
		GameInstance->CurrentKills = 0;
		GameInstance->KillsForLevelUp += GameInstance->KillsForLevelUp/5;
		for (auto Player : players)
		{
			UPlayerData* thisPlayerData = GameInstance->GetPlayerData(Cast<APlayerController>(Cast<APlayerFighter>(Player)->GetController()));
			thisPlayerData->CurrentPoints += 1;
			for (auto delegate : NewUpgradePointsDelegates)
			{
				delegate.ExecuteIfBound();
			}
		}
			
	}
	
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemyFighter::StaticClass(),outActors);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::FromInt(outActors.Num()));
	UE_LOG(WaveLog,Log,TEXT("%d"),outActors.Num());
	// if only one actor left (the one currently dying)
	if(outActors.Num() <= 1)
	{
		//wave finished
		UE_LOG(WaveLog,Warning,TEXT("Spawn next wave"));
		WaveNumber++;
		SpawnNextWave();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerFighter::StaticClass(),players);
		//add point if new best wave
		if(WaveNumber > GameInstance->BestWaveNumber)
		{
			for (auto Player : players)
			{
				UPlayerData* thisPlayerData = GameInstance->GetPlayerData(Cast<APlayerController>(Cast<APlayerFighter>(Player)->GetController()));
				thisPlayerData->CurrentPoints += 1;
				for (auto delegate : NewUpgradePointsDelegates)
				{
					delegate.ExecuteIfBound();
				}
				GameInstance->BestWaveNumber = WaveNumber;
			}
		}
			
		//Cast<UEchoBladeGameInstance>(GetWorld()->GetGameInstance())->CurrentPoints += 1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Start new wave"));
	}
		
		
	
}

void UEnemyWaveSubsystem::AddNewWaveDelegate(FNewWaveDelegate delegate)
{
	NewWaveDelegates.Add(delegate);
}

void UEnemyWaveSubsystem::AddNewUpgradePointDelegate(FNewUpgradePoint delegate)
{
	NewUpgradePointsDelegates.Add(delegate);
}
