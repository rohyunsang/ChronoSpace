// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_TimePauseArea.h"
#include "GA/TA/CSTA_TimePauseArea.h"
#include "AbilitySystemComponent.h"
#include "ChronoSpace.h"

UCSAT_TimePauseArea::UCSAT_TimePauseArea()
{
}

UCSAT_TimePauseArea* UCSAT_TimePauseArea::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_TimePauseArea> TargetActorClass)
{
	UCSAT_TimePauseArea* NewTask = NewAbilityTask<UCSAT_TimePauseArea>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UCSAT_TimePauseArea::Activate()
{
}

void UCSAT_TimePauseArea::OnDestroy(bool AbilityEnded)
{
}

void UCSAT_TimePauseArea::SpawnAndInitializeTargetActor()
{
}

void UCSAT_TimePauseArea::FinalizeTargetActor()
{
}

void UCSAT_TimePauseArea::EndTargetActor()
{
}

void UCSAT_TimePauseArea::OnTargetActorReadyCallback()
{
}
