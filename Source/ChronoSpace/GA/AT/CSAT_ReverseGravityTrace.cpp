// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_ReverseGravityTrace.h"
#include "GA/TA/CSTA_ReverseGravityTrace.h"
#include "AbilitySystemComponent.h"

UCSAT_ReverseGravityTrace::UCSAT_ReverseGravityTrace()
{
}

UCSAT_ReverseGravityTrace* UCSAT_ReverseGravityTrace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_ReverseGravityTrace> TargetActorClass)
{
	UCSAT_ReverseGravityTrace* NewTask = NewAbilityTask<UCSAT_ReverseGravityTrace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UCSAT_ReverseGravityTrace::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_ReverseGravityTrace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCSAT_ReverseGravityTrace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_ReverseGravityTrace>(GetWorld()->SpawnActorDeferred<ACSTA_ReverseGravityTrace>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UCSAT_ReverseGravityTrace::OnTargetDataReadyCallback);
	}
}

void UCSAT_ReverseGravityTrace::FinalizeTargetActor()
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

void UCSAT_ReverseGravityTrace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
