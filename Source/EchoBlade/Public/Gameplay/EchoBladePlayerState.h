// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/EchoBladeAbilitySystemComponent.h"
#include "Attribute/HealthAttributeSet.h"

#include "EchoBladePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API AEchoBladePlayerState : public APlayerState, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AEchoBladePlayerState();

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float MaxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float maxShield;

	UPROPERTY()
	UEchoBladeAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UHealthAttributeSet> HealthAS;

	// Functions
	virtual void BeginPlay() override;														// DONE
	virtual void HealthTagAddedorRemoved(FGameplayTag GameplayTag, int tagCount);			// DONE
	virtual void HealthChanged(const FOnAttributeChangeData& Data);							// DONE

	// Getters
	virtual UHealthAttributeSet* GetHealthAttributeSet() const;								// DONE
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;	// DONE

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual UEchoBladeAbilitySystemComponent* GetAbilitySystemComponent() const override;
};


