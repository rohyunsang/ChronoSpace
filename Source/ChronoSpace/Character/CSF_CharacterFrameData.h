// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CSF_CharacterFrameData.generated.h"

USTRUCT(BlueprintType)
struct CHRONOSPACE_API FCSF_CharacterFrameData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transform History")
    FVector Location;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transform History")
    FRotator Rotation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transform History")
    float Timestamp;

    // �⺻ ������
    FCSF_CharacterFrameData()
        : Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Timestamp(0.0f)
    {
    }

    // �߰� ������
    FCSF_CharacterFrameData(const FVector& InLocation, const FRotator& InRotation, float InTimestamp)
        : Location(InLocation), Rotation(InRotation), Timestamp(InTimestamp)
    {
    }
};