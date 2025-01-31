// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ASWaveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FASATTRIBUTE_API UASWaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable)
	void InitializeAI(UDataTable* DataTable);
};
