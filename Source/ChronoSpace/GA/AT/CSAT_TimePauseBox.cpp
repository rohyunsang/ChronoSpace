// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_TimePauseBox.h"
#include "GA/TA/CSTA_TimePauseBox.h"
#include "AbilitySystemComponent.h"
#include "ChronoSpace.h"

UCSAT_TimePauseBox::UCSAT_TimePauseBox()
{
}

UCSAT_TimePauseBox* UCSAT_TimePauseBox::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_TimePauseBox> InTargetActorClass)
{
	UCSAT_TimePauseBox* NewTask = NewAbilityTask<UCSAT_TimePauseBox>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;

	return NewTask;
}

void UCSAT_TimePauseBox::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_TimePauseBox>(GetWorld()->SpawnActorDeferred<ACSTA_TimePauseBox>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_TimePauseBox::OnTargetActorReadyCallback);
	}
}
void UCSAT_TimePauseBox::FinalizeTargetActor()
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
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_TimePauseBox::EndTargetActor, DurationTime, false);
	}

}
void UCSAT_TimePauseBox::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_TimePauseBox::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}




void UCSAT_TimePauseBox::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
}
