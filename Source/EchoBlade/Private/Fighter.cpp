// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/Fighter.h"
#include "Projectile/Projectile.h"
#include "..\..\..\Plugins\FASAttribute\Source\FASAttribute\Public\CustomAbilitySystem.h"
#include "AttributeSystemComponent.h"
#include "CustomGameplayEffect.h"
#include "EchoBladeGameInstance.h"
#include "EchoBladePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsManager.h"
#include "RewindData.h"
#include "Chaos/ChaosPerfTest.h"
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
	SwordCollision->SetupAttachment(GetMesh(),"Sword_Mid");
	SwordCollision->SetAutoActivate(false);
	SwordCollision->Deactivate();

	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource");
	AIStimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	AIStimuliSource->RegisterWithPerceptionSystem();

	HealthAS = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
}

void AFighter::AddTag(const FGameplayTag Tag) const
{
	if (AvatarASC && !AvatarASC->HasMatchingGameplayTag(Tag))
	{
		AvatarASC->AddLooseGameplayTag(Tag);
	}
}

void AFighter::RemoveTag(const FGameplayTag Tag) const
{
	if (AvatarASC && AvatarASC->HasMatchingGameplayTag(Tag))
	{
		AvatarASC->RemoveLooseGameplayTag(Tag);
	}
}

void AFighter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEchoBladePlayerState* PS = GetPlayerState<AEchoBladePlayerState>();

	if (PS)
	{
		// Set the ASC on the Server
		AvatarASC = Cast<UEchoBladeAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		InitializeDelegates();

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
		InitializeDelegates();

		// Init ASC Actor Info for clients
		AvatarASC->InitAbilityActorInfo(PS, this);

		// Bind to AbilitySystemComponent
		bindToASC();
	}
}

void AFighter::HealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "New Health : " + FString::SanitizeFloat(HealthAS->GetHealth()));
}

void AFighter::InitializeDelegates()
{
	AvatarASC->AddSpawnedAttribute(HealthAS);
	HealthAS->SetHealth(HealthAS->GetHealth() - 50);
	// Responding to Attribute Changes(to update UI or else)
	AvatarASC->GetGameplayAttributeValueChangeDelegate(HealthAS->GetHealthAttribute()).AddUObject(this, &AFighter::HealthChanged);
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

void AFighter::SpawnProjectile(AActor* character, UMaterialInterface* Material,TSubclassOf<UCustomGameplayEffect> linkedEffectClass)
{
	AFighter* Character = Cast<AFighter>(character);
	FRotator CameraRotation;

	if (ProjectileClass && IsValid(Character))
	{
		FVector CharacterForwardVector = character->GetActorForwardVector();
		FVector SpawnLocation = character->GetActorLocation() + CharacterForwardVector * 100.f;
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
				Projectile->SphereMesh->SetMaterial(0, Material);
				Projectile->LinkedEffectClass = linkedEffectClass;
				Projectile->FireInDirection(CharacterForwardVector);
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

void AFighter::Shoot()
{
	//AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.Ranged.FireBall"));
	AbilitySystemComponent->TriggerAbility(FGameplayTag::RequestGameplayTag(FName("Ability.Offensive.Ranged.FireBall")));
}

bool AFighter::SystemHasAbility(UAbility* AbilityInstance)
{
	return AbilitySystemComponent->Abilities.Contains(AbilityInstance);
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
			AbilitySystemComponent->StopAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Defensive.Block"),false);
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
				//AddAbilityGAS(upgrade.LinkedAbility);

				//AbilitySystemComponent->AddAbility(NewObject<UAbility>(this, upgrade.LinkedAbility->GetClass()));
				AbilitySystemComponent->AddAbility(NewObject<UAbility>(this, upgrade.LinkedAbility));
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

