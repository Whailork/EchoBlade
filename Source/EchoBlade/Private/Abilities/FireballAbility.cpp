// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FireballAbility.h"

void UFireballAbility::Start_Implementation(AActor* Instigator)
{
	Super::Start_Implementation(Instigator);
}

void UFireballAbility::Stop_Implementation(AActor* Instigator, bool WasInterrupted)
{
	Super::Stop_Implementation(Instigator, WasInterrupted);
}

void UFireballAbility::OnAbilityAdded_Implementation(AActor* Instigator)
{
	Super::OnAbilityAdded_Implementation(Instigator);
}

void UFireballAbility::OnAbilityRemoved_Implementation(AActor* Instigator)
{
	Super::OnAbilityRemoved_Implementation(Instigator);
}

void UFireballAbility::OnAbilityStarted_Implementation(AActor* Instigator)
{
	Super::OnAbilityStarted_Implementation(Instigator);
}

void UFireballAbility::OnAbilityStopped_Implementation(AActor* Instigator)
{
	Super::OnAbilityStopped_Implementation(Instigator);
}

bool UFireballAbility::CanStartAbility_Implementation(AActor* Instigator)
{
	return Super::CanStartAbility_Implementation(Instigator);
}

bool UFireballAbility::CanAddAbility_Implementation(AActor* Instigator)
{
	return Super::CanAddAbility_Implementation(Instigator);
}
