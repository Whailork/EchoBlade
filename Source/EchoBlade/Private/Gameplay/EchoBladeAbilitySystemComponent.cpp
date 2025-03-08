// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/EchoBladeAbilitySystemComponent.h"

#include "Abilities/EchoBladeGameplayAbility.h"


// Sets default values for this component's properties
UEchoBladeAbilitySystemComponent::UEchoBladeAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEchoBladeAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	Super::AbilityLocalInputPressed(InputID);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pressed!"));

	// Consume the input if this InputID is overloaded with GenericConfirm/Cancel and the GenericConfim/Cancel callback is bound
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	/*----------------------------------------------------------------------------------------------------*/

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			if (Spec.IsActive())
			{
				if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputPressed(Spec.Handle);
				}

				AbilitySpecInputPressed(Spec);

				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle,
				                      Spec.ActivationInfo.GetActivationPredictionKey());
			}
			else
			{
				UEchoBladeGameplayAbility* GA = Cast<UEchoBladeGameplayAbility>(Spec.Ability);
				if (GA && GA->bActivateOnInput)
				{
					const FGameplayAbilitySpecHandle Handle = Spec.Handle;
					const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
					const FGameplayAbilityActivationInfo ActivationInfo = Spec.ActivationInfo;

					GA->SetHandleTemp(Handle);
					GA->SetActorInfoTemp(*ActorInfo);
					GA->SetActivationInfoTemp(ActivationInfo);

					if (TryActivateAbility(Handle))
					{
						/*
						if (Spec.Ability->CommitAbility(Handle, ActorInfo, ActivationInfo))
						{
							
							ApplyGameplayEffectToTarget(
								UGameplayEffect* GameplayEffect,
								UAbilitySystemComponent* Target,
								float Level = UGameplayEffect::INVALID_LEVEL,
								FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(),
								FPredictionKey PredictionKey = FPredictionKey()
							)
							OR
							ApplyGameplayEffectToSelf(GameplayEffect, level, effect.contextHandle, prdictionKey)
							

							// Spec.Ability->ApplyGameplayEffect();
						}
						*/
					}
				}
			}
			return;
		}
	}
}
