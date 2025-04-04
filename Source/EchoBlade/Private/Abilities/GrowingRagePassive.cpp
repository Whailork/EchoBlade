// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GrowingRagePassive.h"

#include "GameplayTagsManager.h"

UGrowingRagePassive::UGrowingRagePassive()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Passive.GrowingRage");
}

void UGrowingRagePassive::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
	AttributeComp = instigator->GetComponentByClass<UAttributeSystemComponent>();
	HealthChangedDelegate.BindDynamic(this,&UGrowingRagePassive::OnHealthChanged);
	if(AttributeComp)
	{
		AttributeComp->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),HealthChangedDelegate);
	}
}

void UGrowingRagePassive::OnHealthChanged(FGameplayTag tag, float min, float current, float max)
{
	if(tag.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health")))
	{
		if(BaseDamage == -1)
		{
			AttributeComp->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Damage"),BaseDamage);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::FromInt(BaseDamage + BaseDamage*(1-(current/(max-min)))));
		AttributeComp->SetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Damage"),BaseDamage + BaseDamage*(1-(current/(max-min))));
	}
}
