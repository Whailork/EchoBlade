#pragma once

#include "UAbility.h"
#include "FireballAbility.generated.h"

UCLASS(Blueprintable)
class UFireballAbility : public UAbility
{
	GENERATED_BODY()

public:
	UFireballAbility();

	/*
	// Niagara System pour les effets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* FireEffectSystem;

	// Composant Niagara pour afficher les effets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	class UNiagaraComponent* FireEffectComponent;
*/
	
	UFUNCTION(BlueprintCallable)
	AActor* Investigator(AActor* actorHit);

	UPROPERTY(EditAnywhere)
	float distance;

	// Conteneur effet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UGameplayEffect* effect;
};
