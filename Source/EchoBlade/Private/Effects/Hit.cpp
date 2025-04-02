// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Hit.h"

#include "CustomAbilitySystem.h"
#include "GameplayTagsManager.h"
#include "Effects/Burning.h"
#include "Kismet/KismetMathLibrary.h"

UHit::UHit()
{
}

void UHit::OnEffectAdded_Implementation(AActor* instigator)
{
	Super::OnEffectAdded_Implementation(instigator);
	
	//50 % chance to apply burn if outer has a flaming sword
	if(auto Outer = Cast<AActor>(GetOuter()))
	{
		UCustomAbilitySystem* AbilitySystem = Outer->GetComponentByClass<UCustomAbilitySystem>();
		UAttributeSystemComponent* AttributeSystemComponent = instigator->GetComponentByClass<UAttributeSystemComponent>();
		if(AbilitySystem && AttributeSystemComponent){
			if(AbilitySystem->HasAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Passive.FlamingSword")))
			{
				
				bool ApplyBurn = UKismetMathLibrary::RandomBool();
				if(ApplyBurn){
					UCustomGameplayEffect* BurningEffect = NewObject<UBurning>(GetOuter());
					BurningEffect->InitializeValues(5,1,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),AttributeModifiers.Value/2,true,false);
					AttributeSystemComponent->AddEffect(BurningEffect);
				}
				
				
			}
		}
	}
	
}

void UHit::OnEffectRemoved_Implementation(AActor* instigator)
{
	Super::OnEffectRemoved_Implementation(instigator);
}

void UHit::OnEffectTriggered_Implementation()
{
	Super::OnEffectTriggered_Implementation();
}

void UHit::InitializeValues_Implementation(float inDuration, float inPeriod, FGameplayTag AffectedAttributeTag, float effectPower,bool inLooping, bool inStoppedByEvent)
{
	Super::InitializeValues_Implementation(inDuration, inPeriod, AffectedAttributeTag,effectPower, inLooping, inStoppedByEvent);
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit");
	BlockingTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Dodging"));
	BlockingTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Blocking"));
	AttributeModifiers.Operation = EModifierOperation::Subtract;
	AttributeModifiers.Value = effectPower;
	AttributeModifiers.TargetAttribute = AffectedAttributeTag;
}


