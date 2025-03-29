// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Dodge.h"

#include "Fighter.h"
#include "GameplayTagsManager.h"
#include "PlayerFighter.h"
#include "Components/CapsuleComponent.h"
#include "Effects/Dodging.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


UDodge::UDodge()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Dodge");
	bCanInterrupt = false;
	ManaCost = 25;
	bInterruptOnHit = false;
	RollForce = 4000000;
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
	DodgeEffect->InitializeValues(0,0.0001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),ManaCost,false,true);
	instigator->GetComponentByClass<UAttributeSystemComponent>()->AddEffect(DodgeEffect);

}

void UDodge::Stop_Implementation(AActor* instigator,bool WasInterrupted)
{
	Super::Stop_Implementation(instigator,WasInterrupted);
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
	if(/*instigator->GetVelocity().Length() > 1 && */Super::CanStartAbility_Implementation(instigator) && outValue >= ManaCost && character->CanJump())
	{
		return true;
	}
	return false;
}

bool UDodge::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}

FVector UDodge::FindRollVector(AActor* instigator)
{
	AFighter* fighter = Cast<AFighter>(instigator);
	FRotator rotation = FRotator(0,fighter->GetControlRotation().Yaw,0);
	FVector rightVector = UKismetMathLibrary::GetRightVector(rotation);
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(rotation);
	if(instigator->IsA(APlayerFighter::StaticClass()))
	{
		
		FVector2d movementVector  = Cast<APlayerFighter>(fighter)->MovementVector;
		UKismetMathLibrary::Normalize2D(movementVector);
		return (rightVector * movementVector.X * RollForce) + (forwardVector * movementVector.Y * RollForce);
	}
	
		float randomForward = UKismetMathLibrary::RandomFloatInRange(-1,0);
		float randomRight = UKismetMathLibrary::RandomFloatInRange(-1,1);
		return (forwardVector * randomForward * RollForce) +(rightVector * randomRight * RollForce);
	
}
