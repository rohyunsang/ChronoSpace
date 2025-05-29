// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_CharacterScale.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "ActorComponent/CSCharacterScaleComponent.h"

UCSGA_CharacterScale::UCSGA_CharacterScale()
{
    // 어빌리티 기본 설정
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

    // 생성자 호출 -> 블루프린트 호출 이라 설정해놔도 된다. 
    TargetScaleType = ECharacterScaleType::Large;

    // 기본값 설정

    NormalScale = 1.0f;
    LargeScale = 1.5f;
    SmallScale = 0.5f;
    ScaleTransitionSpeed = 2.0f;

    // Gameplay Tags 초기화
    ScaleNormalTag = FGameplayTag::RequestGameplayTag(FName("Character.Scale.Normal"));
    ScaleLargeTag = FGameplayTag::RequestGameplayTag(FName("Character.Scale.Large"));
    ScaleSmallTag = FGameplayTag::RequestGameplayTag(FName("Character.Scale.Small"));
}

void UCSGA_CharacterScale::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UE_LOG(LogTemp, Log, TEXT("CharacterScaleLog ! Activate Ability "));
    UE_LOG(LogTemp, Warning, TEXT("CharacterScaleLog Final TargetScaleType: %d"), (int32)TargetScaleType);

    // 스케일 컴포넌트 찾기
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

    // 설정된 목표 스케일로 변경
    ScaleComponent->RequestScaleChange(TargetScaleType);

    // 어빌리티 즉시 종료 (실제 스케일 변경은 컴포넌트에서 처리)
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCSGA_CharacterScale::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // 타이머 정리
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

    // 새로운 스케일 타입 설정
    CurrentScaleType = NewScaleType;

    // 타겟 스케일 계산
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

    // 현재 스케일에서 시작하여 보간 시작
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (Character)
    {
        StartScale = Character->GetActorScale3D().X;
        ElapsedTime = 0.0f;
        TransitionTime = FMath::Abs(TargetScale - StartScale) / ScaleTransitionSpeed;

        // 타이머 시작
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

    // 보간 완료 확인
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

    // 캐릭터 스케일 적용
    Character->SetActorScale3D(NewScaleVector);

    // 캡슐 컴포넌트 크기도 조정 (선택적)
    if (UCapsuleComponent* CapsuleComp = Character->GetCapsuleComponent())
    {
        float BaseRadius = 34.0f; // 기본 캐릭터 캡슐 반지름
        float BaseHalfHeight = 88.0f; // 기본 캐릭터 캡슐 절반 높이

        CapsuleComp->SetCapsuleSize(BaseRadius * NewScale, BaseHalfHeight * NewScale);
    }
}