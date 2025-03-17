// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gameplay/EchoBladeCharacter.h"
#include "Abilities/EchoBladeGameplayAbility.h"
#include "EchoBladePlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "InputActionValue.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Gameplay/EchoBladePlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEchoBladeCharacter

AEchoBladeCharacter::AEchoBladeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AEchoBladeCharacter::AddTag(FGameplayTag tag)
{
	if (AvatarASC && !AvatarASC->HasMatchingGameplayTag(tag))
	{
		AvatarASC->AddLooseGameplayTag(tag);
	}
}

void AEchoBladeCharacter::RemoveTag(FGameplayTag tag)
{
	if (AvatarASC && AvatarASC->HasMatchingGameplayTag(tag))
	{
		AvatarASC->RemoveLooseGameplayTag(tag);
	}
}

void AEchoBladeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEchoBladePlayerState* PS = GetPlayerState<AEchoBladePlayerState>();

	if (PS)
	{
		// Set the ASC on the Server
		AvatarASC = Cast<UEchoBladeAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void AEchoBladeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AEchoBladePlayerState* PS = GetPlayerState<AEchoBladePlayerState>();
	if (PS)
	{
		// Set the ASC for clients
		AvatarASC = Cast<UEchoBladeAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients
		AvatarASC->InitAbilityActorInfo(PS, this);

		// Bind to AbilitySystemComponent
		bindToASC();
	}
}

void AEchoBladeCharacter::AddAbility(TSubclassOf<UEchoBladeGameplayAbility> NewAbility)
{
	if (GetLocalRole() != ROLE_Authority || !HasAuthority() || !AvatarASC) // || AvatarASC->HasMatchingGameplayTag())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability could not be added to the character"));
		return;
	}

	// GiveAbility to ASC
	const FGameplayAbilitySpec& AbilitySpec = FGameplayAbilitySpec(NewAbility, 1, //GetAbilityLevel(NewAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(NewAbility.GetDefaultObject()->AbilityInputID), this);
	AvatarASC->GiveAbility(AbilitySpec);
}

void AEchoBladeCharacter::SpawnProjectile(ACharacter* character)
{
	AEchoBladeCharacter* Character = Cast<AEchoBladeCharacter>(character);
	//Character->AddTag(Gameplay_Ability_Fireball); TODO: add it when ability is added not when activated???

	if (ProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;

		FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
		FRotator SpawnRotation = Character->GetActorRotation();
		FRotator Rotation = CameraRotation;
		Rotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = character;
			SpawnParams.Instigator = character->GetInstigator();

			if (AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, Rotation, SpawnParams))
			{
				//Projectile->InitializeValues(MyElement);
				//Projectile->ShootInDirection(Rotation.Vector());
			}
		}
	}
}

void AEchoBladeCharacter::SetAttribute(const FGameplayAttribute& Attribute, float NewBaseValue)
{
	AvatarASC->SetNumericAttributeBase(Attribute, NewBaseValue);
}

void AEchoBladeCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AvatarASC->GetOwnedGameplayTags(TagContainer);
}

UEchoBladeAbilitySystemComponent* AEchoBladeCharacter::GetAbilitySystemComponent() const
{
	return AvatarASC;
}

int32 AEchoBladeCharacter::GetAbilityLevel(EAbilityInputID AbilityID) const
{
	return GetAbilityLevel(AbilityID);
}

void AEchoBladeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (AvatarASC)
	{
		ASCInputBound = false;

		// gives Startup Abilities
		GiveAbilities();
		// TODO: ajouter addAbility a l'endroit voulu
		//AddAbility(UFireballAbility::StaticClass());
		
	}
}

void AEchoBladeCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	// TODO: regarder s'il faut AEchoBladePlayerController ou APlayerController
	if (AEchoBladePlayerController* PlayerController = Cast<AEchoBladePlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEchoBladeCharacter::bindToASC()
{
	if (!ASCInputBound && AvatarASC && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath =
			FTopLevelAssetPath(FName("/script/EchoBlade"), FName("EAbilityInputID"));
		AvatarASC->BindAbilityActivationToInputComponent(
			InputComponent,
			FGameplayAbilityInputBinds(
				FString("ConfirmTarget"),
				FString("CancelTarget"),
				AbilityEnumAssetPath,
				static_cast<int32>(EAbilityInputID::Confirm),
				static_cast<int32>(EAbilityInputID::Cancel)));
		ASCInputBound = true;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ASC bind to Enum EAbilityInputID"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not bind ASC to Enum EAbilityInputID"));
	}
}
// TODO: voir si fonction est utile ou inutile
void AEchoBladeCharacter::ActivateFire()
{
	if (AvatarASC)
	{
		//AvatarASC->TryActivateAbilityByClass(UFireballAbility::StaticClass());
	}
}

void AEchoBladeCharacter::GiveAbilities()
{
	/*
		if (HasAuthority() && AvatarASC)
		{
			TSubclassOf<UGameplayAbility> AbilityToGrant = YOUR_ABILITY_CLASS_HERE;
			if (AbilityToGrant)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityToGrant, 1); // Level 1 ability
				AvatarASC->GiveAbility(AbilitySpec);
			}
		}
		*/
}

void AEchoBladeCharacter::addCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || AvatarASC) // || AvatarASC->HasMatchingGameplayTag())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability could not be added to the character"));

		return;
	}

	/*
	*void AEchoBladeCharacter::AddAbility(TSubclassOf<UEchoBladeGameplayAbility> NewAbility)
{

	// GiveAbility to ASC
	const FGameplayAbilitySpec& AbilitySpec = FGameplayAbilitySpec(NewAbility, 1, //GetAbilityLevel(NewAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(NewAbility.GetDefaultObject()->AbilityInputID), this);
	AvatarASC->GiveAbility(AbilitySpec);
}
	 */

	for (TSubclassOf<UEchoBladeGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		// TODO: why not working?
		const FGameplayAbilitySpec& AbilitySpec = FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this);
		
		AvatarASC->GiveAbility(AbilitySpec);
	}
	// AvatarASC->bCharacterAbilitiesGiven = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEchoBladeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponentTemp = PlayerInputComponent;
	
	// Add Input Mapping Context
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEchoBladeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEchoBladeCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	// Bind to AbilitySystemComponent
	bindToASC();
}

void AEchoBladeCharacter::Move(const FInputActionValue& Value)
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

void AEchoBladeCharacter::Look(const FInputActionValue& Value)
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