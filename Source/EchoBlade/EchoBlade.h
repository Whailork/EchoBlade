// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	// 0 None
	None					UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm					UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel					UMETA(DisplayName = "Cancel"),
	// 3 Fireball
	FireballAbility			UMETA(DisplayName = "Fireball"),
	// 4 Iceball
	IceballAbility			UMETA(DisplayName = "Iceball"),
	// 5 Mind Control
	MindControlAbility		UMETA(DisplayName = "Mind Control"),
	// 6 Area Slash
	AreaSlashAbility		UMETA(DisplayName = "Area Slash"),
	// 7 Electricity Attack
	ElectricityAbility		UMETA(DisplayName = "Electricity"),
	// 8 Super Armor
	SuperArmorAbility		UMETA(DisplayName = "Super Armor"),
	// 9 Auto Heal
	AutoHealAbility			UMETA(DisplayName = "Auto Heal"),
	// 10 Speed Boost
	SpeedBoostAbility		UMETA(DisplayName = "Speed Boost"),
	// 11 Life Steal
	LifeStealAbility		UMETA(DisplayName = "Life Steal"),
	// 12 Invisible
	InvisibleAbility		UMETA(DisplayName = "Invisible"),
	// 13 Clone Illusion
	CloneIllusionAbility	UMETA(DisplayName = "Clone Illusion"),
	// 14 Mana Absorption
	ManaAbsorptionAbility	UMETA(DisplayName = "Mana Absorption")
};