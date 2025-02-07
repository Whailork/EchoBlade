// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fighter.h"
#include "UObject/Object.h"
#include "PerceptionInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ECHOBLADE_API UPerceptionInfo : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<AFighter>> FightersInSight;
	
};
