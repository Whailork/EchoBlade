// Copyright Epic Games, Inc. All Rights Reserved.

#include "EchoBladeGameMode.h"
#include "Gameplay/EchoBladeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEchoBladeGameMode::AEchoBladeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
