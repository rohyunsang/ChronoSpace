// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CSTransformRecordComponent.h"

UCSTransformRecordComponent::UCSTransformRecordComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCSTransformRecordComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		RecordTimer,
		this,
		&UCSTransformRecordComponent::RecordTransform,
		RecordInterval,
		true,
		0.0f
	);
}

void UCSTransformRecordComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(RecordTimer);
}

void UCSTransformRecordComponent::RecordTransform()
{
	FCSF_CharacterFrameData FrameData(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), GetWorld()->GetTimeSeconds());

	if (TransformHistory.Num() >= MaxHistorySize + 1) // 1 is offset
	{
		TransformHistory.RemoveAt(0);
	}

	TransformHistory.Add(FrameData);
}

