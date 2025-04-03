#pragma once

#include "CoreMinimal.h"
#include "ASActionSystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "UObject/Object.h"
#include "UAbility.generated.h"

UCLASS(Blueprintable)
class FASATTRIBUTE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	UAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float AbilityCooldown = 0.5f;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category="UI")
	UImage* ImageTop;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blockers")
	FGameplayTagContainer AbilityBlockerTags;
	bool bCanAdd;
	bool bCanStart;
	bool bIsActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	float ManaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	float HealthCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInterrupt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInterruptOnHit;
	// Fonctions

	
	// Demarre l’ability
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	void Start(AActor* instigator);
	// Arrete l’ability
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	void Stop(AActor* instigator,bool WasInterrupted);

	// DELEGATES

	// Appelee quand l’ability est ajouter
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	void OnAbilityAdded(AActor* instigator);
	// Appelee quand l’ability est supprimer
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	void OnAbilityRemoved(AActor* instigator);
	// Appelee quand l’ability demarre
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	void OnAbilityStarted(AActor* instigator);
	// Appelee quand l’ability arrete
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	void OnAbilityStopped(AActor* instigator);
	// Appelee pour savoir si l’ability peut demarrer
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	bool CanStartAbility(AActor* instigator);
	// Appelee pour savoir si l’ability peut s’ajouter a un component
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	bool CanAddAbility(AActor* instigator);
	UFUNCTION(BlueprintCallable)
	bool IsInterruptOnHit();

	UFUNCTION(BlueprintCallable)
	void ChangeOpacity();
	void ResetOpacity();
};
