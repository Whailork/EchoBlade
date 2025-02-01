// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameplayEffect.h"

UGameplayEffect::UGameplayEffect()
{
	addedDelegate = FOnEffectAdded();
	removedDelegate = FOnEffectRemoved();
	addedDelegate.BindDynamic(this, &UGameplayEffect::OnEffectAdded);
	removedDelegate.BindDynamic(this, &UGameplayEffect::UGameplayEffect::OnEffectRemoved);
}

void UGameplayEffect::OnEffectAdded_Implementation(AActor* instigator)
{
	InstigatorActor = instigator;
	// le timer pour le période
	instigator->GetWorld()->GetTimerManager().SetTimer(
	  effectHandle, // handle to cancel timer at a later time
	  this, // the owning object
	  &UGameplayEffect::OnEffectTriggered, // function to call on elapsed
	  Period, // float delay until elapsed
	  bLooping); // looping?

	if(!bStoppedByEvent && bLooping)
	{
		//le timer de duration pour trigger quand la duration de l'effet est atteinte
		FTimerHandle durationHandle;
		instigator->GetWorld()->GetTimerManager().SetTimer(
		  durationHandle,
		  this, 
		  &UGameplayEffect::StopPeriodTimer, 
		  Duration, 
		  false); 
	}
	
}

void UGameplayEffect::OnEffectRemoved_Implementation(AActor* instigator)
{
	//call quand l'effet est enlevé de sur l'actor
}

void UGameplayEffect::OnEffectTriggered_Implementation()
{
	//call à chaque loop du timer de la periode
	//fait quelque chose avec l'attribute modifier
	UAttributeSystemComponent* TargetActionSystem = InstigatorActor->GetComponentByClass<UAttributeSystemComponent>();
	float attributeValue;
	
	TargetActionSystem->GetAttributeValue(AttributeModifiers.TargetAttribute,attributeValue);
	switch (AttributeModifiers.Operation)
	{
	case EModifierOperation::Add :
		TargetActionSystem->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue + AttributeModifiers.Value);
		break;
	case EModifierOperation::Subtract :
		TargetActionSystem->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue - AttributeModifiers.Value);
		break;
	case EModifierOperation::Multiply :
		TargetActionSystem->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue * AttributeModifiers.Value);
		break;
	case EModifierOperation::Divide :
		TargetActionSystem->SetAttributeValue(AttributeModifiers.TargetAttribute, +attributeValue / AttributeModifiers.Value);
		break;
	}
	TargetActionSystem->GetAttributeValue(AttributeModifiers.TargetAttribute,attributeValue);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,FString::SanitizeFloat(attributeValue) );

	if(!bLooping && !bStoppedByEvent)
	{
		StopPeriodTimer();
	}
			

}

void UGameplayEffect::InitializeValues_Implementation(float inDuration, float inPeriod,FGameplayTag AffectedAttributeTag,float effectPower,bool inLooping, bool inStoppedByEvent)
{
	Duration = inDuration;
	Period = inPeriod;
	bLooping = inLooping;
	bStoppedByEvent = inStoppedByEvent;
}

void UGameplayEffect::StopPeriodTimer()
{
	if(effectHandle.IsValid())
	{
		InstigatorActor->GetWorld()->GetTimerManager().ClearTimer(effectHandle);
		effectHandle.Invalidate();
	}
	// on enlève l'effet de sur l'actor puisqu'il est terminé
	InstigatorActor->GetComponentByClass<UAttributeSystemComponent>()->RemoveEffect(TagToAdd);
}
