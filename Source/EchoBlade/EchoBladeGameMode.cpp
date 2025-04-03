// Copyright Epic Games, Inc. All Rights Reserved.

#include "EchoBladeGameMode.h"

#include "CustomAbilitySystem.h"
#include "PlayerFighter.h"
#include "Gameplay/EchoBladeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AEchoBladeGameMode::AEchoBladeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APlayerFighter* PlayerFighter = Cast<APlayerFighter>(PlayerCharacter);
	if (!PlayerFighter) return;

	UCustomAbilitySystem* AbilitySystem = PlayerFighter->AbilitySystemComponent;
    for (UAbility* Ability : AbilitySystem->Abilities)
    {
    	AbilityTextures[Ability->AbilityTag];
    }

	AbilitySystem->Abilities[0];
}
