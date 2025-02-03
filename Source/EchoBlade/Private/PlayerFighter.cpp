// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/PlayerFighter.h"

#include "AbilitySystemComponent.h"
#include "AttributeSystemComponent.h"
#include "EchoBladeGameInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsManager.h"
#include "EchoBlade/EchoBladeCharacter.h"
#include "Kismet/GameplayStatics.h"


APlayerFighter::APlayerFighter()
{
	
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
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	//bind death to attribute changed delegate
	AttributeSystemComponent->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),deathDelegate);
	deathDelegate.BindDynamic(this,&APlayerFighter::OnHealthChanged);
	
}

void APlayerFighter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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

void APlayerFighter::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Death"));
	UGameplayStatics::OpenLevel(this, FName("/Game/Maps/Lvl_SkillTreeMenu"), true);
	///Script/Engine.World'/Game/Maps/Lvl_SkillTreeMenu.Lvl_SkillTreeMenu'
}

void APlayerFighter::OnHealthChanged(FGameplayTag tag,float min,float current,float max)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HealthChanged"));
	if(current <= min)
	{
		OnDeath();
	}
}



