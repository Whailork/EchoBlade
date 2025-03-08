// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class ECHOBLADE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	//void SetGameplayEffect(TSubclassOf<UGameplayEffect> Effect) { FireballEffect = Effect; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Projectile components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileComponent")
	UStaticMeshComponent* SphereMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileComponent")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileComponent")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileComponent")
	float projectileRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileComponent")
	float InitialSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileComponent")
	float MaxSpeed;

private:
	//UPROPERTY()
	//TSubclassOf<UGameplayEffect> FireballEffect;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

