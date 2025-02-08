// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ECHOBLADE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorTree;
	UPROPERTY(Transient)
	class UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerception;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;
	UPROPERTY()
	class UPerceptionInfo* PerceptionInfo;
	AEnemyAIController();
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void OnPerceptionForgotten(AActor* Actor);
	virtual void OnPossess(APawn* InPawn) override;
};
