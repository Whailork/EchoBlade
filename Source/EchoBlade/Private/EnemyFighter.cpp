// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/EnemyFighter.h"

#include "EchoBladeGameInstance.h"
#include "EnemyAIController.h"
#include "GameplayTagsManager.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	HitDelegate.BindDynamic(this,&AEnemyFighter::OnTakeHit);
	AttributeSystemComponent->AddEffectAddedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit"),HitDelegate);

}

void AEnemyFighter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeSystemComponent->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),DeathDelegate);
	DeathDelegate.BindDynamic(this,&AEnemyFighter::OnHealthChanged);
}

void AEnemyFighter::OnHealthChanged(FGameplayTag tag, float min, float current, float max)
{
	if(tag.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health")))
	{
		if(current <= min)
		{
			OnDeath();
		}
		//check for low health (under 30 percents)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat((current - min)/ (max - min)));
		if((current - min)/ (max - min) < 0.3)
		{
			Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("LowHealth",true);
		}
		else
		{
			Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("LowHealth",false);
		}
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Death"));
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

void AEnemyFighter::ResetHitBoolean()
{
	Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("GotHit",false);
	GetWorldTimerManager().ClearTimer(HitTimerHandlde);
	HitTimerHandlde.Invalidate();
}

void AEnemyFighter::OnTakeHit(AActor* instigatorActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "GotHit");
	Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("GotHit",true);
	GetWorldTimerManager().SetTimer(HitTimerHandlde, this,&AEnemyFighter::ResetHitBoolean,2,false);
}

