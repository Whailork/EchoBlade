// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/EchoBladeGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
//#include "Effect/cooldownEffect.h"

#include "AbilitySystemBlueprintLibrary.h"

UEchoBladeGameplayAbility::UEchoBladeGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;//::InstancedPerActor;

	// Default tag that block this ability from activating
	//ActivationBlockedTags.AddTag(); TODO: FGameplayTag::RequestGameplayTag(FName("Gameplay.State.Dead"));

	// By default, abilities auto-activate on input.
	bActivateOnInput = true;
}

void UEchoBladeGameplayAbility::EchoEndAbility()
{
	EndAbility(GetHandleTemp(), &ActorInfoTemp, GetActivationInfoTemp(), true, true);
}
/*
void UEchoBladeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
*/
void UEchoBladeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UEchoBladeGameplayAbility::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> EffectClass) const
{
	if (!Target || !EffectClass) return;
	if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
	{
		FGameplayEffectContextHandle EffectContext = TargetAsc->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpec = TargetAsc->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
		if (EffectSpec.IsValid())
		{
			TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}
}

void UEchoBladeGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
}

void UEchoBladeGameplayAbility::SetHandleTemp(FGameplayAbilitySpecHandle Handle)
{
	HandleTemp = Handle;
}

void UEchoBladeGameplayAbility::SetActorInfoTemp(FGameplayAbilityActorInfo ActorInfo)
{
	ActorInfoTemp = ActorInfo;
}

void UEchoBladeGameplayAbility::SetActivationInfoTemp(FGameplayAbilityActivationInfo ActivationInfo)
{
	ActivationInfoTemp = ActivationInfo;
}

FAbilityTriggerData UEchoBladeGameplayAbility::GetTriggerData()
{
	return triggerData;
}

FGameplayAbilitySpecHandle UEchoBladeGameplayAbility::GetHandleTemp()
{
	return HandleTemp;
}

FGameplayAbilityActorInfo UEchoBladeGameplayAbility::GetActorInfoTemp()
{
	return ActorInfoTemp;
}

FGameplayAbilityActivationInfo UEchoBladeGameplayAbility::GetActivationInfoTemp()
{
	return ActivationInfoTemp;
}
