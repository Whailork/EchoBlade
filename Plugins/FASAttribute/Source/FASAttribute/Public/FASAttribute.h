// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Modules/ModuleManager.h"
#include "FASAttribute.generated.h"


class FFASAttributeModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag attributeTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float min;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float current;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float max;
};

USTRUCT(BlueprintType)
struct  FASAIDefaultAttributesDataTable : public FTableRowBase
{
	GENERATED_BODY();
	UPROPERTY(EditDefaultsOnly)
	float MaxLife;
	UPROPERTY(EditDefaultsOnly)
	float MaxShield;
};

