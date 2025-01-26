#include "FireballAbility.h"
//#include "\Epic Games\UE_5.4\Engine\Source"
#include <ObjectArray.h>

#include "UGameplayEffect.h"
#include "ASActionSystemComponent.h"
#include "GameplayTagsManager.h"

//#include "../../../../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

UFireballAbility::UFireballAbility()
{
	// Set gameplay tag
	AbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Fireball");

	/*
	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	// Création du composant Niagara
	FireEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffectComponent"));
	FireEffectComponent->SetupAttachment(RootComp); // Attache au composant racine de l'ability
	*/

	//ConstructorHelpers::FClassFinder<UGameplayEffect> FireEffectAsset (TEXT("/Game/BP_FireGameplayEffect")); // static ConstructorHelpers::FClassFinder<UGameplayEffect> SprintEffectAsset (TEXT("Blueprint'/Game/1TEST/IncreaseMoveSpeed/GE_MoveSpeed.GE_MoveSpeed_C'"));
	//EffectClass = FireEffectAsset.Class;

	//effect = EffectClass->GetDefaultObject<FireGameplayEffect>();
}

AActor* UFireballAbility::Investigator(AActor* actorHit)
{

	//effec
	
	FVector StartTrace = actorHit->GetActorLocation();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, actorHit->GetActorForwardVector().ToString());
	FVector Direction = actorHit->GetActorForwardVector();
	FVector EndTrace = StartTrace + Direction * distance;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(actorHit);

	FHitResult Hit(ForceInit);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, TraceParams);

	DrawDebugLine(
		GetWorld(),
		StartTrace,
		EndTrace,
		bHit ? FColor::Green :FColor::Red ,
		true,
		1,
		0,
		1
	);

	return Hit.GetActor();
}
