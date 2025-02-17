// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_GiveDamage.h"
#include "GA/AT/CSAT_MultiTrace.h"
#include "GA/TA/CSTA_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ChronoSpace.h"

UCSGA_GiveDamage::UCSGA_GiveDamage()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DamageEffectRef(TEXT("/Game/Blueprint/GA/GE/BPGE_PatrolDamage.BPGE_PatrolDamage_C"));
	if ( DamageEffectRef.Succeeded() )
	{
		DamageEffect = DamageEffectRef.Class;
	}
}

void UCSGA_GiveDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//UE_LOG(LogCS, Log, TEXT("ActivateAbility - GiveDamage"));

	UCSAT_MultiTrace* DamageTraceTask = UCSAT_MultiTrace::CreateTask(this, ACSTA_MultiTrace::StaticClass());
	DamageTraceTask->OnComplete.AddDynamic(this, &UCSGA_GiveDamage::OnTraceResultCallback);
	DamageTraceTask->ReadyForActivation();
}

void UCSGA_GiveDamage::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	int32 idx = 0;
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect);
	//UE_LOG(LogCS, Log, TEXT("OnTraceResultCallback"));
	if (EffectSpecHandle.IsValid())
	{
		//UE_LOG(LogCS, Log, TEXT("OnTraceResultCallback - EffectSpecHandle Is Valid"));
		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
	}

	/*while (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, idx))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, idx);
		
		UE_LOG(LogCS, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++idx;
	}*/

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
