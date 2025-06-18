// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_CharacterScale.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "ActorComponent/CSCharacterScaleComponent.h"

UCSGA_CharacterScale::UCSGA_CharacterScale()
{
    // �����Ƽ �⺻ ����
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

    // ������ ȣ�� -> �������Ʈ ȣ�� �̶� �����س��� �ȴ�. 
    TargetScaleType = ECharacterScaleType::Large;

    // �⺻�� ����

    NormalScale = 1.0f;
    LargeScale = 1.5f;
    SmallScale = 0.5f;
    ScaleTransitionSpeed = 2.0f;

    // Gameplay Tags �ʱ�ȭ
    ScaleNormalTag = FGameplayTag::RequestGameplayTag(FName("Character.Scale.Normal"));
    ScaleLargeTag = FGameplayTag::RequestGameplayTag(FName("Character.Scale.Large"));
    ScaleSmallTag = FGameplayTag::RequestGameplayTag(FName("Character.Scale.Small"));
}

void UCSGA_CharacterScale::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UE_LOG(LogTemp, Log, TEXT("CharacterScaleLog ! Activate Ability "));
    UE_LOG(LogTemp, Warning, TEXT("CharacterScaleLog Final TargetScaleType: %d"), (int32)TargetScaleType);

    // ������ ������Ʈ ã��
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Character)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UCSCharacterScaleComponent* ScaleComponent = Character->FindComponentByClass<UCSCharacterScaleComponent>();
    if (!ScaleComponent)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // ������ ��ǥ �����Ϸ� ����
    ScaleComponent->RequestScaleChange(TargetScaleType);

    // �����Ƽ ��� ���� (���� ������ ������ ������Ʈ���� ó��)
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCSGA_CharacterScale::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // Ÿ�̸� ����
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ScaleTransitionTimer);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCSGA_CharacterScale::ChangeCharacterScale(ECharacterScaleType NewScaleType)
{
    if (CurrentScaleType == NewScaleType)
    {
        return;
    }

    // ���ο� ������ Ÿ�� ����
    CurrentScaleType = NewScaleType;

    // Ÿ�� ������ ���
    switch (NewScaleType)
    {
    case ECharacterScaleType::Normal:
        TargetScale = NormalScale;
        break;
    case ECharacterScaleType::Large:
        TargetScale = LargeScale;
        break;
    case ECharacterScaleType::Small:
        TargetScale = SmallScale;
        break;
    }

    // ���� �����Ͽ��� �����Ͽ� ���� ����
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (Character)
    {
        StartScale = Character->GetActorScale3D().X;
        ElapsedTime = 0.0f;
        TransitionTime = FMath::Abs(TargetScale - StartScale) / ScaleTransitionSpeed;

        // Ÿ�̸� ����
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(
                ScaleTransitionTimer,
                this,
                &UCSGA_CharacterScale::UpdateScaleTransition,
                0.016f, // 60 FPS
                true
            );
        }
    }
}

void UCSGA_CharacterScale::UpdateScaleTransition()
{
    ElapsedTime += 0.016f;
    float Alpha = FMath::Clamp(ElapsedTime / TransitionTime, 0.0f, 1.0f);

    // Smooth interpolation
    Alpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);

    float CurrentScale = FMath::Lerp(StartScale, TargetScale, Alpha);
    ApplyScale(CurrentScale);

    // ���� �Ϸ� Ȯ��
    if (Alpha >= 1.0f)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(ScaleTransitionTimer);
        }
    }
}

void UCSGA_CharacterScale::ApplyScale(float NewScale)
{
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Character)
    {
        return;
    }

    FVector NewScaleVector(NewScale, NewScale, NewScale);

    // ĳ���� ������ ����
    Character->SetActorScale3D(NewScaleVector);

    // ĸ�� ������Ʈ ũ�⵵ ���� (������)
    if (UCapsuleComponent* CapsuleComp = Character->GetCapsuleComponent())
    {
        float BaseRadius = 34.0f; // �⺻ ĳ���� ĸ�� ������
        float BaseHalfHeight = 88.0f; // �⺻ ĳ���� ĸ�� ���� ����

        CapsuleComp->SetCapsuleSize(BaseRadius * NewScale, BaseHalfHeight * NewScale);
    }
}