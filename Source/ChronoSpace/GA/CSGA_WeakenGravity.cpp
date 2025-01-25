// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_WeakenGravity.h"
#include "GA/AT/CSAT_WeakenGravityBox.h"
#include "GA/TA/CSTA_WeakenGravityBox.h"
#include "ChronoSpace.h"

UCSGA_WeakenGravity::UCSGA_WeakenGravity()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_WeakenGravity::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateTask();
}

void UCSGA_WeakenGravity::ActivateTask()
{
	UCSAT_WeakenGravityBox* BoxTask = UCSAT_WeakenGravityBox::CreateTask(this, ACSTA_WeakenGravityBox::StaticClass());
	BoxTask->SetDurtionTime(DurationTime);
	BoxTask->SetGravityCoef(GravityCoef);
	BoxTask->OnComplete.AddDynamic(this, &UCSGA_WeakenGravity::StopActivateTask);
	BoxTask->ReadyForActivation();
}

void UCSGA_WeakenGravity::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
