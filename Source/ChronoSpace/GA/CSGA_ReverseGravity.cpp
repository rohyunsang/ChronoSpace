// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_ReverseGravity.h"
#include "GA/AT/CSAT_ReverseGravityTrace.h"
#include "GA/TA/CSTA_ReverseGravityTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StaticMeshComponent.h"
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
	int32 idx = 0;
	while (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, idx))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, idx);
		UE_LOG(LogCS, Log, TEXT("Target %s Detected"), *HitResult.GetActor()->GetName());

		UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(HitResult.GetComponent());

		if (StaticMeshComp)
		{
			UE_LOG(LogCS, Log, TEXT("StaticMeshComp Found!"));
			FVector ZForce(0.0f, 0.0f, 30000.0f);
			StaticMeshComp->AddImpulse(ZForce);
		}
		++idx;
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
