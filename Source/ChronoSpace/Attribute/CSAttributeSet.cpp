// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/CSAttributeSet.h"
#include "ChronoSpace.h"
#include "GameplayEffectExtension.h"
#include "Player/CSPlayerController.h"
#include "Net/UnrealNetwork.h"

UCSAttributeSet::UCSAttributeSet()
{
	// 기본값 설정
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

void UCSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCSAttributeSet, Health, OldHealth);
}

void UCSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCSAttributeSet, MaxHealth, OldMaxHealth);
}

void UCSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// HP 클램핑
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}
