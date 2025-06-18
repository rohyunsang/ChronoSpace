// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/CSAT_ReverseGravityBox.h"
#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemComponent.h"
#include "ChronoSpace.h"

UCSAT_ReverseGravityBox::UCSAT_ReverseGravityBox()
{
}

UCSAT_ReverseGravityBox* UCSAT_ReverseGravityBox::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ACSTA_ReverseGravityBox> InTargetActorClass)
{
	UCSAT_ReverseGravityBox* NewTask = NewAbilityTask<UCSAT_ReverseGravityBox>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;

	return NewTask;
}

void UCSAT_ReverseGravityBox::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACSTA_ReverseGravityBox>(GetWorld()->SpawnActorDeferred<ACSTA_ReverseGravityBox>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetOwner(GetOwnerActor());
		SpawnedTargetActor->OnComplete.AddDynamic(this, &UCSAT_ReverseGravityBox::OnTargetActorReadyCallback);
	}
}

void UCSAT_ReverseGravityBox::FinalizeTargetActor()
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

		// �� �� �� ����
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UCSAT_ReverseGravityBox::EndTargetActor, DurationTime, false);
	}
}

void UCSAT_ReverseGravityBox::EndTargetActor()
{
	SpawnedTargetActor->ConfirmTargeting();
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
