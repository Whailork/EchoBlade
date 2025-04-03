// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystemComponent.h"

#include "CustomGameplayEffect.h"
#include "EchoBladeGameInstance.h"
#include "EchoBladeGameInstance.h"
#include "FASAttribute.h"
#include "GameplayTagsManager.h"

//#include "UGameplayEffect.h"


int FAttributeChangedHolder::currentHandle = 0;
int FAttributeAddedHolder::currentHandle = 0;
int FAttributeRemovedHolder::currentHandle = 0;
int FEffectRemovedHolder::currentHandle = 0;
int FEffectAddedHolder::currentHandle = 0;

FGameplayTagContainer UAttributeSystemComponent::GetEffectsTagContainer()
{
	FGameplayTagContainer EffectsTagContainer;
	for (auto Effect : EffectsContainer)
	{
		EffectsTagContainer.AddTag(Effect->TagToAdd);
	}
	return EffectsTagContainer;
}

void UAttributeSystemComponent::ClearAllEffects()
{
	if(!EffectsContainer.IsEmpty())
	{
		for (auto Effect : EffectsContainer)
		{
			//on cancel les timers et on destroy l'object
			GetOwner()->GetWorldTimerManager().ClearAllTimersForObject(Effect);
			//fire la delegate
			if(mapEffectRemoved.Contains(Effect->TagToAdd))
			{
				for (auto Element : mapEffectRemoved[Effect->TagToAdd])
				{
					Element.effectRemovedDelegate.ExecuteIfBound(GetOwner());
				}
				
			}
		}
		EffectsContainer.Empty();
	}
	
}

bool UAttributeSystemComponent::HasEffect(FGameplayTag effectTag)
{
	for (auto Effect : EffectsContainer)
	{
		if(Effect->TagToAdd.MatchesTag(effectTag))
		{
			return true;
		}
	}
	return false;
}

