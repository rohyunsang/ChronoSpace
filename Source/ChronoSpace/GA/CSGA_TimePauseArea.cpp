// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/CSGA_TimePauseArea.h"
#include "GA/AT/CSAT_TimePauseArea.h"
#include "GA/TA/CSTA_TimePauseArea.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "ChronoSpace.h"

UCSGA_TimePauseArea::UCSGA_TimePauseArea()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_TimePauseArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateTask();
}

void UCSGA_TimePauseArea::ActivateTask()
{
	
	UCSAT_TimePauseArea* TimePauseTask = UCSAT_TimePauseArea::CreateTask(this, ACSTA_TimePauseArea::StaticClass());
	TimePauseTask->SetDurtionTime(1.0f);
	TimePauseTask->OnComplete.AddDynamic(this, &UCSGA_TimePauseArea::StopActivateTask);
	TimePauseTask->ReadyForActivation();
	
}

void UCSGA_TimePauseArea::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
