// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_BlackHole.h"
#include "GA/AT/CSAT_BlackHoleSphere.h"
#include "GA/TA/CSTA_BlackHoleSphere.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "ChronoSpace.h"

UCSGA_BlackHole::UCSGA_BlackHole()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_BlackHole::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogCS, Log, TEXT("UCSGA_BlackHole ActivateAbility"));
	ActivateTask();
}

void UCSGA_BlackHole::ActivateTask()
{
	UCSAT_BlackHoleSphere* SphereTask = UCSAT_BlackHoleSphere::CreateTask(this, ACSTA_BlackHoleSphere::StaticClass());
	SphereTask->SetDurtionTime(DurationTime);
	SphereTask->OnComplete.AddDynamic(this, &UCSGA_BlackHole::StopActivateTask);
	SphereTask->ReadyForActivation();
}

void UCSGA_BlackHole::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
