// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/EchoBladePlayerState.h"

AEchoBladePlayerState::AEchoBladePlayerState()
{
	// Create ability system component, and set it to be explicitly replicated	
	AbilitySystemComponent = CreateDefaultSubobject<UEchoBladeAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	HealthAS = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet")); //HealthAS = NewObject<UHealthAttributeSet>();
	AbilitySystemComponent->AddSpawnedAttribute(HealthAS);

	// Responding to Changes(add or remove) in Gameplay Tags
	/*
	AbilitySystemComponent->RegisterGameplayTagEvent(Gameplay_Attribute_Health, EGameplayTagEventType::NewOrRemoved).
							AddUObject(this, &AEchoBladePlayerState::HealthTagAddedorRemoved);
							*/
	// Responding to Attribute Changes(to update UI or else)
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAS->GetHealthAttribute()).AddUObject(this, &AEchoBladePlayerState::HealthChanged);
}

void AEchoBladePlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Initialisation ( TODO: Move to player??? )
	HealthAS->InitHealth(Health);
	HealthAS->InitMaxHealth(MaxHealth);
	HealthAS->InitMana(Mana);
	HealthAS->InitMaxMana(MaxMana);
	HealthAS->InitShield(shield);
	HealthAS->InitMaxShield(maxShield);
}

// Tag change callbacks
void AEchoBladePlayerState::HealthTagAddedorRemoved(FGameplayTag GameplayTag, int tagCount)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Health tag added or removed"));
}

// Attribute changed callbacks
void AEchoBladePlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Health changed: %f -> %f"), Data.OldValue, Data.NewValue));
}

UHealthAttributeSet* AEchoBladePlayerState::GetHealthAttributeSet() const
{
	return HealthAS;
}

void AEchoBladePlayerState::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

UEchoBladeAbilitySystemComponent* AEchoBladePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}