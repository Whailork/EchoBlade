// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystemComponent.h"
#include "FASAttribute.h"
#include "UASAttributeSet.h"
#include "Components/ActorComponent.h"
#include "ASActionSystemComponent.generated.h"

class UAbility;





UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FASATTRIBUTE_API UASActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()



	
public:
	

	// Sets default values for this component's properties
	UASActionSystemComponent();



	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
