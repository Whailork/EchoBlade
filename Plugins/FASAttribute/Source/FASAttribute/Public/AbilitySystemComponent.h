// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UAbility.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.generated.h"

// Abilities DELEGATES
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAbilityAdded,FGameplayTag,AbilityTag,AActor*,instigator);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAbilityRemoved,FGameplayTag,AbilityTag,AActor*,instrigator);



USTRUCT()
struct FAbilityAddedHolder
{
	GENERATED_BODY()
	static int currentHandle;
	int i;
	FOnAbilityAdded abilityAddedDelegate;
	
};
USTRUCT()
struct FAbilityRemovedHolder
{
	GENERATED_BODY()
	static int currentHandle;
	int i;
	FOnAbilityRemoved abilityRemovedDelegate;
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FASATTRIBUTE_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

	
	//les abilities

	TArray<FAbilityAddedHolder> arrAbilityAddedDelegates;
	TArray<FAbilityRemovedHolder> arrAbilityRemovedDelegates;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UAbility*> Abilities;
	// Sets default values for this component's properties
	UAbilitySystemComponent();

	// Add an ability to the list Abilities (if doesn't contain it)
	UFUNCTION(BlueprintCallable, Category="Abilities")
	void AddAbility(UAbility* NewAbility);
	// Remove an ability from the list Abilities (if contains it)
	UFUNCTION(BlueprintCallable, Category="Abilities")
	void RemoveAbility(FGameplayTag tag);
	// Remove all abilities from the list Abilities (if not empty)
	UFUNCTION(BlueprintCallable, Category="Abilities")
	void RemoveAllAbilities();
	// Start an ability in the list Abilities (if contains it)
	UFUNCTION(BlueprintCallable, Category="Abilities")
	void TriggerAbility(FGameplayTag tag);
	UFUNCTION(BlueprintCallable, Category="Abilities")
	bool HasAbility(FGameplayTag AbilityTag);
	UFUNCTION(BlueprintCallable, Category="Abilities")
	UAbility* GetAbility(FGameplayTag AbilityTag);
	


	
	// pour les delegates des abilities
	UFUNCTION(BlueprintCallable)
	int AddAbilityAddedDelegate(FOnAbilityAdded addedDelegate);
	UFUNCTION(BlueprintCallable)
	void RemoveAbilityAddedDelegate(int i);
	UFUNCTION(BlueprintCallable)
	int AddAbilityRemovedDelegate(FOnAbilityRemoved removedDelegate);
	UFUNCTION(BlueprintCallable)
	void RemoveAbilityRemovedDelegate(int i);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
