// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/PlayerFighter.h"

#include "..\..\..\Plugins\FASAttribute\Source\FASAttribute\Public\CustomAbilitySystem.h"
#include "AttributeSystemComponent.h"
#include "EchoBladeGameInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsManager.h"
#include "Attribute/HealthAttributeSet.h"
#include "Kismet/GameplayStatics.h"


APlayerFighter::APlayerFighter()
{
	//ProjectileClass = 
}

void APlayerFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerFighter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerFighter::Look);

		//basic abilities
		EnhancedInputComponent->BindAction(SwordAttackAction, ETriggerEvent::Triggered, this, &AFighter::SwordAttack);
		//basic abilities
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &AFighter::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AFighter::StopBlock);
		//basic abilities
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AFighter::Dodge);
		//CircleSlash
		EnhancedInputComponent->BindAction(CircleSlashAction, ETriggerEvent::Triggered, this, &AFighter::CircleSlash);
		//Earthquake
		EnhancedInputComponent->BindAction(EarthquakeAction, ETriggerEvent::Triggered, this, &AFighter::EarthQuake);
		//InstantHeal
		EnhancedInputComponent->BindAction(InstantHealAction, ETriggerEvent::Triggered, this, &AFighter::InstantHeal);
		//Haste
		EnhancedInputComponent->BindAction(HasteAction, ETriggerEvent::Triggered, this, &AFighter::Haste);
		
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AFighter::Shoot);
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	
	
}

void APlayerFighter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}


void APlayerFighter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerFighter::AddAbilityGAS(TSubclassOf<UEchoBladeGameplayAbility> NewAbility)
{
	Super::AddAbilityGAS(NewAbility);
}

void APlayerFighter::BeginPlay()
{
	Super::BeginPlay();
	AttributeSystemComponent->FillUpAttributes();
	
}

void APlayerFighter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	
}

void APlayerFighter::PossessedBy(AController* NewController)
{
	//bind death to attribute changed delegate
	deathDelegate.BindDynamic(this,&APlayerFighter::OnHealthChanged);
	AttributeSystemComponent->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),deathDelegate);

	//load upgrades
	UPlayerData* thisPlayerData = Cast<UEchoBladeGameInstance>(GetGameInstance())->GetPlayerData(Cast<APlayerController>(NewController));
	
	TArray<FUpgradeData> upgrades;
	if(!thisPlayerData->PlayerUpgrades.IsEmpty())
	{
		for (auto pair : thisPlayerData->PlayerUpgrades.Array())
		{
			upgrades.Add(pair.Value);
		}
		ProcessUpgrades(upgrades);
	}
	Super::PossessedBy(NewController);
}

void APlayerFighter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	FadeToBlackDelegate.ExecuteIfBound();
	FTimerHandle despawnHandle;
	GetWorldTimerManager().SetTimer(despawnHandle, this,&APlayerFighter::Despawn,1,false);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Death"));
	
	///Script/Engine.World'/Game/Maps/Lvl_SkillTreeMenu.Lvl_SkillTreeMenu'
}

void APlayerFighter::OnHealthChanged(FGameplayTag tag, float min, float current, float max)
{
	if (tag.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health")))
	{
		if (current <= min)
		{
			OnDeath();
		}
	}
	
}

void APlayerFighter::Despawn()
{
	UGameplayStatics::OpenLevel(this, FName("/Game/Maps/Lvl_SkillTreeMenu"), true);
}



