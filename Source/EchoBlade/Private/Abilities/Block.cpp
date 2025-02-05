// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/Block.h"
#include "GameplayTagsManager.h"
#include "Abilities/SpawnableActors/Shield.h"
#include "Components/ArrowComponent.h"
#include "Effects/Blocking.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBlock::UBlock()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block");
	bCanInterrupt = true;

	//ConstructorHelpers::FClassFinder<AActor> ActorToSpawn(TEXT("/Game/Blueprints/BP_AbilityShield"));
	Cost = 10;
	/*if (ActorToSpawn.Class) {
		shieldClass = ActorToSpawn.Class;
	}*/

}

void UBlock::Start_Implementation(AActor* instigator)
{
	
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("BeforeSpawn"));
	Shield = GetWorld()->SpawnActor<AShield>(shieldClass);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("AfterSpawn"));
	Shield->SetActorLocation(Character->GetActorLocation() + Character->GetActorForwardVector()*50);
	Shield->SetActorRotation(Character->GetActorRotation());
	Shield->AttachToActor(Character,FAttachmentTransformRules::KeepWorldTransform);

	BlockingEffect = NewObject<UBlocking>();
	BlockingEffect->InitializeValues(0,1,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),Cost,true,true);
	instigator->GetComponentByClass<UAttributeSystemComponent>()->AddEffect(BlockingEffect);

	Super::Start_Implementation(instigator);
}

void UBlock::Stop_Implementation(AActor* instigator)
{
	
	ACharacter* Character = Cast<ACharacter>(instigator);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	Shield->Destroy();
	BlockingEffect->StopPeriodTimer();
	instigator->GetComponentByClass<UAttributeSystemComponent>()->RemoveEffect(BlockingEffect->TagToAdd);
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
	float outValue = -1;
	instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),outValue);
	if(Super::CanStartAbility_Implementation(instigator) && outValue >= Cost)
	{
		return true;
	}
	return false;
}

bool UBlock::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
