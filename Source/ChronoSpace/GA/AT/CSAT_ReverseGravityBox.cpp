// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_ReverseGravityBox.h"
#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemComponent.h"
#include "ChronoSpace.h"

UCSAT_ReverseGravityBox::UCSAT_ReverseGravityBox()
{
}

UCSAT_ReverseGravityBox* UCSAT_ReverseGravityBox::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_ReverseGravityBox> TargetActorClass)
{
	UCSAT_ReverseGravityBox* NewTask = NewAbilityTask<UCSAT_ReverseGravityBox>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UCSAT_ReverseGravityBox::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_ReverseGravityBox::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCSAT_ReverseGravityBox::SpawnAndInitializeTargetActor()
{
	//UE_LOG(LogCS, Log, TEXT("SpawnAndInitializeTargetActor"));
	SpawnedTargetActor = Cast<ACSTA_ReverseGravityBox>(GetWorld()->SpawnActorDeferred<ACSTA_ReverseGravityBox>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetDurationTime(DurationTime);
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_ReverseGravityBox::OnTargetActorReadyCallback);
	}
	else
	{
		UE_LOG(LogCS, Log, TEXT("Not Spawned SpawnedTargetActor"));
	}
}

void UCSAT_ReverseGravityBox::FinalizeTargetActor()
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
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_ReverseGravityBox::EndTargetActor, DurationTime, false);
	}
}

void UCSAT_ReverseGravityBox::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
}

void UCSAT_ReverseGravityBox::OnTargetActorReadyCallback()
{
	//UE_LOG(LogCS, Log, TEXT("OnTargetActorReadyCallback Called"));
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}

	EndTask();
}
