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
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_BlackHole::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateTask();
}

void UCSGA_BlackHole::ActivateTask()
{
	//UCSAT_BlackHoleSphere* SphereTask = 
}

void UCSGA_BlackHole::StopActivateTask()
{
}
