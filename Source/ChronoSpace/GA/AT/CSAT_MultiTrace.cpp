// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_MultiTrace.h"
#include "GA/TA/CSTA_MultiTrace.h"
#include "AbilitySystemComponent.h"

UCSAT_MultiTrace::UCSAT_MultiTrace()
{
}

UCSAT_MultiTrace* UCSAT_MultiTrace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_MultiTrace> TargetActorClass)
{
	UCSAT_MultiTrace* NewTask = NewAbilityTask<UCSAT_MultiTrace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UCSAT_MultiTrace::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_MultiTrace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCSAT_MultiTrace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_MultiTrace>(GetWorld()->SpawnActorDeferred<ACSTA_MultiTrace>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetOwner(GetOwnerActor());
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UCSAT_MultiTrace::OnTargetDataReadyCallback);
	}
}

void UCSAT_MultiTrace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Add(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UCSAT_MultiTrace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
