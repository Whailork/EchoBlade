// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/Block.h"
#include "GameplayTagsManager.h"
#include "Abilities/SpawnableActors/Shield.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBlock::UBlock()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block");
	bCanInterrupt = true;

	ConstructorHelpers::FClassFinder<AActor> ActorToSpawn(TEXT("/Game/Blueprints/BP_AbilityShield"));

	if (ActorToSpawn.Class) {
		shieldClass = ActorToSpawn.Class;
	}
}

void UBlock::Start_Implementation(AActor* instigator)
{
	
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Shield = GetWorld()->SpawnActor<AShield>(shieldClass, Character->GetActorLocation() + Character->GetArrowComponent()->GetForwardVector()*50,Character->GetActorRotation());
	Shield->AttachToActor(Character,FAttachmentTransformRules::KeepWorldTransform);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BlockStart"));

	Super::Start_Implementation(instigator);
}

void UBlock::Stop_Implementation(AActor* instigator)
{
	
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	Shield->Destroy();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BlockStop"));

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
