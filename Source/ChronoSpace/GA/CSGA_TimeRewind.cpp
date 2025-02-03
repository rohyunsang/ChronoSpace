#include "GA/CSGA_TimeRewind.h"
#include "GA/AT/CSAT_TimeRewind.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"  // GameplayTag 사용을 위한 헤더 추가
#include "Abilities/GameplayAbility.h"  // BlockAbilitiesWithTag 사용을 위해 필요
#include "Character/CSCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"  // 중력 조절을 위해 필요 

UCSGA_TimeRewind::UCSGA_TimeRewind()
{
    UE_LOG(LogTemp, Log, TEXT("GA Active Constructor"));

    // 태그 설정
    FGameplayTagContainer AbilityTagsContainer;
    AbilityTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.TimeRewind")));
    SetAssetTags(AbilityTagsContainer);

    BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Movement")));
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

    // 입력 비활성화
    if (PlayerCharacter->GetController())
    {
        APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
        if (PC)
        {
            PC->DisableInput(PC);
            UE_LOG(LogTemp, Log, TEXT("Player input disabled during Time Rewind."));
        }
    }

    // 중력 비활성화 (중력을 0으로 설정)
    UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->GravityScale = 0.0f;
        MovementComp->StopMovementImmediately(); // 현재 속도 정지
        UE_LOG(LogTemp, Log, TEXT("Gravity disabled during Time Rewind."));
    }

    // Ability Task 생성 (0.5초 동안 되감기)
    TArray<FCSF_CharacterFrameData> RewindFrames;
    for (int32 i = PlayerCharacter->TransformHistory.Num() - 300; i < PlayerCharacter->TransformHistory.Num(); i++)
    {
        RewindFrames.Add(PlayerCharacter->TransformHistory[i]);
    }

    UCSAT_TimeRewind* TimeRewindTask = UCSAT_TimeRewind::CreateTimeRewindTask(
        this,
        PlayerCharacter,
        RewindFrames,
        0.5f
    );

    TimeRewindTask->OnTimeRewindFinished.AddDynamic(this, &UCSGA_TimeRewind::OnTimeRewindFinishedDelegate);
    TimeRewindTask->ReadyForActivation();
}

void UCSGA_TimeRewind::OnTimeRewindFinishedDelegate()
{
    UE_LOG(LogTemp, Log, TEXT("Time rewind finished. Ending ability."));

    ACSCharacterPlayer* PlayerCharacter = Cast<ACSCharacterPlayer>(CurrentActorInfo->AvatarActor.Get());
    if (PlayerCharacter && PlayerCharacter->GetController())
    {
        APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
        if (PC)
        {
            PC->EnableInput(PC);
            UE_LOG(LogTemp, Log, TEXT("Player input re-enabled after Time Rewind."));
        }
    }
    
    // 중력 복구 (중력 값을 원래대로)
    UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->GravityScale = 1.0f;
        UE_LOG(LogTemp, Log, TEXT("Gravity re-enabled after Time Rewind."));
    }



    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}