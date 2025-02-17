// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/CSAttributeSet.h"
#include "ChronoSpace.h"
#include "GameplayEffectExtension.h"
#include "Player/CSPlayerController.h"
//#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"

UCSAttributeSet::UCSAttributeSet() : MaxEnergy(100.0f), Damage(0.0f)
{
	InitEnergy(GetMaxEnergy());
}

void UCSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
	
	// -> 최소 데미지 0.0f 으로 설정. 
}

bool UCSAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return true;
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

	if ( Data.EvaluatedData.Attribute == GetDamageAttribute() )
	{
		SetEnergy(FMath::Clamp(GetEnergy() - GetDamage(), MinimumEnergy, GetMaxEnergy()));
		//UE_LOG(LogCS, Log, TEXT("[NetMode : %d] Damage Detected : %f | Now Energy : %f"), GetWorld()->GetNetMode(), GetDamage(), GetEnergy());

		AActor* TargetActor = Data.Target.GetAvatarActor();
		if (TargetActor == nullptr) return;

		if (APawn* Pawn = Cast<APawn>(TargetActor))
		{
			ACSPlayerController* PC = Cast<ACSPlayerController>(Pawn->GetController());

			if (PC)
			{
				PC->ShakeCamera();
			}
		}
	}
}

void UCSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCSAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCSAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
}

void UCSAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCSAttributeSet, Energy, OldEnergy);
}

void UCSAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCSAttributeSet, MaxEnergy, OldMaxEnergy);
}


