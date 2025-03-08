// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "EchoBlade/EchoBlade.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "AttributeSet.h"
#include "EchoBladeAbilitySystemComponent.h"
#include "Abilities/EchoBladeGameplayAbility.h"
#include "EchoBlade/Public/Projectile/Projectile.h"
#include "Logging/LogMacros.h"
#include "EchoBladeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AEchoBladeCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY()
	UInputComponent* PlayerInputComponentTemp;

public:
	AEchoBladeCharacter();

	// Properties
	bool ASCInputBound;
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UEchoBladeGameplayAbility>> CharacterAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystemComponent")
	UEchoBladeAbilitySystemComponent* AvatarASC; // TObjectPtr<>

	// Functions
	void AddTag(FGameplayTag tag);															// DONE
	void RemoveTag(FGameplayTag tag);														// DONE
	virtual void PossessedBy(AController* NewController) override;							// DONE
	virtual void OnRep_PlayerState() override;												// DONE
	
	UFUNCTION(BlueprintCallable, Category= "Ability")
	void AddAbility(TSubclassOf<UEchoBladeGameplayAbility> NewAbility);							// DONE
	
	UFUNCTION(BlueprintCallable, Category = Projectile)
	void SpawnProjectile(ACharacter* character);											// DONE

	// Setters
	UFUNCTION(BlueprintCallable)
	void SetAttribute( const FGameplayAttribute& Attribute,  float NewBaseValue);			// DONE
	
	// Getters
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;	// DONE
	virtual UEchoBladeAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category= "Ability")
	virtual int32 GetAbilityLevel(EAbilityInputID AbilityID) const;							// DONE
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);	// DONE

	/** Called for looking input */
	void Look(const FInputActionValue& Value);	// DONE
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// DONE
	
	// To add mapping context
	virtual void BeginPlay();							// DONE

	virtual void NotifyControllerChanged() override;	// DONE
	void bindToASC();									// DONE
	void ActivateFire();								// DONE
	void GiveAbilities();								// DONE
	void addCharacterAbilities();						// DONE

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

