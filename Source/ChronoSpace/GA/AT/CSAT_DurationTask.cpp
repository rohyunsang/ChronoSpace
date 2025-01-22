// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_DurationTask.h"

void UCSAT_DurationTask::OnTargetActorReadyCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}

	EndTask();
}
