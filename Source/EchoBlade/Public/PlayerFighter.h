// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystemComponent.h"
#include "Fighter.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayerFighter.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE(FFadeToBlackDelegate);
UCLASS()
class ECHOBLADE_API APlayerFighter : public AFighter
{
	GENERATED_BODY()
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void AddAbilityGAS(TSubclassOf<UEchoBladeGameplayAbility> NewAbility) override;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwordAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CircleSlashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EarthquakeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InstantHealAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HasteAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;
public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector2D MovementVector;
	UFUNCTION()
	void OnDeath();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	 FFadeToBlackDelegate FadeToBlackDelegate;
	UPROPERTY()
	FAttributeChangedDelegate deathDelegate;
	UFUNCTION()
	void OnHealthChanged(FGameplayTag tag,float min,float current,float max);
	UFUNCTION()
	void Despawn();
	APlayerFighter();
};
