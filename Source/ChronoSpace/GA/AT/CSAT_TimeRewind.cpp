#include "GA/AT/CSAT_TimeRewind.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

/** �ð� �ǰ��� Task ���� */
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
        Task->CurrentFrameIndex = TransformHistory.Num() - 1;  // ���� �ֽ� �����Ӻ��� �̵� ����
        Task->DurationPerFrame = Duration / TransformHistory.Num(); // 1�� ���� 300���� �������� ���� �̵�
    }
    return Task;
}

/** Task Ȱ��ȭ */
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

/** ������ ���� ���� �̵� */
void UCSAT_TimeRewind::MoveToNextFrame()
{
    if (CurrentFrameIndex <= 0)
    {
        FinishRewind();
        return;
    }

    // ���� �����Ӱ� ���� ������ ����
    FCSF_CharacterFrameData CurrentFrame = TransformFrames[CurrentFrameIndex];
    FCSF_CharacterFrameData NextFrame = TransformFrames[CurrentFrameIndex - 1];

    // Lerp�� �ε巴�� �����ϸ� �̵�
    FVector NewLocation = FMath::VInterpTo(TargetActor->GetActorLocation(), NextFrame.Location, GetWorld()->GetDeltaSeconds(), 10.0f);
    FRotator NewRotation = FMath::RInterpTo(TargetActor->GetActorRotation(), NextFrame.Rotation, GetWorld()->GetDeltaSeconds(), 10.0f);

    TargetActor->SetActorLocationAndRotation(NewLocation, NewRotation);

    // ���� ���������� ����
    CurrentFrameIndex--;
    if (CurrentFrameIndex < 0) CurrentFrameIndex = 0; // ���� ����


    // ���� �ð� �� �ٽ� ���� 
    GetWorld()->GetTimerManager().SetTimer(
        RewindTimerHandle,
        this,
        &UCSAT_TimeRewind::MoveToNextFrame,
        DurationPerFrame,
        false
    );
}

/** �ǰ��� ���� */
void UCSAT_TimeRewind::FinishRewind()
{
    GetWorld()->GetTimerManager().ClearTimer(RewindTimerHandle);
    UE_LOG(LogTemp, Log, TEXT("Time rewind finished."));
    OnTimeRewindFinished.Broadcast();
    EndTask();
}
