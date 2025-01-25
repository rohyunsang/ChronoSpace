// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_WeakenGravityBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ChronoSpace.h"


ACSTA_WeakenGravityBox::ACSTA_WeakenGravityBox()
{
    bShowDebug = false;

    // Trigger
    BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_WeakenGravityBox::OnTriggerBeginOverlap);
    BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_WeakenGravityBox::OnTriggerEndOverlap);

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialRef(TEXT("/Script/Engine.Material'/Game/Material/MAT_AntyGravity.MAT_AntyGravity'"));
    SetSteticMeshMaterial(MaterialRef.Object, MeshScale.X);

    
}

void ACSTA_WeakenGravityBox::BeginPlay()
{
    SaturationSetting();
}

void ACSTA_WeakenGravityBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if ( GravityCoef <= 0.0f || GravityCoef >= 1.0f )
    {
        UE_LOG(LogCS, Error, TEXT("GravityCoef Size Error")); 
        return;
    }

    for (auto Act = ActorsInBoxTrigger.CreateIterator(); Act; ++Act)
    {
        if (!IsValid(Act.Value())) continue;

        ACharacter* RemainedCharacter = Cast<ACharacter>(Act.Value());
        if (RemainedCharacter)
        {
            UCharacterMovementComponent* MovementComp = RemainedCharacter->GetCharacterMovement();
            if (!MovementComp) continue;

            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale /= GravityCoef;
        }

        ActorsInBoxTrigger.Remove(Act.Value()->GetFName());
    }
    Super::EndPlay(EndPlayReason);
}

void ACSTA_WeakenGravityBox::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);
    SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_WeakenGravityBox::ConfirmTargetingAndContinue()
{
    if (SourceActor)
    {
        OnComplete.Broadcast();
    }
}

void ACSTA_WeakenGravityBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    if (GravityCoef <= 0.0f || GravityCoef >= 1.0f)
    {
        UE_LOG(LogCS, Error, TEXT("GravityCoef Size Error"));
        return;
    }

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
            MovementComp->GravityScale *= GravityCoef;

            ActorsInBoxTrigger.Emplace(OtherActor->GetFName(), OtherActor);
        }
    }
}

void ACSTA_WeakenGravityBox::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (GravityCoef <= 0.0f || GravityCoef >= 1.0f)
    {
        UE_LOG(LogCS, Error, TEXT("GravityCoef Size Error"));
        return;
    }

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
            MovementComp->GravityScale /= GravityCoef;

            ActorsInBoxTrigger.Remove(OtherActor->GetFName());
        }
    }
}

void ACSTA_WeakenGravityBox::SaturationSetting()
{
    UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(StaticMeshComp->GetMaterial(0));
    FLinearColor OrgColor;
    DynMaterial->GetVectorParameterValue(FName(TEXT("Color")), OrgColor);

    // r:h g:s b:v a:a
    FLinearColor HSVColor = OrgColor.LinearRGBToHSV();
    HSVColor.G *= GravityCoef;

    DynMaterial->SetVectorParameterValue(FName(TEXT("Color")), HSVColor.HSVToLinearRGB());
}
