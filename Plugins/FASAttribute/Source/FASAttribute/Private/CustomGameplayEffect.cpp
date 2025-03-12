// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameplayEffect.h"

UCustomGameplayEffect::UCustomGameplayEffect()
{
	addedDelegate = FOnEffectAdded();
	removedDelegate = FOnEffectRemoved();
	addedDelegate.BindDynamic(this, &UCustomGameplayEffect::OnEffectAdded);
	removedDelegate.BindDynamic(this, &UCustomGameplayEffect::OnEffectRemoved);
}

void UCustomGameplayEffect::BeginDestroy()
{
	UObject::BeginDestroy();
	if(InstgatorAttributeComponent)
	{
		// on enlève l'effet de sur l'actor puisqu'il est terminé
		InstgatorAttributeComponent->RemoveEffect(TagToAdd);
	}
}

void UCustomGameplayEffect::DestroyEffect(AActor* instigator)
{
	tm->ClearAllTimersForObject(this);
}

void UCustomGameplayEffect::StopPeriodTimer()
{
	if(effectHandle.IsValid())
	{
		
		if(world)
		{
			if(tm){
				if(effectHandle.IsValid())
				{
					tm->ClearTimer(effectHandle);
					effectHandle.Invalidate();
				}
			}
		}
		
	}
	if(InstgatorAttributeComponent.IsValid())
	{
		// on enlève l'effet de sur l'actor puisqu'il est terminé
		InstgatorAttributeComponent->RemoveEffect(TagToAdd);
	}
}

void UCustomGameplayEffect::InitializeValues_Implementation(float inDuration, float inPeriod,FGameplayTag AffectedAttributeTag, float effectPower, bool inLooping, bool inStoppedByEvent)
{
	Duration = inDuration;
	Period = inPeriod;
	bLooping = inLooping;
	bStoppedByEvent = inStoppedByEvent;
}

void UCustomGameplayEffect::OnEffectTriggered_Implementation()
{
	//call à chaque loop du timer de la periode
	//fait quelque chose avec l'attribute modifier
	
	float attributeValue;
	float MaxValue;
	float MinValue;
	InstgatorAttributeComponent->GetAttributeValue(AttributeModifiers.TargetAttribute,attributeValue);
	InstgatorAttributeComponent->GetAttributeMaxValue(AttributeModifiers.TargetAttribute,MaxValue);
	InstgatorAttributeComponent->GetAttributeMinValue(AttributeModifiers.TargetAttribute,MinValue);
	switch (AttributeModifiers.Operation)
	{
	case EModifierOperation::Add :

		if(attributeValue != MaxValue)
		{
			InstgatorAttributeComponent->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue + AttributeModifiers.Value);
		}
		
		break;
	case EModifierOperation::Subtract :
		if(attributeValue != MinValue)
		{
			InstgatorAttributeComponent->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue - AttributeModifiers.Value);
		}
		
		break;
	case EModifierOperation::Multiply :
		if(attributeValue != MaxValue)
		{
			InstgatorAttributeComponent->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue + AttributeModifiers.Value);
		}
		
		InstgatorAttributeComponent->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue * AttributeModifiers.Value);
		break;
	case EModifierOperation::Divide :
		if(attributeValue != MinValue)
		{
			InstgatorAttributeComponent->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue - AttributeModifiers.Value);
		}
		InstgatorAttributeComponent->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue / AttributeModifiers.Value);
		break;
	}
	InstgatorAttributeComponent->GetAttributeValue(AttributeModifiers.TargetAttribute,attributeValue);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,FString::SanitizeFloat(attributeValue) );

	if(!bLooping && !bStoppedByEvent)
	{
		StopPeriodTimer();
	}
}

void UCustomGameplayEffect::OnEffectRemoved_Implementation(AActor* instigator)
{
	//call quand l'effet est enlevé de sur l'actor
}

void UCustomGameplayEffect::OnEffectAdded_Implementation(AActor* instigator)
{
	InstigatorActor = instigator;
	InstigatorActor->OnDestroyed.AddDynamic(this,&UCustomGameplayEffect::DestroyEffect);
	world = instigator->GetWorld();
	tm = &world->GetTimerManager();
	InstgatorAttributeComponent = instigator->GetComponentByClass<UAttributeSystemComponent>();
	// le timer pour le période
	world->GetTimerManager().SetTimer(
	  effectHandle, // handle to cancel timer at a later time
	  this, // the owning object
	  &UCustomGameplayEffect::OnEffectTriggered, // function to call on elapsed
	  Period, // float delay until elapsed
	  bLooping); // looping?

	if(!bStoppedByEvent && bLooping)
	{
		//le timer de duration pour trigger quand la duration de l'effet est atteinte
		FTimerHandle durationHandle;
		world->GetTimerManager().SetTimer(
		  durationHandle,
		  this, 
		  &UCustomGameplayEffect::StopPeriodTimer, 
		  Duration, 
		  false); 
	}
}
