// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/Fighter.h"

#include "..\..\..\Plugins\FASAttribute\Source\FASAttribute\Public\CustomAbilitySystem.h"
#include "AttributeSystemComponent.h"
#include "GameplayTagsManager.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeSystemComponent = CreateDefaultSubobject<UAttributeSystemComponent>("AttributeSystemComponent");
	AddOwnedComponent(AttributeSystemComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystem>("CustomAbilitySystem");
	AddOwnedComponent(AbilitySystemComponent);
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>("CharacterTrajectory");
	AddOwnedComponent(CharacterTrajectory);

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>("SwordCollision");
	SwordCollision->SetupAttachment(GetMesh(),"Sword_Tip");
	SwordCollision->SetAutoActivate(false);
	SwordCollision->Deactivate();

	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource");
	AIStimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	AIStimuliSource->RegisterWithPerceptionSystem();
	
}



void AFighter::SwordAttack()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack"));
}

void AFighter::Block()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
}

void AFighter::StopBlock()
{
	UAbility* returnAbility = AbilitySystemComponent->AbilityInUse();

	if(returnAbility != nullptr)
	{
		if(returnAbility->AbilityTag == UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"))
		{
			AbilitySystemComponent->StopAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
		}
	}
	
}

void AFighter::Dodge()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Dodge"));
}


// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFighter::Jump()
{
	Super::Jump();
}

