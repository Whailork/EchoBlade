// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UASAttributeSet.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FASAttributeData
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere)
	FGameplayTag attributeTag;
	UPROPERTY(EditAnywhere)
	float min;
	UPROPERTY(EditAnywhere)
	float current;
	UPROPERTY(EditAnywhere)
	float max;
};

UCLASS()
class FASATTRIBUTE_API UUASAttributeSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FASAttributeData> AttributeData;
	
};


