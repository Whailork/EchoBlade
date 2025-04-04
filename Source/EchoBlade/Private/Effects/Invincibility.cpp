// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Invincibility.h"

#include "GameplayTagsManager.h"

UInvincibility::UInvincibility()
{
	TagToAdd = UGameplayTagsManager::Get().RequestGameplayTag("Effect.Invincibility");
}
