// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_DurationTask.h"
#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ChronoSpace.h"

UCSAT_DurationTask::UCSAT_DurationTask()
{
}

UCSAT_DurationTask* UCSAT_DurationTask::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass)
{
	UCSAT_DurationTask* NewTask = NewAbilityTask<UCSAT_DurationTask>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UCSAT_DurationTask::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_DurationTask::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCSAT_DurationTask::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_ReverseGravityBox>(GetWorld()->SpawnActorDeferred<ACSTA_ReverseGravityBox>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetDurationTime(DurationTime);
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_DurationTask::OnTargetActorReadyCallback);
	}
}

void UCSAT_DurationTask::FinalizeTargetActor()
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
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_DurationTask::EndTargetActor, DurationTime, false);
	}
}

void UCSAT_DurationTask::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
}

void UCSAT_DurationTask::OnTargetActorReadyCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}

	EndTask();
}
