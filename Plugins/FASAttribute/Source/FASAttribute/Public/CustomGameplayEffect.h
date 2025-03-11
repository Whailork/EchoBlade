// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ASActionSystemComponent.h"
#include "AttributeSystemComponent.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "TimerManager.h"
#include "CustomGameplayEffect.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EModifierOperation : uint8
{
	Add UMETA(DisplayName = "Add"),
	Subtract UMETA(DisplayName = "Subtract"),
	Multiply UMETA(DisplayName = "Multiply"),
	Divide UMETA(DisplayName = "Divide")
};

USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	FGameplayTag TargetAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	EModifierOperation Operation;

   
};


UCLASS(Blueprintable)
class FASATTRIBUTE_API UCustomGameplayEffect : public UObject
{
	GENERATED_BODY()

public:
	UCustomGameplayEffect();

	FOnEffectAdded addedDelegate;
	FOnEffectRemoved removedDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bLooping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bStoppedByEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Period;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FGameplayTag TagToAdd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FGameplayTagContainer BlockingTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FAttributeModifier AttributeModifiers;

	UPROPERTY()
	AActor* InstigatorActor;
	UPROPERTY()
	UWorld* world;
	UPROPERTY()
	TSoftObjectPtr<UAttributeSystemComponent> InstgatorAttributeComponent;
	FTimerManager* tm; 
	FTimerHandle effectHandle;
	virtual void BeginDestroy() override;
	UFUNCTION(BlueprintNativeEvent)
	void OnEffectAdded(AActor* instigator);
	UFUNCTION()
	void DestroyEffect(AActor* instigator);
	UFUNCTION(BlueprintNativeEvent)
	void OnEffectRemoved(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent)
	void OnEffectTriggered();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void InitializeValues(float inDuration, float inPeriod,FGameplayTag AffectedAttributeTag,float effectPower,bool inLooping, bool inStoppedByEvent);
	UFUNCTION()
	void StopPeriodTimer();
};
