// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FlamingSwordPassive.h"

#include "GameplayTagsManager.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UFlamingSwordPassive::UFlamingSwordPassive()
{
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Passive.FlamingSword");
}

void UFlamingSwordPassive::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
	UNiagaraComponent* NiagaraComponent = instigator->GetComponentByClass<UNiagaraComponent>();
	
	FireTrail = UNiagaraFunctionLibrary::SpawnSystemAttached(FlamingParticles,NiagaraComponent,FName(""),FVector(0,0,0),FRotator(0,0,0),EAttachLocation::SnapToTarget,true);
	
}
