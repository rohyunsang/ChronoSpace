// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_ReverseGravity.h"
#include "GA/AT/CSAT_ReverseGravityTrace.h"
#include "GA/TA/CSTA_ReverseGravityTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ChronoSpace.h"

UCSGA_ReverseGravity::UCSGA_ReverseGravity()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_ReverseGravity::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogCS, Log, TEXT("Begin"));

	UCSAT_ReverseGravityTrace* AttackTraceTask = UCSAT_ReverseGravityTrace::CreateTask(this, ACSTA_ReverseGravityTrace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UCSGA_ReverseGravity::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UCSGA_ReverseGravity::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogCS, Log, TEXT("Target %s Detected"), *HitResult.GetActor()->GetName());

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		if (!SourceASC || !TargetASC)
		{
			UE_LOG(LogCS, Log, TEXT("ASC Not Found.."));
			return;
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
