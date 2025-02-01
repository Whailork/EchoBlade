// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UAbility.h"
#include "Components/CapsuleComponent.h"
#include "SwordAttack.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API USwordAttack : public UAbility
{
	GENERATED_BODY()

public:
	USwordAttack();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int AttackCount;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool isAttacking;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bHasHit;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UCapsuleComponent* SwordCollision;
	UFUNCTION(BlueprintCallable)
	void StartSwordCollision();
	UFUNCTION(BlueprintCallable)
	void StopSwordCollision();
	UFUNCTION(BlueprintCallable)
	void SetupSwordCollision(UCapsuleComponent* Sword);
	UFUNCTION(BlueprintCallable)
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
	
	virtual void Start_Implementation(AActor* instigator) override;
	virtual void Stop_Implementation(AActor* instigator) override;
	virtual void OnAbilityAdded_Implementation(AActor* instigator) override;
	virtual void OnAbilityRemoved_Implementation(AActor* instigator) override;
	virtual void OnAbilityStarted_Implementation(AActor* instigator) override;
	virtual void OnAbilityStopped_Implementation(AActor* instigator) override;
	virtual bool CanStartAbility_Implementation(AActor* instigator) override;
	virtual bool CanAddAbility_Implementation(AActor* instigator) override;

	
	
};
