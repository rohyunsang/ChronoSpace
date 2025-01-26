// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_AbilityPreviewBox.h"
#include "GA/AT/CSAT_AbilityPreviewBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GA/CSGA_ChronoControl.h"


UCSGA_AbilityPreviewBox::UCSGA_AbilityPreviewBox()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCSGA_AbilityPreviewBox::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("ActivateAiblity"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateTask();
}

void UCSGA_AbilityPreviewBox::ActivateTask()
{
	UCSAT_AbilityPreviewBox* BoxTask = UCSAT_AbilityPreviewBox::CreateTask(this);
	BoxTask->SetDurtionTime(DurationTime);
	
	BoxTask->RunAbility.AddDynamic(this, &UCSGA_AbilityPreviewBox::RunAbility);
	BoxTask->StopAbility.AddDynamic(this, &UCSGA_AbilityPreviewBox::StopAbility);

	BoxTask->ReadyForActivation();
}

void UCSGA_AbilityPreviewBox::RunAbility()
{
	// 새로운 어빌리티 실행
	if (CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		FGameplayAbilitySpec* NewAbilitySpec = ASC->FindAbilitySpecFromClass(UCSGA_ChronoControl::StaticClass());
		if (NewAbilitySpec)
		{
			ASC->TryActivateAbility(NewAbilitySpec->Handle); // 새로운 어빌리티 실행
		}
	}

	StopActivateTask(); // 부모 어빌리티 종료 처리
}

void UCSGA_AbilityPreviewBox::StopAbility()
{
	// 단순 종료 처리
	StopActivateTask();
}

void UCSGA_AbilityPreviewBox::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
