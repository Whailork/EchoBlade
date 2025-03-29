// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwordAttack.h"
#include "UAbility.h"
#include "Components/CapsuleComponent.h"
#include "CircleSlash.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API UCircleSlash : public UAbility
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	UCapsuleComponent* SwordCollision;
	UPROPERTY()
	USwordAttack* SwordAttackAbility;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SlashRange;
	AActor* Instigator;
	FTimerHandle SlashTimerHandle;
	TArray<AActor*> HitActors;
	UCircleSlash();
	virtual void Start_Implementation(AActor* instigator) override;
	virtual void Stop_Implementation(AActor* instigator,bool WasInterrupted) override;
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
	virtual void OnAbilityRemoved_Implementation(AActor* instigator) override;
	virtual void OnAbilityStarted_Implementation(AActor* instigator) override;
	virtual void OnAbilityStopped_Implementation(AActor* instigator) override;
	virtual bool CanStartAbility_Implementation(AActor* instigator) override;
	virtual bool CanAddAbility_Implementation(AActor* instigator) override;

	UFUNCTION(BlueprintCallable)
	void StartSlashTimer(AActor* instigator);
	UFUNCTION(BlueprintCallable)
	void StopSlashTimer(AActor* instigator);
	
	UFUNCTION()
	void Slash();
};
