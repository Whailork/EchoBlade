// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTrajectoryComponent.h"
#include "Fighter.generated.h"


UCLASS()
class ECHOBLADE_API AFighter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AFighter();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UAttributeSystemComponent* AttributeSystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* SwordCollision;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UCharacterTrajectoryComponent* CharacterTrajectory;

	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	void SwordAttack();
	void Block();
	void Dodge();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};


