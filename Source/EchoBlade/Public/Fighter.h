// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Logging/LogMacros.h"
#include "EchoBlade/EchoBlade.h"
#include "AttributeSet.h"
#include "Gameplay/EchoBladeAbilitySystemComponent.h"
#include "Abilities/EchoBladeGameplayAbility.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CharacterTrajectoryComponent.h"
#include "EchoBladeGameInstance.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Attribute/HealthAttributeSet.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Projectile/Projectile.h"
#include "Fighter.generated.h"


class UEchoBladeAbilitySystemComponent;

UCLASS()
class ECHOBLADE_API AFighter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY()
	UInputComponent* PlayerInputComponentTemp;
	
public:
	// Sets default values for this character's properties
	AFighter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TObjectPtr<UHealthAttributeSet> HealthAS;

	// Properties
	bool ASCInputBound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UEchoBladeGameplayAbility>> CharacterAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystemComponent")
	UEchoBladeAbilitySystemComponent* AvatarASC; // TObjectPtr<>

	// Functions
	void AddTag(FGameplayTag tag);
	void RemoveTag(FGameplayTag tag);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	void HealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void InitializeDelegates();
	
	UFUNCTION(BlueprintCallable, Category= "Ability")
	virtual void AddAbilityGAS(TSubclassOf<UEchoBladeGameplayAbility> NewAbility);
	
	UFUNCTION(BlueprintCallable, Category = Projectile)
	void SpawnProjectile(AActor* character);

	// Setters
	UFUNCTION(BlueprintCallable)
	void SetAttribute( const FGameplayAttribute& Attribute,  float NewBaseValue);
	
	// Getters
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual UEchoBladeAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category= "Ability")
	virtual int32 GetAbilityLevel(EAbilityInputID AbilityID) const;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool IsStrafing;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UAttributeSystemComponent* AttributeSystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCustomAbilitySystem* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* SwordCollision;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UCharacterTrajectoryComponent* CharacterTrajectory;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIStimuliSource;
	

	void SwordAttack();
	void Block();
	void StopBlock();
	void Dodge();
	void ProcessUpgrades(TArray<FUpgradeData> upgrades);

	virtual void BeginDestroy() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;
	void bindToASC();
	//void ActivateFire();
	void GiveAbilities();
	void addCharacterAbilities();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

};