TArray<UCustomGameplayEffect*> UAttributeSystemComponent::GetPassiveEffects()
{
	TArray<UCustomGameplayEffect*> PassiveEffects;
	for (auto Effect : EffectsContainer)
	{
		if(Effect->TagToAdd.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Effect.Passive")))
		{
			PassiveEffects.Add(Effect);
		}
	}
	return PassiveEffects;
}


// Sets default values for this component's properties
UAttributeSystemComponent::UAttributeSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAttributeSystemComponent::LoadDefaultAttributes()
{
	if(DefaultAttributes)
	{
		for(FASAttributeData AttributeData : DefaultAttributes->AttributeData)
		{
			FAttribute newAttribute = {AttributeData.attributeTag, AttributeData.min, AttributeData.current, AttributeData.max};
			AddAttribute(newAttribute);
		}
	}
}

bool UAttributeSystemComponent::SetAttributeValue(FGameplayTag tag, float newValue)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi

	// Liste des attributs == vide
	if(Attributes.IsEmpty())
	{
		// Attributs par défaut.
		LoadDefaultAttributes();
	}
	// Parcourt liste pour trouver tag
	for (int i = 0; i < Attributes.Num();i++)
	{
		// Attribut avec tag trouvé
		if(Attributes[i].attributeTag == tag)
		{
			if(newValue < Attributes[i].min)
			{
				// set la valeur à min
				Attributes[i].current = Attributes[i].min;
			}
			else
			{
				if(newValue > Attributes[i].max)
				{
					//set la valeur à max
					Attributes[i].current = Attributes[i].max;
				}
				else
				{
					// Met à jour la valeur actuelle de l'attribut avec la nouvelle valeur
					Attributes[i].current = newValue;
				}
			}
			

			// Notifie les delegates que la valeur a changé.
			for (auto attribute : arrChangedDelegates)
			{
				// Exécute delegate en passant les informations de l'attribut
				if(attribute.changedDelegate.ExecuteIfBound(tag, Attributes[i].min, Attributes[i].current, Attributes[i].max))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("execute delegate"));
				}
			}
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::GetAttributeValue(FGameplayTag tag, float& outValue)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	for (int i = 0; i < Attributes.Num();i++)
	{
		if(Attributes[i].attributeTag == tag)
		{
			outValue = Attributes[i].current;
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::SetAttributeMaxValue(FGameplayTag tag, float newValue)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	for (int i = 0; i < Attributes.Num();i++)
	{
		if(Attributes[i].attributeTag == tag)
		{
			Attributes[i].max = newValue;
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::GetAttributeMaxValue(FGameplayTag tag, float& outValue)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	
	for (int i = 0; i < Attributes.Num(); i++)
	{

		if (Attributes[i].attributeTag == tag)
		{
			outValue = Attributes[i].max;
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::SetAttributeMinValue(FGameplayTag tag, float newValue)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	for (int i = 0; i < Attributes.Num();i++)
	{
		if(Attributes[i].attributeTag == tag)
		{
			Attributes[i].min = newValue;
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::GetAttributeMinValue(FGameplayTag tag, float& outValue)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	for (int i = 0; i < Attributes.Num();i++)
	{
		if(Attributes[i].attributeTag == tag)
		{
			outValue = Attributes[i].min;
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::HasAttribute(FGameplayTag tag)
{
	//on dirait que les widgets s'initialisent avant le begin play de l'action system donc on get les valeurs par defaut ici aussi
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	for (int i = 0; i < Attributes.Num();i++)
	{
		if(Attributes[i].attributeTag == tag)
		{
			return true;
		}
	}
	return false;
}

bool UAttributeSystemComponent::AddAttribute(const FAttribute& newAttribute)
{
	if(Attributes.IsEmpty())
	{
		Attributes.Add(newAttribute);
		return true;
	}
	else
	{
		if(!HasAttribute(newAttribute.attributeTag))
		{
			Attributes.Add(newAttribute);
			return true;
		}
	}
	
	return false;
	
}

// todo enlever les delegates de la liste quand on enlève l'attribute
bool UAttributeSystemComponent::RemoveAttribute(FGameplayTag tag)
{
	int tagIndex = -1;
	for (int i = 0; i < Attributes.Num();i++)
	{
		if(Attributes[i].attributeTag == tag)
		{
			tagIndex = i;
		}
	}
	if(tagIndex != -1)
	{
		Attributes.RemoveAt(tagIndex);
		return true;
	}
	return false;
}

int UAttributeSystemComponent::AddAttributeChangedDelegate(FGameplayTag attributeTag,FAttributeChangedDelegate changedDelegate)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("Tag is : ") + attributeTag.ToString());
	// Crée un conteneur pour le délégué et son identifiant unique (handle) {Incrémente le handle unique pour l'associer à ce délégué, Associe le délégué passé en paramètre.
	FAttributeChangedHolder Holder = {FAttributeChangedHolder::currentHandle++,changedDelegate};

	// Ajoute le conteneur à la liste des delegates surveillant les changements d'attributs.
	arrChangedDelegates.Add(Holder);
	return Holder.i;
}

void UAttributeSystemComponent::RemoveAttributeChangedDelegate(int i)
{
	int x;
	for(x = 0; x <  arrChangedDelegates.Num();x++)
	{
		if(arrChangedDelegates[x].i == i)
		{
			break;
		}
	}
	arrChangedDelegates.RemoveAt(x);
}

int UAttributeSystemComponent::AddAttributeAddedDelegate(FAttributeAddedDelegate addedDelegate)
{
	FAttributeAddedHolder Holder = {FAttributeAddedHolder::currentHandle++,addedDelegate};
	arrAddedDelegates.Add(Holder);
	return Holder.i;
}

void UAttributeSystemComponent::RemoveAttributeAddedDelegate(int i)
{
	int x;
	for(x = 0; x< arrAddedDelegates.Num();x++)
	{
		if(arrAddedDelegates[x].i == i)
		{
			break;
		}
	}
	arrAddedDelegates.RemoveAt(x);
}

int UAttributeSystemComponent::AddAttributeRemovedDelegate(FAttributeRemovedDelegate removedDelegate)
{
	FAttributeRemovedHolder Holder = {FAttributeRemovedHolder::currentHandle++,removedDelegate};
	arrRemovedDelegates.Add(Holder);
	return Holder.i;
}

void UAttributeSystemComponent::RemoveAttributeRemovedDelegate(int i)
{
	int x;
	for(x = 0; x < arrRemovedDelegates.Num();x++)
	{
		if(arrRemovedDelegates[x].i == i )
		{
			break;
		}
	}
	arrRemovedDelegates.RemoveAt(x);
}

void UAttributeSystemComponent::FillUpAttributes()
{
	for (auto Attribute : Attributes)
	{
		SetAttributeValue(Attribute.attributeTag,Attribute.max);
	}
}

bool UAttributeSystemComponent::AddEffect(UCustomGameplayEffect* effect)
{
	if(!GetEffectsTagContainer().HasAny(effect->BlockingTags))
	{
		//on valide s'il peut être lancé
		EffectsContainer.Add(effect);
		//fire la delegate de l'effet
		effect->addedDelegate.ExecuteIfBound(GetOwner());
		//fire les delegates de l'attributeSystemComponent
		if(mapEffectAdded.Contains(effect->TagToAdd))
		{
			for (FEffectAddedHolder delegateHolder : mapEffectAdded[effect->TagToAdd])
			{
				delegateHolder.effectAddeddDelegate.ExecuteIfBound(GetOwner());
			}
		}
		//bind la delegate d'effect removed
		if(mapEffectRemoved.Contains(effect->TagToAdd))
		{
			mapEffectRemoved[effect->TagToAdd].Add(FEffectRemovedHolder(FEffectRemovedHolder::currentHandle++,effect->removedDelegate));
		}
		else
		{
			TArray<FEffectRemovedHolder> newArray = {FEffectRemovedHolder(FEffectRemovedHolder::currentHandle++,effect->removedDelegate)};
			mapEffectRemoved.Add(effect->TagToAdd,newArray);
		}
		return true;
	}
	return false;
}

void UAttributeSystemComponent::RemoveEffect(FGameplayTag effectTag)
{
	UCustomGameplayEffect* EffectToRemove = nullptr;
	for (auto Effect : EffectsContainer)
	{
		if(Effect->TagToAdd.MatchesTagExact(effectTag))
		{
			EffectToRemove = Effect;
		}
		
	}
	if(EffectToRemove != nullptr)
	{
		EffectsContainer.Remove(EffectToRemove);
		//on cancel les timers et on destroy l'object
		GetOwner()->GetWorldTimerManager().ClearAllTimersForObject(EffectToRemove);
		//fire la delegate
		if(mapEffectRemoved.Contains(effectTag))
		{
			for (auto Element : mapEffectRemoved[effectTag])
			{
				Element.effectRemovedDelegate.ExecuteIfBound(GetOwner());
			}
			
		}
	}
	
	
	
}
void UAttributeSystemComponent::AddEffectAddedDelegate(FGameplayTag effectTag, FOnEffectAdded addedDelegate)
{
	FEffectAddedHolder holder = {FEffectAddedHolder::currentHandle++,addedDelegate};
	if(mapEffectAdded.Contains(effectTag))
	{
		mapEffectAdded[effectTag].Add(holder);
	}
	else
	{
		TArray<FEffectAddedHolder> newArray;
		newArray.Add(holder);
		mapEffectAdded.Add(effectTag,newArray);
	}
}

void UAttributeSystemComponent::AddEffectRemovedDelegate(FGameplayTag effectTag, FOnEffectRemoved removedDelegate)
{
	FEffectRemovedHolder holder = {FEffectRemovedHolder::currentHandle++,removedDelegate};
	if(mapEffectRemoved.Contains(effectTag))
	{
		mapEffectRemoved[effectTag].Add(holder);
	}
	else
	{
		TArray<FEffectRemovedHolder> newArray;
		newArray.Add(holder);
		mapEffectRemoved.Add(effectTag,newArray);
	}
}


// Called when the game starts
void UAttributeSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	//On assigne les valeurs par défaut
	if(Attributes.IsEmpty())
	{
		LoadDefaultAttributes();
	}
	
}


// Called every frame
void UAttributeSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

