#include "GA/CSGA_TimeRewind.h"
#include "GA/AT/CSAT_TimeRewind.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"  // GameplayTag ����� ���� ��� �߰�
#include "Abilities/GameplayAbility.h"  // BlockAbilitiesWithTag ����� ���� �ʿ�
#include "Character/CSCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"  // �߷� ������ ���� �ʿ� 

UCSGA_TimeRewind::UCSGA_TimeRewind()
{
    UE_LOG(LogTemp, Log, TEXT("GA Active Constructor"));

    // �±� ����
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
    if (!PlayerCharacter || PlayerCharacter->TransformHistory.Num() < 99)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough Transform History Available"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // �Է� ��Ȱ��ȭ
    if (PlayerCharacter->GetController())
    {
        APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
        if (PC)
        {
            PC->DisableInput(PC);
            UE_LOG(LogTemp, Log, TEXT("Player input disabled during Time Rewind."));
        }
    }

    // �߷� ��Ȱ��ȭ (�߷��� 0���� ����)
    UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->GravityScale = 0.0f;
        MovementComp->StopMovementImmediately(); // ���� �ӵ� ����
        UE_LOG(LogTemp, Log, TEXT("Gravity disabled during Time Rewind."));
    }

    // Ability Task ���� (0.5�� ���� �ǰ���)
    TArray<FCSF_CharacterFrameData> RewindFrames;
    for (int32 i = PlayerCharacter->TransformHistory.Num() - 99; i < PlayerCharacter->TransformHistory.Num(); i++)
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
    
    // �߷� ���� (�߷� ���� �������)
    UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->GravityScale = 1.0f;
        UE_LOG(LogTemp, Log, TEXT("Gravity re-enabled after Time Rewind."));
    }



    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}