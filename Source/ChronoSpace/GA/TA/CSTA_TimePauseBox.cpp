// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_TimePauseBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ChronoSpace.h"


ACSTA_TimePauseBox::ACSTA_TimePauseBox()
{
    bShowDebug = false;

    // Trigger
    BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_TimePauseBox::OnTriggerBeginOverlap);
    BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_TimePauseBox::OnTriggerEndOverlap);

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialRef(TEXT("/Script/Engine.Material'/Game/Material/MAT_AntyGravity.MAT_AntyGravity'"));

    SetSteticMeshMaterial(MaterialRef.Object, MeshScale.X);
}

void ACSTA_TimePauseBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto Act = ActorsInBoxTrigger.CreateIterator(); Act; ++Act)
    {
        if (!IsValid(Act.Value())) continue;

        ACharacter* RemainedCharacter = Cast<ACharacter>(Act.Value());
        if (RemainedCharacter)
        {
            UCharacterMovementComponent* MovementComp = RemainedCharacter->GetCharacterMovement();
            if (!MovementComp) continue;
            if (MovementComp->GravityScale > 0) continue;

            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;
        }

        ActorsInBoxTrigger.FindAndRemoveChecked(Act.Value()->GetFName());
    }
    Super::EndPlay(EndPlayReason);
}

void ACSTA_TimePauseBox::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);
    SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_TimePauseBox::ConfirmTargetingAndContinue()
{
    //UE_LOG(LogCS, Log, TEXT("ConfirmTargetingAndContinue"));
    if (SourceActor)
    {
        OnComplete.Broadcast();
    }
}

void ACSTA_TimePauseBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

    if (DetectedCharacter)
    {
        UCharacterMovementComponent* MovementComp = DetectedCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            if (MovementComp->GravityScale < 0)
            {
                return;
            }
            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;

            ActorsInBoxTrigger.Emplace(OtherActor->GetFName(), OtherActor);
        }
    }
}

void ACSTA_TimePauseBox::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    //UE_LOG(LogCS, Log, TEXT("OnTriggerEndOverlap called: %s"), *OtherActor->GetName());
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
    if (DetectedCharacter)
    {
        UCharacterMovementComponent* MovementComp = DetectedCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            if (MovementComp->GravityScale > 0)
            {
                return;
            }

            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;

            ActorsInBoxTrigger.FindAndRemoveChecked(OtherActor->GetFName());
        }
    }
}
