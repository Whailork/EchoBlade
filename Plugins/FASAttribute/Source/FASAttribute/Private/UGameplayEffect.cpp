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
	GetWorld()->GetTimerManager().SetTimer(
	  effectHandle, // handle to cancel timer at a later time
	  this, // the owning object
	  &UGameplayEffect::OnEffectTriggered, // function to call on elapsed
	  Period, // float delay until elapsed
	  true); // looping?

	//le timer de duration pour trigger quand la duration de l'effet est atteinte
	FTimerHandle durationHandle;
	GetWorld()->GetTimerManager().SetTimer(
	  durationHandle,
	  this, 
	  &UGameplayEffect::StopPeriodTimer, 
	  Duration, 
	  false); 
}

void UGameplayEffect::OnEffectRemoved_Implementation(AActor* instigator)
{
	//call quand l'effet est enlevé de sur l'actor
}

void UGameplayEffect::OnEffectTriggered_Implementation()
{
	//call à chaque loop du timer de la periode
	//fait quelque chose avec l'attribute modifier
	UAttributeSystemComponent* TargetActionSystem = Cast<AActor>(GetOuter())->GetComponentByClass<UAttributeSystemComponent>();
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
			

}

void UGameplayEffect::StopPeriodTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(effectHandle);
	effectHandle.Invalidate();
	// on enlève l'effet de sur l'actor puisqu'il est terminé
	InstigatorActor->GetComponentByClass<UAttributeSystemComponent>()->RemoveEffect(TagToAdd);
}
