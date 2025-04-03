// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoBladeGameInstance.h"
#include "EnemyFighter.h"
#include "SpawnLocation.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyWaveSubsystem.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(WaveLog, Log, All);
DECLARE_DYNAMIC_DELEGATE(FNewWaveDelegate);
DECLARE_DYNAMIC_DELEGATE(FNewUpgradePoint);

USTRUCT(BlueprintType)
struct  FEnemyTypeData
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FUpgradeData> enemyUpgrades;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMesh* EnemyMesh;
	
};

USTRUCT(BlueprintType)
struct  FEnemyData
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FEnemyTypeData type;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int enemyNb;
	
};

USTRUCT(BlueprintType)
struct  FWaveInfoDataTable : public FTableRowBase
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FEnemyData> enemies;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SpawnRate;
	
};

UCLASS()
class ECHOBLADE_API UEnemyWaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int WaveNumber;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FNewWaveDelegate> NewWaveDelegates;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FNewUpgradePoint> NewUpgradePointsDelegates;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UDataTable> WavesTable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AEnemyFighter> EnemyClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FTimerHandle> SpawnTimers;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AActor*> SpawnLocations;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int> SpawnNumbers;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SpawnFrequency;
	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* waves, TSubclassOf<AEnemyFighter> classToSpawn);
	UFUNCTION(BlueprintCallable)
	void SpawnNextWave();
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(FEnemyData spawnData, FTimerHandle SpawnTimerHandle, int i);
	UFUNCTION(BlueprintCallable)
	void OnFighterDefeated();
	UFUNCTION(BlueprintCallable)
	void AddNewWaveDelegate(FNewWaveDelegate delegate);
	UFUNCTION(BlueprintCallable)
	void AddNewUpgradePointDelegate(FNewUpgradePoint delegate);
};





