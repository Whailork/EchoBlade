// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Collision component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(projectileRadius);
	RootComponent = SphereComponent;

	// Mesh component
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	//SphereMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	SphereMesh->SetupAttachment(SphereComponent);
	

	// Movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	/*
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	*/
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	
	UAttributeSystemComponent* TargetAttributeSystem = OtherActor->GetComponentByClass<UAttributeSystemComponent>();
	
	if(TargetAttributeSystem && LinkedEffectClass)
	{
		UAttributeSystemComponent* ownerAttributes = GetOwner()->GetComponentByClass<UAttributeSystemComponent>();
		float OwnerDamage = 0;
		if(ownerAttributes)
		{
			ownerAttributes->GetAttributeValue(UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Damage"),OwnerDamage);
		}
		
		UCustomGameplayEffect* Effect = NewObject<UCustomGameplayEffect>(GetOwner(),LinkedEffectClass);
		Effect->InitializeValues(0,0.001,UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health"),OwnerDamage,false,false);
		TargetAttributeSystem->AddEffect(Effect);
	}
	/*if (TargetASC && FireballEffect)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpec = TargetASC->MakeOutgoingSpec(FireballEffect, 1.f, EffectContext);
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
	}*/
	
	Destroy();
}
