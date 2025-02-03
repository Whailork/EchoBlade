// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/EnemyFighter.h"

#include "EchoBladeGameInstance.h"
#include "GameplayTagsManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"

AEnemyFighter::AEnemyFighter()
{
}

void AEnemyFighter::BeginPlay()
{
	Super::BeginPlay();
	AttributeSystemComponent->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),DeathDelegate);
	DeathDelegate.BindDynamic(this,&AEnemyFighter::OnHealthChanged);
}

void AEnemyFighter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeSystemComponent->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),DeathDelegate);
	DeathDelegate.BindDynamic(this,&AEnemyFighter::OnHealthChanged);
}

void AEnemyFighter::OnHealthChanged(FGameplayTag tag, float min, float current, float max)
{
	if(current <= min)
	{
		OnDeath();
	}
}

void AEnemyFighter::Despawn()
{
	GetWorldTimerManager().ClearTimer(DespawnTimerHandle);
	DespawnTimerHandle.Invalidate();
	this->Destroy();
}

void AEnemyFighter::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Death"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(HealthWidget)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	GetWorldTimerManager().SetTimer(DespawnTimerHandle, this,&AEnemyFighter::Despawn,2,false);
	Cast<UEchoBladeGameInstance>(GetGameInstance())->CurrentPoints += 1;
	
}
