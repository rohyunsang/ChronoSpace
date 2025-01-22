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
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_TimePauseArea::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCSAT_TimePauseArea::SpawnAndInitializeTargetActor()
{
	//UE_LOG(LogCS, Log, TEXT("SpawnAndInitializeTargetActor"));
	SpawnedTargetActor = Cast<ACSTA_TimePauseArea>(GetWorld()->SpawnActorDeferred<ACSTA_TimePauseArea>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetDurationTime(DurationTime);
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_TimePauseArea::OnTargetActorReadyCallback);
	}
	else
	{
		UE_LOG(LogCS, Log, TEXT("Not Spawned SpawnedTargetActor"));
	}
}

void UCSAT_TimePauseArea::FinalizeTargetActor()
{
	//UE_LOG(LogCS, Log, TEXT("FinalizeTargetActor"));
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		if (SpawnedTargetActor == nullptr)
		{
			UE_LOG(LogCS, Log, TEXT("SpawnedTargetActor Not Found"));
			return;
		}
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Add(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);

		// 몇 초 후 종료
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_TimePauseArea::EndTargetActor, DurationTime, false);
	}
}

void UCSAT_TimePauseArea::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
}

void UCSAT_TimePauseArea::OnTargetActorReadyCallback()
{
	//UE_LOG(LogCS, Log, TEXT("OnTargetActorReadyCallback Called"));
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}

	EndTask();
}
