// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/Fighter.h"

#include "..\..\..\Plugins\FASAttribute\Source\FASAttribute\Public\CustomAbilitySystem.h"
#include "AttributeSystemComponent.h"
#include "EchoBladeGameInstance.h"
#include "GameplayTagsManager.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeSystemComponent = CreateDefaultSubobject<UAttributeSystemComponent>("AttributeSystemComponent");
	AddOwnedComponent(AttributeSystemComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystem>("CustomAbilitySystem");
	AddOwnedComponent(AbilitySystemComponent);
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>("CharacterTrajectory");
	AddOwnedComponent(CharacterTrajectory);

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>("SwordCollision");
	SwordCollision->SetupAttachment(GetMesh(),"Sword_Tip");
	SwordCollision->SetAutoActivate(false);
	SwordCollision->Deactivate();

	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource");
	AIStimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	AIStimuliSource->RegisterWithPerceptionSystem();
	
}



void AFighter::SwordAttack()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack"));
}

void AFighter::Block()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
}

void AFighter::StopBlock()
{
	UAbility* returnAbility = AbilitySystemComponent->AbilityInUse();

	if(returnAbility != nullptr)
	{
		if(returnAbility->AbilityTag == UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"))
		{
			AbilitySystemComponent->StopAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
		}
	}
	
}

void AFighter::Dodge()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Dodge"));
}

void AFighter::ProcessUpgrades(TArray<FUpgradeData> upgrades)
{
	for(auto upgrade : upgrades)
	{
		if(upgrade.TimesBought > 0)
		{
			if(upgrade.LinkedAbility != nullptr)
			{
				AbilitySystemComponent->AddAbility(NewObject<UAbility>(this,upgrade.LinkedAbility->GetClass()));
			}
			if(upgrade.LinkedEffect != nullptr)
			{
				AttributeSystemComponent->AddEffect(NewObject<UGameplayEffect>(this,upgrade.LinkedEffect->GetClass()));
			}
			if(upgrade.LinkedAttribute.IsValid())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, upgrade.LinkedAttribute.ToString());
				float maxValue = -1;
				float value = -1;
				AttributeSystemComponent->GetAttributeMaxValue(upgrade.LinkedAttribute,maxValue);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::SanitizeFloat(maxValue));
				AttributeSystemComponent->SetAttributeMaxValue(upgrade.LinkedAttribute,maxValue + (upgrade.UpgradeValue*upgrade.TimesBought));
				AttributeSystemComponent->GetAttributeValue(upgrade.LinkedAttribute,value);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::SanitizeFloat(value));
				AttributeSystemComponent->SetAttributeValue(upgrade.LinkedAttribute,value + (upgrade.UpgradeValue*upgrade.TimesBought));
			}
		}
		
	}
	AttributeSystemComponent->FillUpAttributes();
}

void AFighter::BeginDestroy()
{
	//UAbility* ability = AbilitySystemComponent->AbilityInUse();
	/*if(ability != nullptr)
	{
		ability->Stop(this);
	}*/


	//AttributeSystemComponent->EffectsContainer.Empty();
	Super::BeginDestroy();
	
}


// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFighter::Jump()
{
	Super::Jump();
}

