// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_ReverseGravity.h"
#include "GA/AT/CSAT_ReverseGravityBox.h"
#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "ChronoSpace.h"

UCSGA_ReverseGravity::UCSGA_ReverseGravity()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_ReverseGravity::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogCS, Log, TEXT("Begin"));
	
	ActivateTasks();
}

void UCSGA_ReverseGravity::ActivateTasks()
{
	UCSAT_ReverseGravityBox* BoxTask = UCSAT_ReverseGravityBox::CreateTask(this, ACSTA_ReverseGravityBox::StaticClass());
	//StopActivateTasks();
	BoxTask->OnComplete.AddDynamic(this, &UCSGA_ReverseGravity::StopActivateTasks);
	BoxTask->ReadyForActivation();
}

void UCSGA_ReverseGravity::StopActivateTasks()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
