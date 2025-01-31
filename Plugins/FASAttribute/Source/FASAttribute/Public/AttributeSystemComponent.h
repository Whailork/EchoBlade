// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "AttributeSystemComponent.generated.h"

class UUASAttributeSet;
//Attribute Delagates
DECLARE_DYNAMIC_DELEGATE_FourParams(FAttributeChangedDelegate, FGameplayTag, attributeTag, float, min, float, current,float, max);
DECLARE_DYNAMIC_DELEGATE_FourParams(FAttributeAddedDelegate,FGameplayTag, attributeTag, float,min,float,current,float,max);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAttributeRemovedDelegate, FGameplayTag, attributeTag);

//effect Delegates
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEffectAdded,AActor*,instigator);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEffectRemoved,AActor*,instigator);


//attribute holders
USTRUCT()
struct FAttributeChangedHolder
{
	GENERATED_BODY();
	static int currentHandle;
	int i;
	FAttributeChangedDelegate changedDelegate;
	//FAttributeChangedHolder(int i) : i(i) {}
	//FAttributeChangedHolder(int i, FAttributeChangedDelegate delegate) : i(i), changedDelegate(delegate) {}
};

USTRUCT()
struct FAttributeAddedHolder
{
	GENERATED_BODY();
	static int currentHandle;
	int i;
	FAttributeAddedDelegate addedDelegate;
	//FAttributeAddedHolder(int i) : i(i) {}
	//FAttributeAddedHolder(int i, FAttributeChangedDelegate delegate) : i(i), addedDelegate(delegate) {}
};
USTRUCT()
struct FAttributeRemovedHolder
{
	GENERATED_BODY();
	static int currentHandle;
	int i;
	FAttributeRemovedDelegate removedDelegate;
};

//effect holder
USTRUCT()
struct FEffectRemovedHolder
{
	GENERATED_BODY()
	static int currentHandle;
	int i;
	FOnEffectRemoved effectRemovdDelegate;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FASATTRIBUTE_API UAttributeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	//les attributes
	TArray<FAttribute> Attributes;
	TArray<FAttributeChangedHolder> arrChangedDelegates;
	TArray<FAttributeAddedHolder> arrAddedDelegates;
	TArray<FAttributeRemovedHolder> arrRemovedDelegates;

	// les effects
	UPROPERTY(EditAnywhere, Category = "Effects")
	FGameplayTagContainer EffectsTagContainer;
	TMap<FGameplayTag,FEffectRemovedHolder> mapEffectRemoved;
	
public:	
	// Sets default values for this component's properties
	UAttributeSystemComponent();

	UPROPERTY(EditAnywhere, Category="defaults")
	UUASAttributeSet* DefaultAttributes;

	void LoadDefaultAttributes();
	//getters et setters
	UFUNCTION(BlueprintCallable)
	bool SetAttributeValue(FGameplayTag tag, float newValue); // Set new value to attribute with specific tag
	UFUNCTION(BlueprintCallable)
	bool GetAttributeValue(FGameplayTag tag, float& outValue);
	UFUNCTION(BlueprintCallable)
	bool SetAttributeMaxValue(FGameplayTag tag, float newValue);
	UFUNCTION(BlueprintCallable)
	bool GetAttributeMaxValue(FGameplayTag tag, float& outValue);
	UFUNCTION(BlueprintCallable)
	bool SetAttributeMinValue(FGameplayTag tag, float newValue);
	UFUNCTION(BlueprintCallable)
	bool GetAttributeMinValue(FGameplayTag tag, float& outValue);
	UFUNCTION(BlueprintCallable)
	bool HasAttribute(FGameplayTag tag);
	UFUNCTION(BlueprintCallable)
	bool AddAttribute(const FAttribute& newAttribute);
	UFUNCTION(BlueprintCallable)
	bool RemoveAttribute(FGameplayTag tag);
	//fonctions pour les delegates
	UFUNCTION(BlueprintCallable)
	int AddAttributeChangedDelegate(FGameplayTag attributeTag,FAttributeChangedDelegate changedDelegate);
	UFUNCTION(BlueprintCallable)
	void RemoveAttributeChangedDelegate(int i);
	UFUNCTION(BlueprintCallable)
	int AddAttributeAddedDelegate(FAttributeAddedDelegate addedDelegate);
	UFUNCTION(BlueprintCallable)
	void RemoveAttributeAddedDelegate(int i);
	UFUNCTION(BlueprintCallable)
	int AddAttributeRemovedDelegate(FAttributeRemovedDelegate removedDelegate);
	UFUNCTION(BlueprintCallable)
	void RemoveAttributeRemovedDelegate(int i);

	//ajout et suppression des effets
	UFUNCTION(BlueprintCallable)
	void AddEffect(UGameplayEffect* effect);
	UFUNCTION(BlueprintCallable)
	void RemoveEffect(FGameplayTag effectTag);
	FGameplayTagContainer GetEffectsTagContainer();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
