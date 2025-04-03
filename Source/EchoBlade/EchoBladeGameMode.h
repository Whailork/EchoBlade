// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "GameFramework/GameModeBase.h"
#include "EchoBladeGameMode.generated.h"

UCLASS(minimalapi)
class AEchoBladeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEchoBladeGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dictionary")
	TMap<FGameplayTag, UTexture*> AbilityTextures;
};