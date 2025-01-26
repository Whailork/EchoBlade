// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/Block.h"
#include "GameplayTagsManager.h"




UBlock::UBlock()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block");
}

void UBlock::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
}

void UBlock::Stop_Implementation(AActor* instigator)
{
	Super::Stop_Implementation(instigator);
}

void UBlock::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
}

void UBlock::OnAbilityRemoved_Implementation(AActor* instigator)
{
	Super::OnAbilityRemoved_Implementation(instigator);
}

void UBlock::OnAbilityStarted_Implementation(AActor* instigator)
{
	Super::OnAbilityStarted_Implementation(instigator);
}

void UBlock::OnAbilityStopped_Implementation(AActor* instigator)
{
	Super::OnAbilityStopped_Implementation(instigator);
}

bool UBlock::CanStartAbility_Implementation(AActor* instigator)
{
	return Super::CanStartAbility_Implementation(instigator);
}

bool UBlock::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
