// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "EnemyFighter.h"
#include "Fighter.h"
#include "PerceptionInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemyAIController::AEnemyAIController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehariourTree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));

	SetPerceptionComponent(*PerceptionComponent);
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->SightRadius =10.0f;
	SightConfig->SetMaxAge(6.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 2.0f;
	SightConfig->LoseSightRadius = 12.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
		PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyAIController::OnPerceptionForgotten);
	}
	
	if(auto Fighter = Cast<AEnemyFighter>(InPawn))
	{
		if(Fighter->TreeAsset)
		{
			BlackboardComponent->InitializeBlackboard(*Fighter->TreeAsset->BlackboardAsset);
			BehaviorTree->StartTree(*Fighter->TreeAsset);
		}
	}
}


void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("perception updated"));
	if(BlackboardComponent->GetValueAsObject("FightersInSight") == nullptr)
	{
		//first fighter seen so create perception info
		UPerceptionInfo* pInfo = NewObject<UPerceptionInfo>();
		PerceptionInfo = pInfo;
		if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if(Stimulus.WasSuccessfullySensed())
			{
				if(Actor != GetPawn())
				{
					pInfo->FightersInSight.Add(Cast<AFighter>(Actor));
				}
				
			}
		}
		BlackboardComponent->SetValueAsObject("FightersInSight",pInfo);
	}
	else
	{
		if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if(Stimulus.WasSuccessfullySensed())
			{
				if(Actor != GetPawn())
				{
					if(!Cast<UPerceptionInfo>(BlackboardComponent->GetValueAsObject("FightersInSight"))->FightersInSight.Contains(Cast<AFighter>(Actor)))
					{
						Cast<UPerceptionInfo>(BlackboardComponent->GetValueAsObject("FightersInSight"))->FightersInSight.Add(Cast<AFighter>(Actor));
					}
				}
				
			}
		}
		
	}
	BlackboardComponent->SetValueAsBool("PerceptionUpdated",true);
	
}

void AEnemyAIController::OnPerceptionForgotten(AActor* Actor)
{
	if(Actor != GetPawn())
	{
		Cast<UPerceptionInfo>(BlackboardComponent->GetValueAsObject("FightersInSight"))->FightersInSight.Remove(Cast<AFighter>(Actor));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "Fighterforgotten");
	
			
		
	}
	
}
