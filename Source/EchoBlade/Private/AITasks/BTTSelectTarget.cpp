// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTSelectTarget.h"

#include "EnemyAIController.h"
#include "EnemyFighter.h"
#include "Fighter.h"
#include "PerceptionInfo.h"
#include "PlayerFighter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTSelectTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool TargetSet = false;
	float ClosestDistance = -1;
	AFighter* ClosestNonPlayer = nullptr;
	TArray<APlayerFighter*> PlayersInSight;
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyFighter* EnemyFighter = Cast<AEnemyFighter>(AIController->GetCharacter());
	UPerceptionInfo* PerceptionInfo = Cast<UPerceptionInfo>(AIController->GetBlackboardComponent()->GetValueAsObject("FightersInSight"));
	if(!PerceptionInfo)
	{
		return EBTNodeResult::Failed;
	}
	if(PerceptionInfo->FightersInSight.IsEmpty())
	{
		return EBTNodeResult::Failed;
	}
	TArray<TSoftObjectPtr<AFighter>> FightersToRemove;
	// loop to find players and closest non player
	for (auto Fighter : PerceptionInfo->FightersInSight)
	{
		if(Fighter.IsValid())
		{
			if(auto playerFighter = Cast<APlayerFighter>(Fighter.Get()))
			{
				PlayersInSight.Add(playerFighter);
			}
			else
			{
				float DistanceWithCurrentFighter = UKismetMathLibrary::Vector_Distance(EnemyFighter->GetActorLocation(),Fighter->GetActorLocation());
				if(ClosestDistance == -1)
				{
					ClosestDistance = DistanceWithCurrentFighter;
					ClosestNonPlayer = Fighter.Get();
				}
				else
				{
					if(ClosestDistance > DistanceWithCurrentFighter)
					{
						ClosestDistance = DistanceWithCurrentFighter;
						ClosestNonPlayer = Fighter.Get();
					}
				}
			}
		}
		else
		{
			FightersToRemove.Add(Fighter);
		}
	}
	for (auto ToRemove : FightersToRemove)
	{
		PerceptionInfo->FightersInSight.Remove(ToRemove);
	}
	// if no player, target closest non player
	if(PlayersInSight.IsEmpty())
	{
		if(ClosestNonPlayer)
		{
			AIController->GetBlackboardComponent()->SetValueAsObject("Target",ClosestNonPlayer);
			TargetSet = true;
		}
	}
	else
	{
		APlayerFighter* ClosestPlayer = nullptr;
		ClosestDistance = -1;
		for (auto InSight : PlayersInSight)
		{
			float DistanceWithCurrentPlayer = UKismetMathLibrary::Vector_Distance(EnemyFighter->GetActorLocation(),InSight->GetActorLocation());
			if(ClosestDistance == -1)
			{
				ClosestDistance = DistanceWithCurrentPlayer;
				ClosestPlayer = InSight;
			}
			else
			{
				if(ClosestDistance > DistanceWithCurrentPlayer)
				{
					ClosestDistance = DistanceWithCurrentPlayer;
					ClosestPlayer = InSight;
				}
			}
			AIController->GetBlackboardComponent()->SetValueAsObject("Target",ClosestPlayer);
			TargetSet = true;
		}
		
	}
	AIController->GetBlackboardComponent()->SetValueAsBool("PerceptionUpdated",TargetSet);
	if(TargetSet)
	{
		//Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
		if(auto target= Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target")))
		{
			AIController->SetFocus(Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target")));
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
		
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
