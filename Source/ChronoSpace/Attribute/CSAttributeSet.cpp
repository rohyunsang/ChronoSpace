// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/CSAttributeSet.h"
#include "ChronoSpace.h"
#include "GameplayEffectExtension.h"
#include "CoreMinimal.h"



UCSAttributeSet::UCSAttributeSet() : MaxEnergy(100.0f)
{
	InitEnergy(GetMaxEnergy());
}

void UCSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	/*
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
	*/
	// -> 최소 데미지 0.0f 으로 설정. 
}

bool UCSAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return false;
}

void UCSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumEnergy = 0.0f;

	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetEnergy());
		SetEnergy(FMath::Clamp(GetEnergy(), MinimumEnergy, GetMaxEnergy()));
	}
}
