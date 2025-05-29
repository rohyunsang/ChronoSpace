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

    // �⺻�� ����
    CurrentScaleType = ECharacterScaleType::Normal;
    CurrentScaleValue = 1.0f;
    NormalScale = 1.0f;
    LargeScale = 1.5f;
    SmallScale = 0.5f;
    ScaleTransitionSpeed = 2.0f;

    // Ʈ������ ���� �ʱ�ȭ
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

    // �ʱ� ������ ����
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

    // �������� ���� ���̸� ���� ó��
    if (GetOwner()->HasAuthority())
    {
        ServerChangeScale(NewScaleType);
    }
    else
    {
        // Ŭ���̾�Ʈ�� ���� RPC ȣ��
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

    // �ε巯�� Ʈ������ ����
    StartScale = GetCurrentScaleValue();
    TargetScale = CurrentScaleValue;
    ElapsedTime = 0.0f;
    TransitionTime = FMath::Abs(TargetScale - StartScale) / ScaleTransitionSpeed;
    bIsTransitioning = true;

    // ��� Ŭ���̾�Ʈ���� ������ ���� ����
    MulticastApplyScale(NewScaleType, CurrentScaleValue);

    // �̺�Ʈ ��ε�ĳ��Ʈ
    OnCharacterScaleChanged.Broadcast(OldScaleType, NewScaleType);
}

bool UCSCharacterScaleComponent::ServerChangeScale_Validate(ECharacterScaleType NewScaleType)
{
    // ? �ùٸ� ����: Small(0) ~ Large(2)
    return NewScaleType >= ECharacterScaleType::Small && NewScaleType <= ECharacterScaleType::Large;
}

void UCSCharacterScaleComponent::MulticastApplyScale_Implementation(ECharacterScaleType NewScaleType, float NewScale)
{
    // Ʈ������ ����
    StartScale = GetOwner() ? Cast<ACharacter>(GetOwner())->GetActorScale3D().X : 1.0f;
    TargetScale = NewScale;
    ElapsedTime = 0.0f;
    TransitionTime = FMath::Abs(TargetScale - StartScale) / ScaleTransitionSpeed;
    bIsTransitioning = true;

    // Ÿ�̸� ����
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
    // ������ Ÿ���� ����Ǿ��� �� ó��
    OnCharacterScaleChanged.Broadcast(OldScaleType, CurrentScaleType);
}

void UCSCharacterScaleComponent::OnRep_CurrentScaleValue()
{
    // ������ ���� ����Ǿ��� �� ĳ���Ϳ� ����
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

    // ���������� ĸ�� ������Ʈ ũ�� ����
    if (UCapsuleComponent* CapsuleComp = Character->GetCapsuleComponent())
    {
        // ���� ũ�� �������� �����ϸ�
        float BaseRadius = 34.0f; // �⺻ ĳ���� ĸ�� ������
        float BaseHalfHeight = 88.0f; // �⺻ ĳ���� ĸ�� ���� ����

        CapsuleComp->SetCapsuleSize(BaseRadius * NewScale, BaseHalfHeight * NewScale);
    }

    // ����� �α�
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

    // ���� �Ϸ� Ȯ��
    if (Alpha >= 1.0f)
    {
        bIsTransitioning = false;
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(ScaleTransitionTimer);
        }
    }
}