// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/SwordAttack.h"
#include "GameplayTagsManager.h"




USwordAttack::USwordAttack()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack");
}

void USwordAttack::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
	
}

void USwordAttack::Stop_Implementation(AActor* instigator)
{
	Super::Stop_Implementation(instigator);
}

void USwordAttack::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
}

void USwordAttack::OnAbilityRemoved_Implementation(AActor* instigator)
{
	Super::OnAbilityRemoved_Implementation(instigator);
}

void USwordAttack::OnAbilityStarted_Implementation(AActor* instigator)
{
	Super::OnAbilityStarted_Implementation(instigator);
}

void USwordAttack::OnAbilityStopped_Implementation(AActor* instigator)
{
	Super::OnAbilityStopped_Implementation(instigator);
}

bool USwordAttack::CanStartAbility_Implementation(AActor* instigator)
{
	return Super::CanStartAbility_Implementation(instigator);
}

bool USwordAttack::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
