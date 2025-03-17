// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EchoBlade/EchoBlade.h"
#include "EchoBladeGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UEchoBladeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


	FGameplayAbilitySpecHandle HandleTemp;
	FGameplayAbilityActorInfo ActorInfoTemp;
	FGameplayAbilityActivationInfo ActivationInfoTemp;

public:
	UEchoBladeGameplayAbility();

	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAbilitySpecHandle HandleTemp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAbilityActorInfo ActorInfoTemp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAbilityActivationInfo ActivationInfoTemp;
	*/

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void EchoEndAbility();
	/*
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	                        */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	// Properties
	FAbilityTriggerData triggerData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ability")
	EAbilityInputID AbilityID = EAbilityInputID::None;

	// When true, the ability will automatically activate when its bound input is pressed. Set to false to handle activation manually
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool bActivateOnInput;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability")
	bool ActivateAbilityOnGranted = false;

	// Functions
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> EffectClass) const;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	// Setters
	UFUNCTION(BlueprintCallable)
	void SetHandleTemp(FGameplayAbilitySpecHandle Handle);
	UFUNCTION(BlueprintCallable)
	void SetActorInfoTemp(FGameplayAbilityActorInfo ActorInfo);
	UFUNCTION(BlueprintCallable)
	void SetActivationInfoTemp(FGameplayAbilityActivationInfo ActivationInfo);

	// Getters
	virtual FAbilityTriggerData GetTriggerData();
	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle GetHandleTemp();
	UFUNCTION(BlueprintCallable)
	FGameplayAbilityActorInfo GetActorInfoTemp();
	UFUNCTION(BlueprintCallable)
	FGameplayAbilityActivationInfo GetActivationInfoTemp();

	// virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec)const override;
	// OnEndAbility
	// Clean up
};
