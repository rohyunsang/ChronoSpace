// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_WeakenGravityBox.h"
#include "GA/TA/CSTA_WeakenGravityBox.h"
#include "AbilitySystemComponent.h"
#include "ChronoSpace.h"

UCSAT_WeakenGravityBox::UCSAT_WeakenGravityBox()
{
}

UCSAT_WeakenGravityBox* UCSAT_WeakenGravityBox::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_WeakenGravityBox> InTargetActorClass)
{
	UCSAT_WeakenGravityBox* NewTask = NewAbilityTask<UCSAT_WeakenGravityBox>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;

	return NewTask;
}

void UCSAT_WeakenGravityBox::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCSAT_WeakenGravityBox::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCSAT_WeakenGravityBox::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_WeakenGravityBox>(GetWorld()->SpawnActorDeferred<ACSTA_WeakenGravityBox>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetOwner(GetOwnerActor());
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_WeakenGravityBox::OnTargetActorReadyCallback);
		SpawnedTargetActor->SetGravityCoef(GravityCoef);
	}
}

void UCSAT_WeakenGravityBox::FinalizeTargetActor()
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
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_WeakenGravityBox::EndTargetActor, DurationTime, false);
	}
}

void UCSAT_WeakenGravityBox::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
}
