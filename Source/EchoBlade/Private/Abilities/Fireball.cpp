// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Fireball.h"

#include "GameplayTagsManager.h"

UFireball::UFireball()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.Ranged.FireBall");
}

void UFireball::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
}

void UFireball::Stop_Implementation(AActor* instigator, bool WasInterrupted)
{
	Super::Stop_Implementation(instigator, WasInterrupted);
}

void UFireball::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
}

void UFireball::OnAbilityRemoved_Implementation(AActor* instigator)
{
	Super::OnAbilityRemoved_Implementation(instigator);
}

void UFireball::OnAbilityStarted_Implementation(AActor* instigator)
{
	Super::OnAbilityStarted_Implementation(instigator);
}

void UFireball::OnAbilityStopped_Implementation(AActor* instigator)
{
	Super::OnAbilityStopped_Implementation(instigator);
}

bool UFireball::CanStartAbility_Implementation(AActor* instigator)
{
	return Super::CanStartAbility_Implementation(instigator);
}

bool UFireball::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
