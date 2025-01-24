// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_AbilityPreviewBox.h"
#include "GA/AT/CSAT_AbilityPreviewBox.h"


UCSGA_AbilityPreviewBox::UCSGA_AbilityPreviewBox()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_AbilityPreviewBox::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateTask();
}

void UCSGA_AbilityPreviewBox::ActivateTask()
{
	UCSAT_AbilityPreviewBox* BoxTask = UCSAT_AbilityPreviewBox::CreateTask(this);
	BoxTask->SetDurtionTime(DurationTime);
	BoxTask->OnComplete.AddDynamic(this, &UCSGA_AbilityPreviewBox::StopActivateTask);
	BoxTask->ReadyForActivation();
}

void UCSGA_AbilityPreviewBox::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
