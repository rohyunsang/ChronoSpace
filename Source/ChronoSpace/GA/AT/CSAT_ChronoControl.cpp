// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_ChronoControl.h"
#include "GA/TA/CSTA_ChronoControl.h"
#include "AbilitySystemComponent.h"
#include "ChronoSpace.h"

UCSAT_ChronoControl::UCSAT_ChronoControl()
{
}

UCSAT_ChronoControl* UCSAT_ChronoControl::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_ChronoControl> InTargetActorClass)
{
	UCSAT_ChronoControl* NewTask = NewAbilityTask<UCSAT_ChronoControl>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;

	return NewTask;
}

void UCSAT_ChronoControl::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_ChronoControl>(GetWorld()->SpawnActorDeferred<ACSTA_ChronoControl>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_ChronoControl::OnTargetActorReadyCallback);
	}
}
void UCSAT_ChronoControl::FinalizeTargetActor()
{
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
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_ChronoControl::EndTargetActor, DurationTime, false);
	}

}
void UCSAT_ChronoControl::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_ChronoControl::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}




void UCSAT_ChronoControl::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
}
