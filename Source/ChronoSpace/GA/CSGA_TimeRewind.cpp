#include "GA/CSGA_TimeRewind.h"
#include "GA/AT/CSAT_TimeRewind.h"
#include "AbilitySystemComponent.h"
#include "Character/CSCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"

UCSGA_TimeRewind::UCSGA_TimeRewind()
{
    UE_LOG(LogTemp, Log, TEXT("GA Active Constructor"));
}

void UCSGA_TimeRewind::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Log, TEXT("GA Active"));

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    AActor* AvatarActor = ActorInfo->AvatarActor.Get();
    if (!AvatarActor)
    {
        UE_LOG(LogTemp, Error, TEXT("AvatarActor is null! Ending ability."));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    ACSCharacterPlayer* PlayerCharacter = Cast<ACSCharacterPlayer>(AvatarActor);
    if (!PlayerCharacter || PlayerCharacter->TransformHistory.Num() < 300)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough Transform History Available"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // 최근 300개 프레임을 가져와서 사용
    TArray<FCSF_CharacterFrameData> RewindFrames;
    for (int32 i = PlayerCharacter->TransformHistory.Num() - 300; i < PlayerCharacter->TransformHistory.Num(); i++)
    {
        RewindFrames.Add(PlayerCharacter->TransformHistory[i]);
    }

    UE_LOG(LogTemp, Log, TEXT("Rewinding through %d frames"), RewindFrames.Num());

    // Ability Task 생성 (1초 동안 되감기)
    UCSAT_TimeRewind* TimeRewindTask = UCSAT_TimeRewind::CreateTimeRewindTask(
        this,
        PlayerCharacter,
        RewindFrames,
        1.0f
    );

    TimeRewindTask->OnTimeRewindFinished.AddDynamic(this, &UCSGA_TimeRewind::OnTimeRewindFinishedDelegate);
    TimeRewindTask->ReadyForActivation();
}

void UCSGA_TimeRewind::OnTimeRewindFinishedDelegate()
{
    UE_LOG(LogTemp, Log, TEXT("Time rewind finished. Ending ability."));
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
