// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/Fighter.h"

#include "AbilitySystemComponent.h"
#include "AttributeSystemComponent.h"
#include "GameplayTagsManager.h"
#include "MovieSceneSequenceID.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeSystemComponent = CreateDefaultSubobject<UAttributeSystemComponent>("AttributeSystemComponent");
	AddOwnedComponent(AttributeSystemComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AddOwnedComponent(AbilitySystemComponent);
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>("CharacterTrajectory");
	AddOwnedComponent(CharacterTrajectory);
	

}

void AFighter::SwordAttack()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack"));
}

void AFighter::Block()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
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

