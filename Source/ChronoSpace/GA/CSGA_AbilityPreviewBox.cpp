// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_AbilityPreviewBox.h"
#include "GA/AT/CSAT_AbilityPreviewBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GA/CSGA_ChronoControl.h"
#include "GA/CSGA_ReverseGravity.h"
#include "GA/CSGA_WeakenGravity.h"
#include "DataAsset/CSDA_BoxProperties.h"
#include "ChronoSpace.h"


UCSGA_AbilityPreviewBox::UCSGA_AbilityPreviewBox()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	Ability = EAbilityIndex::ChronoControl;
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

void UCSGA_AbilityPreviewBox::RunAbility(float BoxSize)
{
	// /Script/ChronoSpace.CSDA_BoxProperties'/Game/DataAssets/CSDA_BoxProperties.CSDA_BoxProperties'

	// ������ ���� ���
	const FString AssetPath = TEXT("/Game/DataAssets/CSDA_BoxProperties.CSDA_BoxProperties");
	UCSDA_BoxProperties* BoxPropertiesAsset = Cast<UCSDA_BoxProperties>(StaticLoadObject(UCSDA_BoxProperties::StaticClass(), nullptr, *AssetPath));

	if (BoxPropertiesAsset)
	{
		// BoxSize ����
		BoxPropertiesAsset->BoxSize = BoxSize;
		UE_LOG(LogTemp, Log, TEXT("Successfully updated BoxSize in DataAsset to: %f"), BoxSize);

		// ���� ���� ���� (�����Ϳ����� ����)
#if WITH_EDITOR
		BoxPropertiesAsset->MarkPackageDirty();
#endif
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load BoxPropertiesDataAsset at path: %s"), *AssetPath);
	}

	if (CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		FGameplayAbilitySpec* NewAbilitySpec;

		if ( Ability == EAbilityIndex::ChronoControl )
		{
			NewAbilitySpec = ASC->FindAbilitySpecFromClass(UCSGA_ChronoControl::StaticClass());
			if (NewAbilitySpec)
			{
				ASC->TryActivateAbility(NewAbilitySpec->Handle); // ���ο� �����Ƽ ����
			}
		}
		else if ( Ability == EAbilityIndex::ReverseGravity )
		{
			UE_LOG(LogCS, Log, TEXT("Preview - ReverseGravity"));
			NewAbilitySpec = ASC->FindAbilitySpecFromClass(UCSGA_ReverseGravity::StaticClass());
			if (NewAbilitySpec)
			{
				UE_LOG(LogCS, Log, TEXT("Preview - ReverseGravity Activated"));
				ASC->TryActivateAbility(NewAbilitySpec->Handle); 
			}
		}
		else if ( Ability == EAbilityIndex::WeakenGravity10P )
		{
			NewAbilitySpec = ASC->FindAbilitySpecFromClass(WeakenGravity10P);
			if (NewAbilitySpec)
			{
				ASC->TryActivateAbility(NewAbilitySpec->Handle);
			}
		}
		else if (Ability == EAbilityIndex::WeakenGravity50P)
		{
			NewAbilitySpec = ASC->FindAbilitySpecFromClass(WeakenGravity50P);
			if (NewAbilitySpec)
			{
				ASC->TryActivateAbility(NewAbilitySpec->Handle);
			}
		}
	}

	StopActivateTask(); // �θ� �����Ƽ ���� ó��
}

void UCSGA_AbilityPreviewBox::StopAbility()
{
	// �ܼ� ���� ó��
	StopActivateTask();
}

void UCSGA_AbilityPreviewBox::StopActivateTask()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
