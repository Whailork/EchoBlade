// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystemComponent.h"
#include "Fighter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/Widget.h"
#include "EnemyFighter.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API AEnemyFighter : public AFighter
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UUserWidget* HealthWidget;
	FAttributeChangedDelegate DeathDelegate;
	FOnEffectAdded HitDelegate;
	FTimerHandle HitTimerHandlde;
	FTimerHandle DespawnTimerHandle;
	UPROPERTY(EditAnywhere)
	UBehaviorTree* TreeAsset;
	FOnActorDestroyed DestroyedDelegate;
	AEnemyFighter();
	virtual void Destroyed() override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	UFUNCTION()
	void OnHealthChanged(FGameplayTag tag,float min,float current,float max);
	void OnDeath();
	void ResetHitBoolean();
	UFUNCTION()
	void OnEnemyDestroyed(AActor* destroyedActor);
	UFUNCTION()
	void OnTakeHit(AActor* instigatorActor);
	UFUNCTION()
	void Despawn();
	
};
