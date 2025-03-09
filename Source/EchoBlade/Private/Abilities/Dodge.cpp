// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Dodge.h"
#include "GameplayTagsManager.h"
#include "Components/CapsuleComponent.h"
#include "Effects/Dodging.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UDodge::UDodge()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Dodge");
	bCanInterrupt = false;
	Cost = 25;
	bInterruptOnHit = false;
}

void UDodge::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	Character->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	DodgeEffect = NewObject<UDodging>();
	DodgeEffect->InitializeValues(0,0.0001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),Cost,false,true);
	instigator->GetComponentByClass<UAttributeSystemComponent>()->AddEffect(DodgeEffect);

}

void UDodge::Stop_Implementation(AActor* instigator)
{
	Super::Stop_Implementation(instigator);
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	instigator->GetComponentByClass<UAttributeSystemComponent>()->RemoveEffect(DodgeEffect->TagToAdd);
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
	float outValue = -1;
	ACharacter* character = Cast<ACharacter>(instigator);
	instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),outValue);
	if(/*instigator->GetVelocity().Length() > 1 && */Super::CanStartAbility_Implementation(instigator) && outValue >= Cost && character->CanJump())
	{
		return true;
	}
	return false;
}

bool UDodge::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
