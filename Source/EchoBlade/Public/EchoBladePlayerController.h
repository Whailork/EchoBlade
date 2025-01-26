// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EchoBladePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API AEchoBladePlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void OnPossess(APawn* InPawn) override;
};
