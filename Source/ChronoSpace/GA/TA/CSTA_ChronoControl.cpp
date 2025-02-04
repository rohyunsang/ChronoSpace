// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_ChronoControl.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Character/CSCharacterPlayer.h"
#include "ChronoSpace.h"


ACSTA_ChronoControl::ACSTA_ChronoControl()
{
    bShowDebug = false;

    // Trigger
    BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_ChronoControl::OnTriggerBeginOverlap);
    BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_ChronoControl::OnTriggerEndOverlap);

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialRef(TEXT("/Script/Engine.Material'/Game/Material/MAT_ChronoControl.MAT_ChronoControl'"));

    SetSteticMeshMaterial(MaterialRef.Object, MeshScale.X);
}

void ACSTA_ChronoControl::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto Act = ActorsInBoxTrigger.CreateIterator(); Act; ++Act)
    {
        if (!IsValid(Act.Value())) continue;

        ACharacter* RemainedCharacter = Cast<ACharacter>(Act.Value());
        if (RemainedCharacter)
        {
            // 시간 복원
            if (RemainedCharacter->CustomTimeDilation != 1.0f)
            {
                RemainedCharacter->CustomTimeDilation = 1.0f;
                UE_LOG(LogTemp, Log, TEXT("CustomTimeDilation reset to 1.0 for actor: %s"), *RemainedCharacter->GetName());
            }
        }

        ActorsInBoxTrigger.FindAndRemoveChecked(Act.Value()->GetFName());
    }
    Super::EndPlay(EndPlayReason);
}

void ACSTA_ChronoControl::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);
    SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_ChronoControl::ConfirmTargetingAndContinue()
{
    //UE_LOG(LogCS, Log, TEXT("ConfirmTargetingAndContinue"));
    if (SourceActor)
    {
        OnComplete.Broadcast();
    }
}

void ACSTA_ChronoControl::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{

    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
    ACSCharacterPlayer* DetectedCharacterPlayer = Cast<ACSCharacterPlayer>(OtherActor);

    if (DetectedCharacterPlayer)
    {
        // 시간 정지의 영향을 받지 않도록 제외
        return;
    }

    if (DetectedCharacter)
    {
        // 시간 진행 멈추기
        if (DetectedCharacter->CustomTimeDilation == 0.0f)
        {
            // 이미 멈춘 상태라면 다시 처리하지 않음
            return;
        }
        DetectedCharacter->CustomTimeDilation = 0.0f;

        ActorsInBoxTrigger.Emplace(OtherActor->GetFName(), OtherActor);
    } 
}

void ACSTA_ChronoControl::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
    if (DetectedCharacter)
    {
        // 시간 복원
        if (DetectedCharacter->CustomTimeDilation == 1.0f)
        {
            // 이미 시간이 정상 상태라면 다시 처리하지 않음
            return;
        }

        DetectedCharacter->CustomTimeDilation = 1.0f;

        ActorsInBoxTrigger.FindAndRemoveChecked(OtherActor->GetFName());
    }
}
