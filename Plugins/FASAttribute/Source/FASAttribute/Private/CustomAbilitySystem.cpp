// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\CustomAbilitySystem.h"

#include "Engine/Engine.h"


int FAbilityAddedHolder::currentHandle = 0;
int FAbilityRemovedHolder::currentHandle = 0;
// Sets default values for this component's properties
UCustomAbilitySystem::UCustomAbilitySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCustomAbilitySystem::AddAbility(UAbility* NewAbility)
{
	
	if (!Abilities.Contains(NewAbility) && NewAbility->CanAddAbility(this->GetOwner()))
	{
		Abilities.Add(NewAbility);
		NewAbility->OnAbilityAdded(this->GetOwner());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Added ability \"") + NewAbility->AbilityTag.ToString() + "\"");
	}
}

void UCustomAbilitySystem::RemoveAbility(FGameplayTag tag)
{
	int removeIndex = -1;
	for (int i = 0; i < Abilities.Num(); i++)
	{
		if(Abilities[i]->AbilityTag == tag)
		{
			removeIndex = i;
			break;
		}
		
	}
	if(removeIndex != -1)
	{
		Abilities[removeIndex]->OnAbilityRemoved(this->GetOwner());
		Abilities.RemoveAt(removeIndex);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Removed ability \"") + tag.ToString() + "\"");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ability not found : ") + tag.ToString() + "\"");

	}
}

void UCustomAbilitySystem::RemoveAllAbilities()
{
	if (!Abilities.IsEmpty())
	{
		Abilities.Empty();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Removed all abilities"));
	}
}

bool UCustomAbilitySystem::TriggerAbility(FGameplayTag tag)
{
	
	for (auto ability : Abilities)
	{
		if(ability->AbilityTag == tag)
		{
			if(ability->CanStartAbility(this->GetOwner()))
			{
				UAbility* returnAbility = AbilityInUse();
				if(returnAbility != nullptr)
				{
					if(returnAbility->bCanInterrupt)
					{
						returnAbility->Stop(GetOwner());
						ability->Start(this->GetOwner());
						return true;
					}
					return false;
				}
				ability->Start(this->GetOwner());
				return true;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Started ability \"") + tag.ToString() + "\"");
			}
			return false;
			
		}
	}
	return false;
}

bool UCustomAbilitySystem::HasAbility(FGameplayTag AbilityTag)
{
	for (auto Ability : Abilities)
	{
		if(Ability->AbilityTag == AbilityTag)
		{
			return true;
		}
	}
	return false;
}

UAbility* UCustomAbilitySystem::GetAbility(FGameplayTag AbilityTag)
{
	for (auto Ability : Abilities)
	{
		if(Ability->AbilityTag == AbilityTag)
		{
			return Ability;
		}
	}
	return nullptr;
}

void UCustomAbilitySystem::StopAbility(FGameplayTag AbilityTag)
{
	GetAbility(AbilityTag)->Stop(this->GetOwner());
}

UAbility* UCustomAbilitySystem::AbilityInUse()
{
	for (UAbility* Ability : Abilities)
	{
		if(Ability->bIsActive)
		{
			return Ability;
		}
	}
	return nullptr;
}

TArray<UAbility*> UCustomAbilitySystem::GetAllAvailableAbilitiesWithTag(FGameplayTag TagToMatch, AActor* Instigator)
{
	TArray<UAbility*> MatchingAbilities;
	for (UAbility* Ability : Abilities)
	{
		if(Ability->CanStartAbility(Instigator) && Ability->AbilityTag.MatchesTag(TagToMatch))
		{
			MatchingAbilities.Add(Ability);
		}
	}
	return MatchingAbilities;
}

int UCustomAbilitySystem::AddAbilityAddedDelegate(FOnAbilityAdded addedDelegate)
{
	
	FAbilityAddedHolder Holder{FAbilityAddedHolder::currentHandle++,addedDelegate};
	arrAbilityAddedDelegates.Add(Holder);
	return Holder.i;
}

void UCustomAbilitySystem::RemoveAbilityAddedDelegate(int i)
{
	int x;
	for(x = 0; x < arrAbilityAddedDelegates.Num();x++)
	{
		if(arrAbilityAddedDelegates[x].i == i )
		{
			break;
		}
	}
	arrAbilityAddedDelegates.RemoveAt(x);
}

int UCustomAbilitySystem::AddAbilityRemovedDelegate(FOnAbilityRemoved removedDelegate)
{
	FAbilityRemovedHolder Holder{FAbilityRemovedHolder::currentHandle++,removedDelegate};
	arrAbilityRemovedDelegates.Add(Holder);
	return Holder.i;
}

void UCustomAbilitySystem::RemoveAbilityRemovedDelegate(int i)
{
	int x;
	for(x = 0; x < arrAbilityRemovedDelegates.Num();x++)
	{
		if(arrAbilityRemovedDelegates[x].i == i)
		{
			break;
		}
	}
	arrAbilityRemovedDelegates.RemoveAt(x);
}


// Called when the game starts
void UCustomAbilitySystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCustomAbilitySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

