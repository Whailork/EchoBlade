// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/SwordAttack.h"
#include "GameplayTagsManager.h"
#include "Effects/Hit.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


USwordAttack::USwordAttack()
{
	
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Ability.Offensive.SwordAttack");
	AttackCount = 0;
	isAttacking = false;
	bCanInterrupt = false;
	bHasHit = false;
	bInterruptOnHit = true;
	
}

void USwordAttack::StartSwordCollision()
{
	
	SwordCollision->Activate();
	SwordCollision->SetGenerateOverlapEvents(true);
}

void USwordAttack::StopSwordCollision()
{
	SwordCollision->Deactivate();
	SwordCollision->SetGenerateOverlapEvents(false);
	bHasHit = false;
}

void USwordAttack::SetupSwordCollision(UCapsuleComponent* Sword,AActor* instigator)
{
	UAttributeSystemComponent* AttributeSystemComponent = instigator->GetComponentByClass<UAttributeSystemComponent>();
	if(AttributeSystemComponent->HasAttribute(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.AttackRange")))
	{
		float AttackRange = -1;
		AttributeSystemComponent->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.AttackRange"),AttackRange);
		if(AttackRange != -1)
		{
			float baseHalfSize = Sword->GetRelativeScale3D().Z;
			
			Sword->SetRelativeScale3D(FVector(Sword->GetRelativeScale3D().X,Sword->GetRelativeScale3D().Y,AttackRange));
			//8 et 5 parce que jai teste et un ajout de 5 en capsule size necesite un decalement de 8 dans la position x
			//Sword->SetWorldLocation(FVector(Sword->GetComponentLocation().X +20,Sword->GetComponentLocation().Y,Sword->GetComponentLocation().Z));
			//Sword->AddRelativeLocation(FVector((AttackRange - baseHalfSize)* 13 / 0.5,0,0));
			DrawDebugCapsule(GetWorld(),Sword->GetComponentLocation(),Sword->GetScaledCapsuleHalfHeight(),Sword->GetScaledCapsuleRadius(),Sword->GetComponentRotation().Quaternion(),FColor::Blue,true,5,0,3);
		}
	}
	this->SwordCollision = Sword;
	this->SwordCollision->OnComponentBeginOverlap.AddDynamic(this,&USwordAttack::OverlapBegin);
	SwordCollision->SetAutoActivate(false);
	SwordCollision->Deactivate();
	SwordCollision->SetGenerateOverlapEvents(false);
}


void USwordAttack::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if(!bHasHit)
	//{
		if(OtherActor != SwordCollision->GetOwner())
		{
			UAttributeSystemComponent* HitAttributeComponent = OtherActor->GetComponentByClass<UAttributeSystemComponent>();
			if(HitAttributeComponent != nullptr)
			{
				bHasHit = true;
				UHit* hitEffect = NewObject<UHit>();
				
				hitEffect->InitializeValues(0,0.00001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),Cost*(1 +(AttackCount-1)*ComboMultiplier),false,false);
				HitAttributeComponent->AddEffect(hitEffect);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("hit"));

			}
		}
		
	//}
}

void USwordAttack::Start_Implementation(AActor* instigator)
{
	Super::Start_Implementation(instigator);

	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(AttackCount));
	if(AttackCount >= 3)
	{
		Stop_Implementation(instigator,false);
	}
	else
	{
		AttackCount++;
		ACharacter* Character = Cast<ACharacter>(instigator);
		
		instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Damage"),Cost);
		instigator->GetComponentByClass<UAttributeSystemComponent>()->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.ComboMultiplier"),ComboMultiplier);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start in c++"));
	}
	
	
}

void USwordAttack::Stop_Implementation(AActor* instigator,bool WasInterrupted)
{
	Super::Stop_Implementation(instigator,WasInterrupted);
	bHasHit = false;
	ACharacter* Character = Cast<ACharacter>(instigator);
	
	if(WasInterrupted || AttackCount >= 3)
	{
		AttackCount = 0;
	}
	StopSwordCollision();
}

void USwordAttack::OnAbilityAdded_Implementation(AActor* instigator)
{
	Super::OnAbilityAdded_Implementation(instigator);
}

void USwordAttack::OnAbilityRemoved_Implementation(AActor* instigator)
{
	Super::OnAbilityRemoved_Implementation(instigator);
}

void USwordAttack::OnAbilityStarted_Implementation(AActor* instigator)
{
	
	
}

void USwordAttack::OnAbilityStopped_Implementation(AActor* instigator)
{
	Super::OnAbilityStopped_Implementation(instigator);
}

bool USwordAttack::CanStartAbility_Implementation(AActor* instigator)
{
	ACharacter* character = Cast<ACharacter>(instigator);
	if(Super::CanStartAbility_Implementation(instigator) && character->CanJump())
	{
		return true;
	}
	return false;
}

bool USwordAttack::CanAddAbility_Implementation(AActor* instigator)
{
	return Super::CanAddAbility_Implementation(instigator);
}
