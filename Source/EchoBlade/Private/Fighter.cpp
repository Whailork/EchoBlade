// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/Fighter.h"

#include "..\..\..\Plugins\FASAttribute\Source\FASAttribute\Public\CustomAbilitySystem.h"
#include "AttributeSystemComponent.h"
#include "CustomGameplayEffect.h"
#include "EchoBladeGameInstance.h"
#include "EchoBladePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsManager.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/EchoBladeCharacter.h"
#include "Gameplay/EchoBladePlayerState.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeSystemComponent = CreateDefaultSubobject<UAttributeSystemComponent>("AttributeSystemComponent");
	AddOwnedComponent(AttributeSystemComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystem>("CustomAbilitySystem");
	AddOwnedComponent(AbilitySystemComponent);
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>("CharacterTrajectory");
	AddOwnedComponent(CharacterTrajectory);

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>("SwordCollision");
	SwordCollision->SetupAttachment(GetMesh(),"Sword_Tip");
	SwordCollision->SetAutoActivate(false);
	SwordCollision->Deactivate();

	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource");
	AIStimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	AIStimuliSource->RegisterWithPerceptionSystem();
	
}

void AFighter::AddTag(FGameplayTag tag)
{
	if (AvatarASC && !AvatarASC->HasMatchingGameplayTag(tag))
	{
		AvatarASC->AddLooseGameplayTag(tag);
	}
}

void AFighter::RemoveTag(FGameplayTag tag)
{
	if (AvatarASC && AvatarASC->HasMatchingGameplayTag(tag))
	{
		AvatarASC->RemoveLooseGameplayTag(tag);
	}
}

void AFighter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, "PossessedBy AController");

	AEchoBladePlayerState* PS = GetPlayerState<AEchoBladePlayerState>();

	if (PS)
	{
		// Set the ASC on the Server
		AvatarASC = Cast<UEchoBladeAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void AFighter::OnRep_PlayerState()
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

void AFighter::AddAbilityGAS(TSubclassOf<UEchoBladeGameplayAbility> NewAbility)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, "AddAbilityGAS");
	
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

void AFighter::SpawnProjectile(ACharacter* character)
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

void AFighter::SetAttribute(const FGameplayAttribute& Attribute, float NewBaseValue)
{
	AvatarASC->SetNumericAttributeBase(Attribute, NewBaseValue);
}

void AFighter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AvatarASC->GetOwnedGameplayTags(TagContainer);
}

UEchoBladeAbilitySystemComponent* AFighter::GetAbilitySystemComponent() const
{
	return AvatarASC;
}

int32 AFighter::GetAbilityLevel(EAbilityInputID AbilityID) const
{
	return GetAbilityLevel(AbilityID);
}


void AFighter::SwordAttack()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack"));
}

void AFighter::Block()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
}

void AFighter::StopBlock()
{
	UAbility* returnAbility = AbilitySystemComponent->AbilityInUse();

	if(returnAbility != nullptr)
	{
		if(returnAbility->AbilityTag == UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"))
		{
			AbilitySystemComponent->StopAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"));
		}
	}
	
}

void AFighter::Dodge()
{
	AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Dodge"));
}

void AFighter::ProcessUpgrades(TArray<FUpgradeData> upgrades)
{
	for(auto upgrade : upgrades)
	{
		if(upgrade.TimesBought > 0)
		{
			if(upgrade.LinkedAbility != nullptr)
			{
				AbilitySystemComponent->AddAbility(NewObject<UAbility>(this,upgrade.LinkedAbility->GetClass()));
			}
			if(upgrade.LinkedEffect != nullptr)
			{
				AttributeSystemComponent->AddEffect(NewObject<UCustomGameplayEffect>(this,upgrade.LinkedEffect->GetClass()));
			}
			if(upgrade.LinkedAttribute.IsValid())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, upgrade.LinkedAttribute.ToString());
				float maxValue = -1;
				float value = -1;
				AttributeSystemComponent->GetAttributeMaxValue(upgrade.LinkedAttribute,maxValue);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::SanitizeFloat(maxValue));
				AttributeSystemComponent->SetAttributeMaxValue(upgrade.LinkedAttribute,maxValue + (upgrade.UpgradeValue*upgrade.TimesBought));
				AttributeSystemComponent->GetAttributeValue(upgrade.LinkedAttribute,value);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::SanitizeFloat(value));
				AttributeSystemComponent->SetAttributeValue(upgrade.LinkedAttribute,value + (upgrade.UpgradeValue*upgrade.TimesBought));
			}
		}
		
	}
	AttributeSystemComponent->FillUpAttributes();
}

void AFighter::BeginDestroy()
{
	//UAbility* ability = AbilitySystemComponent->AbilityInUse();
	/*if(ability != nullptr)
	{
		ability->Stop(this);
	}*/


	//AttributeSystemComponent->EffectsContainer.Empty();
	Super::BeginDestroy();
	
}


// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();

	if (AvatarASC)
	{
		ASCInputBound = false;

		// gives Startup Abilities
		GiveAbilities();
		// TODO: ajouter addAbility a l'endroit voulu
		//AddAbilityGAS(UFireballAbility::StaticClass());
	}
}

void AFighter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	// TODO: regarder s'il faut AEchoBladePlayerController ou APlayerController
	if (AEchoBladePlayerController* PlayerController = Cast<AEchoBladePlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// TODO: voir si son absence fait bugger
			//Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFighter::bindToASC()
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

void AFighter::GiveAbilities()
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

void AFighter::addCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || AvatarASC) //  || AvatarASC->HasMatchingGameplayTag())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability could not be added to the character"));

		return;
	}

	for (TSubclassOf<UEchoBladeGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		const FGameplayAbilitySpec& AbilitySpec = FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this);
		
		AvatarASC->GiveAbility(AbilitySpec);
	}
	// AvatarASC->bCharacterAbilitiesGiven = true;
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponentTemp = PlayerInputComponent;

	// Bind to AbilitySystemComponent
	bindToASC();
}

void AFighter::Jump()
{
	Super::Jump();
}

