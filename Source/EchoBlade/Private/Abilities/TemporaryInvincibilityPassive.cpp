// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/TemporaryInvincibilityPassive.h"

#include "GameplayTagsManager.h"
#include "Effects/Invincibility.h"

UTemporaryInvincibilityPassive::UTemporaryInvincibilityPassive()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Passive.TemporaryInvincibility");
}

void UTemporaryInvincibilityPassive::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
	AttributeComp = instigator->GetComponentByClass<UAttributeSystemComponent>();
	OnHitDelegate.BindDynamic(this,&UTemporaryInvincibilityPassive::OnHit);
	if(AttributeComp)
	{
		AttributeComp->AddEffectAddedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit"),OnHitDelegate);
	}
}

void UTemporaryInvincibilityPassive::OnHit(AActor* instigator)
{
	UInvincibility* InvincibilityEffect = NewObject<UInvincibility>(this);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Invincibility");
	InvincibilityEffect->InitializeValues(1.5,0,FGameplayTag::EmptyTag,0,true,false);
	AttributeComp->AddEffect(InvincibilityEffect);
	//spawn particles?
}
