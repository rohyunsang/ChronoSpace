// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CSCharacterScaleComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"



// Sets default values for this component's properties
UCSCharacterScaleComponent::UCSCharacterScaleComponent()
{
    // Set this component to be ticked every frame.  You can turn this off to improve performance if not needed.
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    // 기본값 설정
    CurrentScaleType = ECharacterScaleType::Normal;
    CurrentScaleValue = 1.0f;
    NormalScale = 1.0f;
    LargeScale = 1.5f;
    SmallScale = 0.5f;
    ScaleTransitionSpeed = 2.0f;

    // 트랜지션 관련 초기화
    StartScale = 1.0f;
    TargetScale = 1.0f;
    TransitionTime = 0.0f;
    ElapsedTime = 0.0f;
    bIsTransitioning = false;
}

// Called when the game starts
void UCSCharacterScaleComponent::BeginPlay()
{
    Super::BeginPlay();

    // 초기 스케일 적용
    ApplyScaleToCharacter(CurrentScaleValue);
}

void UCSCharacterScaleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCSCharacterScaleComponent, CurrentScaleType);
    DOREPLIFETIME(UCSCharacterScaleComponent, CurrentScaleValue);
}

// Called every frame
void UCSCharacterScaleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UCSCharacterScaleComponent::RequestScaleChange(ECharacterScaleType NewScaleType)
{
    UE_LOG(LogTemp, Log, TEXT("CharacterScaleLog ! RequestScaleChange "));
    if (NewScaleType == CurrentScaleType)
    {
        return;
    }

    // 서버에서 실행 중이면 직접 처리
    if (GetOwner()->HasAuthority())
    {
        ServerChangeScale(NewScaleType);
    }
    else
    {
        // 클라이언트면 서버 RPC 호출
        ServerChangeScale(NewScaleType);
    }
}

float UCSCharacterScaleComponent::GetCurrentScaleValue() const
{
    return CurrentScaleValue;
}

void UCSCharacterScaleComponent::ServerChangeScale_Implementation(ECharacterScaleType NewScaleType)
{
    if (NewScaleType == CurrentScaleType)
    {
        return;
    }

    ECharacterScaleType OldScaleType = CurrentScaleType;
    CurrentScaleType = NewScaleType;
    CurrentScaleValue = GetScaleValueFromType(NewScaleType);

    // 부드러운 트랜지션 시작
    StartScale = GetCurrentScaleValue();
    TargetScale = CurrentScaleValue;
    ElapsedTime = 0.0f;
    TransitionTime = FMath::Abs(TargetScale - StartScale) / ScaleTransitionSpeed;
    bIsTransitioning = true;

    // 모든 클라이언트에게 스케일 변경 적용
    MulticastApplyScale(NewScaleType, CurrentScaleValue);

    // 이벤트 브로드캐스트
    OnCharacterScaleChanged.Broadcast(OldScaleType, NewScaleType);
}

bool UCSCharacterScaleComponent::ServerChangeScale_Validate(ECharacterScaleType NewScaleType)
{
    // ? 올바른 범위: Small(0) ~ Large(2)
    return NewScaleType >= ECharacterScaleType::Small && NewScaleType <= ECharacterScaleType::Large;
}

void UCSCharacterScaleComponent::MulticastApplyScale_Implementation(ECharacterScaleType NewScaleType, float NewScale)
{
    // 트랜지션 설정
    StartScale = GetOwner() ? Cast<ACharacter>(GetOwner())->GetActorScale3D().X : 1.0f;
    TargetScale = NewScale;
    ElapsedTime = 0.0f;
    TransitionTime = FMath::Abs(TargetScale - StartScale) / ScaleTransitionSpeed;
    bIsTransitioning = true;

    // 타이머 시작
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            ScaleTransitionTimer,
            this,
            &UCSCharacterScaleComponent::UpdateScaleTransition,
            0.016f, // 60 FPS
            true
        );
    }
}

void UCSCharacterScaleComponent::OnRep_CurrentScaleType(ECharacterScaleType OldScaleType)
{
    // 스케일 타입이 변경되었을 때 처리
    OnCharacterScaleChanged.Broadcast(OldScaleType, CurrentScaleType);
}

void UCSCharacterScaleComponent::OnRep_CurrentScaleValue()
{
    // 스케일 값이 변경되었을 때 캐릭터에 적용
    ApplyScaleToCharacter(CurrentScaleValue);
}

void UCSCharacterScaleComponent::ApplyScaleToCharacter(float NewScale)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    FVector NewScaleVector(NewScale, NewScale, NewScale);
    Character->SetActorScale3D(NewScaleVector);

    // 선택적으로 캡슐 컴포넌트 크기 조정
    if (UCapsuleComponent* CapsuleComp = Character->GetCapsuleComponent())
    {
        // 원본 크기 기준으로 스케일링
        float BaseRadius = 34.0f; // 기본 캐릭터 캡슐 반지름
        float BaseHalfHeight = 88.0f; // 기본 캐릭터 캡슐 절반 높이

        CapsuleComp->SetCapsuleSize(BaseRadius * NewScale, BaseHalfHeight * NewScale);
    }

    // 디버그 로그
    if (GEngine)
    {
        FString ScaleTypeName;
        switch (CurrentScaleType)
        {
        case ECharacterScaleType::Normal:
            ScaleTypeName = TEXT("Normal");
            break;
        case ECharacterScaleType::Large:
            ScaleTypeName = TEXT("Large");
            break;
        case ECharacterScaleType::Small:
            ScaleTypeName = TEXT("Small");
            break;
        }

        GEngine->AddOnScreenDebugMessage(
            -1, 3.0f, FColor::Green,
            FString::Printf(TEXT("Character Scale Changed to %s (%.2f)"), *ScaleTypeName, NewScale)
        );
    }
}

float UCSCharacterScaleComponent::GetScaleValueFromType(ECharacterScaleType ScaleType) const
{
    switch (ScaleType)
    {
    case ECharacterScaleType::Normal:
        return NormalScale;
    case ECharacterScaleType::Large:
        return LargeScale;
    case ECharacterScaleType::Small:
        return SmallScale;
    default:
        return NormalScale;
    }
}

void UCSCharacterScaleComponent::UpdateScaleTransition()
{
    if (!bIsTransitioning)
    {
        GetWorld()->GetTimerManager().ClearTimer(ScaleTransitionTimer);
        return;
    }

    ElapsedTime += 0.016f;
    float Alpha = FMath::Clamp(ElapsedTime / TransitionTime, 0.0f, 1.0f);

    // Smooth interpolation
    Alpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);

    float CurrentScale = FMath::Lerp(StartScale, TargetScale, Alpha);
    ApplyScaleToCharacter(CurrentScale);

    // 보간 완료 확인
    if (Alpha >= 1.0f)
    {
        bIsTransitioning = false;
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(ScaleTransitionTimer);
        }
    }
}