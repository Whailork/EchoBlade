#pragma once

#include "CoreMinimal.h"
#include "ASActionSystemComponent.h"
#include "AttributeSystemComponent.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "UGameplayEffect.generated.h"

UENUM(BlueprintType)
enum class EModifierOperation : uint8
{
    Add UMETA(DisplayName = "Add"),
    Subtract UMETA(DisplayName = "Subtract"),
    Multiply UMETA(DisplayName = "Multiply"),
    Divide UMETA(DisplayName = "Divide")
};

USTRUCT(BlueprintType)
struct FAttributeModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    FGameplayTag TargetAttribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
    EModifierOperation Operation;
};


UCLASS(Blueprintable)
class FASATTRIBUTE_API UGameplayEffect : public UObject
{
    GENERATED_BODY()

public:
    UGameplayEffect();
    FOnEffectAdded addedDelegate;
    FOnEffectRemoved removedDelegate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool bLooping;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool bStoppedByEvent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Period;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FGameplayTag TagToAdd;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FGameplayTagContainer BlockingTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FAttributeModifier AttributeModifiers;

    UPROPERTY()
    AActor* InstigatorActor;
    FTimerHandle effectHandle;
    UFUNCTION(BlueprintNativeEvent)
    void OnEffectAdded(AActor* instigator);

    UFUNCTION(BlueprintNativeEvent)
    void OnEffectRemoved(AActor* instigator);

    UFUNCTION(BlueprintNativeEvent)
    void OnEffectTriggered();

    UFUNCTION(BlueprintNativeEvent)
    void InitializeValues(float inDuration, float inPeriod,FGameplayTag AffectedAttributeTag,float effectPower,bool inLooping, bool inStoppedByEvent);
    UFUNCTION()
    void StopPeriodTimer();
};
