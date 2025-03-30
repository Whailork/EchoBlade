// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Earthquake.h"

#include "GameplayTagsManager.h"
#include "Effects/Hit.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UEarthquake::UEarthquake()
{
	ManaCost = 20;
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.Ranged.EarthQuake");
	EarthquakeRange = 1000;
}

void UEarthquake::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);

	//remove manaCost from instigator
	UAttributeSystemComponent* AttributeSystemComponent = instigator->GetComponentByClass<UAttributeSystemComponent>();
	float StaminaValue;
	AttributeSystemComponent->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),StaminaValue);
	AttributeSystemComponent->SetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),StaminaValue - ManaCost);

}

void UEarthquake::Stop_Implementation(AActor* instigator, bool WasInterrupted)
{
	Super::Stop_Implementation(instigator, WasInterrupted);
}

bool UEarthquake::CanStartAbility_Implementation(AActor* instigator)
{
	float outValue;
	instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),outValue);
	if(Super::CanStartAbility_Implementation(instigator) && outValue >= ManaCost)
	{
		return true;
	}
	return false;
}

void UEarthquake::TriggerEarthquake(AActor* instigator)
{
	FVector Direction;
	if(auto character = Cast<ACharacter>(instigator))
	{
		
		Direction = FRotator(0,character->GetControlRotation().Yaw,0).RotateVector(FVector::ForwardVector);
	}
	TArray<FHitResult> outHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(instigator);
	auto channel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	UKismetSystemLibrary::SphereTraceMulti(instigator->GetWorld(),instigator->GetActorLocation(),instigator->GetActorLocation() + Direction*EarthquakeRange,80,channel,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,outHits,true);
	for (auto Hit : outHits)
	{
		UHit* hitEffect = NewObject<UHit>();
				
		hitEffect->InitializeValues(0,0.00001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),ManaCost*(1.5),false,false);
		UAttributeSystemComponent* AttributeSystemComponent = Hit.GetActor()->GetComponentByClass<UAttributeSystemComponent>();
		if(AttributeSystemComponent)
		{
			AttributeSystemComponent->AddEffect(hitEffect);
		}
	}
}
