// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_TimePause.h"
#include "GA/AT/CSAT_TimePauseBox.h"
#include "GA/TA/CSTA_TimePauseBox.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "ChronoSpace.h"

UCSGA_TimePause::UCSGA_TimePause()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_TimePause::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateTask();
}

void UCSGA_TimePause::ActivateTask()
{
	UCSAT_TimePauseBox* BoxTask = UCSAT_TimePauseBox::CreateTask(this, ACSTA_TimePauseBox::StaticClass());
	BoxTask->SetDurtionTime(DurationTime);
	BoxTask->OnComplete.AddDynamic(this, &UCSGA_TimePause::StopActivateTask);
	BoxTask->ReadyForActivation();
}

void UCSGA_TimePause::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
