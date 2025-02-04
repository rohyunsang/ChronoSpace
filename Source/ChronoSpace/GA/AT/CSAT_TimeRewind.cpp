#include "GA/AT/CSAT_TimeRewind.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

/** 시간 되감기 Task 생성 */
UCSAT_TimeRewind* UCSAT_TimeRewind::CreateTimeRewindTask(
    UGameplayAbility* OwningAbility,
    AActor* TargetActor,
    const TArray<FCSF_CharacterFrameData>& TransformHistory,
    float Duration)
{
    UCSAT_TimeRewind* Task = NewAbilityTask<UCSAT_TimeRewind>(OwningAbility);
    if (Task)
    {
        Task->TargetActor = TargetActor;
        Task->TransformFrames = TransformHistory;
        Task->CurrentFrameIndex = TransformHistory.Num() - 1;  // 가장 최신 프레임부터 이동 시작
        Task->DurationPerFrame = Duration / TransformHistory.Num(); // 1초 동안 300개의 프레임을 따라 이동
    }
    return Task;
}

/** Task 활성화 */
void UCSAT_TimeRewind::Activate()
{
    if (!TargetActor || TransformFrames.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid TargetActor or empty TransformFrames in TimeRewindTask!"));
        OnTimeRewindFinished.Broadcast();
        EndTask();
        return;
    }

    MoveToNextFrame();
}

/** 지나온 길을 따라 이동 */
void UCSAT_TimeRewind::MoveToNextFrame()
{
    if (CurrentFrameIndex <= 0)
    {
        FinishRewind();
        return;
    }

    // 현재 프레임과 이전 프레임 설정
    FCSF_CharacterFrameData CurrentFrame = TransformFrames[CurrentFrameIndex];
    FCSF_CharacterFrameData NextFrame = TransformFrames[CurrentFrameIndex - 1];

    // Lerp로 부드럽게 보간하며 이동
    FVector NewLocation = FMath::VInterpTo(TargetActor->GetActorLocation(), NextFrame.Location, GetWorld()->GetDeltaSeconds(), 10.0f);
    FRotator NewRotation = FMath::RInterpTo(TargetActor->GetActorRotation(), NextFrame.Rotation, GetWorld()->GetDeltaSeconds(), 10.0f);

    TargetActor->SetActorLocationAndRotation(NewLocation, NewRotation);

    // 다음 프레임으로 진행
    CurrentFrameIndex--;
    if (CurrentFrameIndex < 0) CurrentFrameIndex = 0; // 음수 방지


    // 일정 시간 후 다시 실행 
    GetWorld()->GetTimerManager().SetTimer(
        RewindTimerHandle,
        this,
        &UCSAT_TimeRewind::MoveToNextFrame,
        DurationPerFrame,
        false
    );
}

/** 되감기 종료 */
void UCSAT_TimeRewind::FinishRewind()
{
    GetWorld()->GetTimerManager().ClearTimer(RewindTimerHandle);
    UE_LOG(LogTemp, Log, TEXT("Time rewind finished."));
    OnTimeRewindFinished.Broadcast();
    EndTask();
}
