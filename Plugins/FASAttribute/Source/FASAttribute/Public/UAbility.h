#pragma once

#include "CoreMinimal.h"
#include "ASActionSystemComponent.h"

#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "UAbility.generated.h"

UCLASS(Blueprintable)
class FASATTRIBUTE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	UAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blockers")
	FGameplayTagContainer AbilityBlockerTags;
	bool bCanAdd;
	bool bCanStart;
	// Fonctions

	
	// Demarre l’ability
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void Start(AActor* instigator);
	// Arrete l’ability
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void Stop(AActor* instigator);

	// DELEGATES

	// Appelee quand l’ability est ajouter
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnAbilityAdded(AActor* instigator);
	// Appelee quand l’ability est supprimer
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnAbilityRemoved(AActor* instigator);
	// Appelee quand l’ability demarre
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnAbilityStarted(AActor* instigator);
	// Appelee quand l’ability arrete
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void OnAbilityStopped(AActor* instigator);
	// Appelee pour savoir si l’ability peut demarrer
	UFUNCTION(BlueprintNativeEvent, Category="Ability",BlueprintCallable)
	bool CanStartAbility(AActor* instigator);
	// Appelee pour savoir si l’ability peut s’ajouter a un component
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	bool CanAddAbility(AActor* instigator);
};
