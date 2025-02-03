// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Character/CSF_CharacterFrameData.h"
#include "CSAT_TimeRewind.generated.h"

/** Task 완료 시 브로드캐스트할 델리게이트 */
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

    /** 시간 되감기 Task 생성 */
    static UCSAT_TimeRewind* CreateTimeRewindTask(
        UGameplayAbility* OwningAbility,
        AActor* TargetActor,
        const TArray<FCSF_CharacterFrameData>& TransformHistory,
        float Duration);

protected:
    virtual void Activate() override; // Task 실행
    void MoveToNextFrame();           // 지나온 길을 따라 이동
    void FinishRewind();              // 되감기 종료

private:
    AActor* TargetActor;              // 되감기 대상 액터
    TArray<FCSF_CharacterFrameData> TransformFrames; // 저장된 프레임들
    int32 CurrentFrameIndex;          // 현재 이동 중인 프레임 인덱스
    float DurationPerFrame;           // 한 프레임당 이동 시간
    FTimerHandle RewindTimerHandle;   // 타이머 핸들
};