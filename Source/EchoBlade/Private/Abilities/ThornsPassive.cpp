// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ThornsPassive.h"

#include "CustomGameplayEffect.h"
#include "GameplayTagsManager.h"
#include "Kismet/KismetMathLibrary.h"

UThornsPassive::UThornsPassive()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Passive.Thorns");
}

void UThornsPassive::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
	AttributeComp = instigator->GetComponentByClass<UAttributeSystemComponent>();
	OnHitDelegate.BindDynamic(this,&UThornsPassive::OnHit);
	if(AttributeComp)
	{
		AttributeComp->AddEffectAddedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit"),OnHitDelegate);
	}
}

void UThornsPassive::OnHit(AActor* instigator)
{
	int random = UKismetMathLibrary::RandomInteger(3);
	// one chance in three to return half the damage
	if(random == 0)
	{
		if(AttributeComp)
		{
			UCustomGameplayEffect* HitEffect = AttributeComp->GetEffect(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit"));
			AActor* OtherActor = Cast<AActor>(HitEffect->GetOuter());
			if(OtherActor)
			{
				UAttributeSystemComponent* OtherAttributeComp = OtherActor->GetComponentByClass<UAttributeSystemComponent>();
				if(OtherAttributeComp)
				{
					float OtherHealth = -1;
					OtherAttributeComp->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),OtherHealth);
					OtherAttributeComp->SetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),OtherHealth - (HitEffect->AttributeModifiers.Value/2));
				}
			}
		}
	}
	
}
