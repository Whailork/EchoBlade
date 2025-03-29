// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CircleSlash.h"

#include "CustomAbilitySystem.h"
#include "GameplayTagsManager.h"
#include "Abilities/SwordAttack.h"
#include "Effects/Hit.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


UCircleSlash::UCircleSlash()
{
	ManaCost = 30;
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.Melee.CircleSlash");
	SlashRange = 300;
}

void UCircleSlash::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);
	Instigator = instigator;
	if(!SwordAttackAbility)
	{
		SwordAttackAbility = Cast<USwordAttack>(instigator->GetComponentByClass<UCustomAbilitySystem>()->GetAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack")));
		if(SwordAttackAbility)
		{
			SwordCollision = SwordAttackAbility->SwordCollision;
		}
	}
	HitActors.Add(instigator);
	UNiagaraComponent* NiagaraComponent = Instigator->GetComponentByClass<UNiagaraComponent>();
	UAttributeSystemComponent* AttributeSystemComponent = instigator->GetComponentByClass<UAttributeSystemComponent>();
	PassiveEffects = AttributeSystemComponent->GetPassiveEffects();
	Trail = UNiagaraFunctionLibrary::SpawnSystemAttached(SlashParticles,NiagaraComponent,FName(""),FVector(0,0,0),FRotator(0,0,0),EAttachLocation::SnapToTarget,true);
	
}

void UCircleSlash::Stop_Implementation(AActor* instigator, bool WasInterrupted)
{
	Super::Stop_Implementation(instigator, WasInterrupted);
	HitActors.Empty();
	if(Trail)
	{
		Trail->Deactivate();
	}
	
	
}

void UCircleSlash::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
	
}

void UCircleSlash::OnAbilityRemoved_Implementation(AActor* instigator)
{
	Super::OnAbilityRemoved_Implementation(instigator);
}

void UCircleSlash::OnAbilityStarted_Implementation(AActor* instigator)
{
	Super::OnAbilityStarted_Implementation(instigator);
}

void UCircleSlash::OnAbilityStopped_Implementation(AActor* instigator)
{
	Super::OnAbilityStopped_Implementation(instigator);
}

bool UCircleSlash::CanStartAbility_Implementation(AActor* instigator)
{
	return Super::CanStartAbility_Implementation(instigator);
}

bool UCircleSlash::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}


void UCircleSlash::StartSlashTimer(AActor* instigator)
{
	instigator->GetWorld()->GetTimerManager().SetTimer(SlashTimerHandle,this,&UCircleSlash::Slash,0.01,true);
}

void UCircleSlash::StopSlashTimer(AActor* instigator)
{
	instigator->GetWorldTimerManager().ClearTimer(SlashTimerHandle);
}

void UCircleSlash::Slash()
{
	TArray<FHitResult> outHits;
	auto channel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	UKismetSystemLibrary::SphereTraceMulti(Instigator->GetWorld(),Instigator->GetActorLocation(),Instigator->GetActorLocation() + FRotator(0,SwordCollision->GetComponentRotation().Yaw,0).RotateVector(FVector::ForwardVector)*SlashRange * -1,80,channel,false,HitActors,EDrawDebugTrace::ForDuration,outHits,true);
	for (auto Hit : outHits)
	{
		
		UHit* hitEffect = NewObject<UHit>();
				
		hitEffect->InitializeValues(0,0.00001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),ManaCost*(1),false,false);
		UAttributeSystemComponent* AttributeSystemComponent = Hit.GetActor()->GetComponentByClass<UAttributeSystemComponent>();
		if(AttributeSystemComponent)
		{
			AttributeSystemComponent->AddEffect(hitEffect);
		}
		HitActors.Add(Hit.GetActor());
	}
}
