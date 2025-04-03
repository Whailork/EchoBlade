// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoBlade/Public/EnemyFighter.h"

#include "CustomAbilitySystem.h"
#include "EchoBladeGameInstance.h"
#include "EnemyAIController.h"
#include "EnemyWaveSubsystem.h"
#include "GameplayTagsManager.h"
#include "Abilities/SecondWindPassive.h"
#include "Abilities/SwordAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"

AEnemyFighter::AEnemyFighter()
{
}

void AEnemyFighter::Destroyed()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::Destroyed();
}

void AEnemyFighter::BeginPlay()
{
	Super::BeginPlay();
	AttributeSystemComponent->AddAttributeChangedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),DeathDelegate);
	DeathDelegate.BindDynamic(this,&AEnemyFighter::OnHealthChanged);
	HitDelegate.BindDynamic(this,&AEnemyFighter::OnTakeHit);
	AttributeSystemComponent->AddEffectAddedDelegate(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Hit"),HitDelegate);
	OnDestroyed.AddDynamic(this,&AEnemyFighter::OnEnemyDestroyed);

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
			
			//apply second wind if the fighter has the ability
			if(AbilitySystemComponent->TriggerAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Passive.SecondWind")))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SecondWind"));
				
			}
			else
			{
				//else die
			
				Cast<AEnemyAIController>(GetController())->BehaviorTree->StopTree(EBTStopMode::Forced);
				OnDeath();
			}

			
			
		}
		//check for low health (under 30 percents)
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat((current - min)/ (max - min)));
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


void AEnemyFighter::OnDeath()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Death"));
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(HealthWidget)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	Cast<AEnemyAIController>(GetController())->BehaviorTree->StopTree(EBTStopMode::Forced);
	UAbility* AinUse = AbilitySystemComponent->AbilityInUse();
	if(AinUse)
	{
		AbilitySystemComponent->StopAbility(AinUse->AbilityTag,true);
	}
	PlayAnimMontage(DeathAnimMontage);
	//StopAnimMontage();
	GetWorldTimerManager().SetTimer(DespawnTimerHandle, this,&AEnemyFighter::Despawn,2,false);
	
	
	
}

void AEnemyFighter::ResetHitBoolean()
{
	Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("GotHit",false);
	GetWorldTimerManager().ClearTimer(HitTimerHandlde);
	HitTimerHandlde.Invalidate();
}

void AEnemyFighter::OnEnemyDestroyed(AActor* destroyedActor)
{
	GetWorld()->GetSubsystem<UEnemyWaveSubsystem>()->OnFighterDefeated();
}

void AEnemyFighter::OnTakeHit(AActor* instigatorActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "GotHit");
	Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("GotHit",true);
	Cast<AEnemyAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("AttackComboStarted",false);
	UAbility* SwordAttackAbility = AbilitySystemComponent->GetAbility(UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack"));

	if(SwordAttackAbility)
	{
		Cast<USwordAttack>(SwordAttackAbility)->AttackCount = 0;
	}
	GetWorldTimerManager().SetTimer(HitTimerHandlde, this,&AEnemyFighter::ResetHitBoolean,2,false);
}

void AEnemyFighter::Despawn()
{
	Destroy();
}

