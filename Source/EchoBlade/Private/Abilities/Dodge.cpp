// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Dodge.h"
#include "GameplayTagsManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UDodge::UDodge()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Dodge");
	bCanInterrupt = false;
}

void UDodge::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UDodge::Stop_Implementation(AActor* instigator)
{
	Super::Stop_Implementation(instigator);
}

void UDodge::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
}

void UDodge::OnAbilityRemoved_Implementation(AActor* instigator)
{
	Super::OnAbilityRemoved_Implementation(instigator);
}

void UDodge::OnAbilityStarted_Implementation(AActor* instigator)
{
	Super::OnAbilityStarted_Implementation(instigator);
}

void UDodge::OnAbilityStopped_Implementation(AActor* instigator)
{
	Super::OnAbilityStopped_Implementation(instigator);
}

bool UDodge::CanStartAbility_Implementation(AActor* instigator)
{
	if(instigator->GetVelocity().Length() > 1 && Super::CanStartAbility_Implementation(instigator))
	{
		return true;
	}
	return false;

	
}

bool UDodge::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
