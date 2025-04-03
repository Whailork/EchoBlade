// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HasteAbility.h"

#include "GameplayTagsManager.h"
#include "Effects/HasteEffect.h"

UHasteAbility::UHasteAbility()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Buff.Haste");
	ManaCost = 10;
}

void UHasteAbility::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
	UAttributeSystemComponent* AttributeComp = instigator->GetComponentByClass<UAttributeSystemComponent>();
	if(AttributeComp)
	{
		UHasteEffect* HasteEffect = NewObject<UHasteEffect>(instigator);
		HasteEffect->InitializeValues(5,0,UGameplayTagsManager::Get().RequestGameplayTag("Effect.Haste"),300,true,false);
		AttributeComp->AddEffect(HasteEffect);
	}
	//remove cost
	float StaminaValue;
	AttributeComp->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),StaminaValue);
	AttributeComp->SetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),StaminaValue - ManaCost);
	Stop(instigator,false);
}

void UHasteAbility::Stop_Implementation(AActor* instigator, bool WasInterrupted)
{
	Super::Stop_Implementation(instigator, WasInterrupted);
}

bool UHasteAbility::CanStartAbility_Implementation(AActor* instigator)
{
	float outValue = -1;
	instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Stamina"),outValue);
	if(Super::CanStartAbility_Implementation(instigator) && outValue >= ManaCost)
	{
		return true;
	}
	return false;
}
