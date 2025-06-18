// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Character/CSF_CharacterFrameData.h"
#include "CSAT_TimeRewind.generated.h"

/** Task �Ϸ� �� ��ε�ĳ��Ʈ�� ��������Ʈ */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeRewindFinished);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_TimeRewind : public UAbilityTask
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnTimeRewindFinished OnTimeRewindFinished;

    /** �ð� �ǰ��� Task ���� */
    static UCSAT_TimeRewind* CreateTimeRewindTask(
        UGameplayAbility* OwningAbility,
        AActor* TargetActor,
        const TArray<FCSF_CharacterFrameData>& TransformHistory,
        float Duration);

protected:
    virtual void Activate() override; // Task ����
    void MoveToNextFrame();           // ������ ���� ���� �̵�
    void FinishRewind();              // �ǰ��� ����

private:
    AActor* TargetActor;              // �ǰ��� ��� ����
    TArray<FCSF_CharacterFrameData> TransformFrames; // ����� �����ӵ�
    int32 CurrentFrameIndex;          // ���� �̵� ���� ������ �ε���
    float DurationPerFrame;           // �� �����Ӵ� �̵� �ð�
    FTimerHandle RewindTimerHandle;   // Ÿ�̸� �ڵ�
};