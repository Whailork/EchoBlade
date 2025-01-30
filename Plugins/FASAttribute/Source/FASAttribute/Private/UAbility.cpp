#include "UAbility.h"

#include "ASActionSystemComponent.h"
#include "GameFramework/Actor.h"


UAbility::UAbility()
{

	bCanAdd = true;
	bCanStart = false;
}


void UAbility::Start_Implementation(AActor* instigator)
{
	bIsActive = true;
	OnAbilityStarted(instigator);
	
}

void UAbility::Stop_Implementation(AActor* instigator)
{
	bIsActive = false;
	OnAbilityStopped(instigator);
	
}

void UAbility::OnAbilityAdded_Implementation(AActor* instigator)
{
	bCanAdd = false;
	bCanStart = true;
}


void UAbility::OnAbilityRemoved_Implementation(AActor* instigator)
{
	bCanAdd = true;
	bCanStart = false;
}

void UAbility::OnAbilityStarted_Implementation(AActor* instigator)
{
	bCanStart = false;
}

void UAbility::OnAbilityStopped_Implementation(AActor* instigator)
{
	bCanStart = true;
}

bool UAbility::CanStartAbility_Implementation(AActor* instigator)
{
	if(bCanStart && !instigator->GetComponentByClass<UAttributeSystemComponent>()->GetEffectsTagContainer().HasAny(AbilityBlockerTags))
	{
		return true;
	}

	return false;
	
	
}

bool UAbility::CanAddAbility_Implementation(AActor* instigator)
{
	
	return bCanAdd;
}
