// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/Block.h"
#include "GameplayTagsManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBlock::UBlock()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block");
}

void UBlock::Start_Implementation(AActor* instigator)
{
	
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BlockStart"));
}

void UBlock::Stop_Implementation(AActor* instigator)
{
	
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BlockStop"));
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
