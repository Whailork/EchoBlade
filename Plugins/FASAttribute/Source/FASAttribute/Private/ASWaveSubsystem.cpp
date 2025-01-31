// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWaveSubsystem.h"

#include <string>

#include "FASAttribute.h"


void UASWaveSubsystem::InitializeAI(UDataTable* DataTable)
{
	TArray<FName> names =DataTable->GetRowNames();
	
	for (FName name : names)
	{
		FString contextString;
		FASAIDefaultAttributesDataTable* defaultAttributes = DataTable->FindRow<FASAIDefaultAttributesDataTable>(name,contextString);
		
		UE_LOG(LogTemp,Warning,TEXT("%s"), *name.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,  name.ToString());
		UE_LOG(LogTemp,Warning,TEXT("life : %s"), *FString::SanitizeFloat(defaultAttributes->MaxLife));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,FString::SanitizeFloat(defaultAttributes->MaxLife));
		UE_LOG(LogTemp,Warning,TEXT("shield : %s"), *FString::SanitizeFloat(defaultAttributes->MaxShield));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(defaultAttributes->MaxShield));
		
	}

}
