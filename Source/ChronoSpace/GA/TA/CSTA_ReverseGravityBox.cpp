// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "Physics/CSCollision.h"
#include "ChronoSpace.h"

ACSTA_ReverseGravityBox::ACSTA_ReverseGravityBox()
{
	ReverseGravityTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ReverseGravityTrigger"));
	ReverseGravityTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	ReverseGravityTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ReverseGravityTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	ReverseGravityTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerBeginOverlap);
}

void ACSTA_ReverseGravityBox::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_ReverseGravityBox::ConfirmTargetingAndContinue()
{
	UE_LOG(LogCS, Log, TEXT("ConfirmTargetingAndContinue"));
	if (SourceActor)
	{
		OnComplete.Broadcast();
	}
}

void ACSTA_ReverseGravityBox::BeginPlay()
{
    Super::BeginPlay();

    if (ReverseGravityTrigger)
    {
        FVector BoxLocation = ReverseGravityTrigger->GetComponentLocation();
        FVector BoxExtent = ReverseGravityTrigger->GetScaledBoxExtent(); // 스케일이 적용된 박스 크기
        FQuat   BoxRotation = ReverseGravityTrigger->GetComponentRotation().Quaternion();

        DrawDebugBox(
            GetWorld(),
            BoxLocation,
            BoxExtent,
            BoxRotation,
            FColor::Green,
            false,  // 지속 표시
            DurtionTime, // -1은 시간 제한 없음
            0,     // 디버그 선 우선순위
            2.0f   // 선 두께
        );
    }
}

void ACSTA_ReverseGravityBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UE_LOG(LogCS, Log, TEXT("OnTriggerBeginOverlap: %s is detected"), *OtherActor->GetName());
    //APawn* DetectedPawn = Cast<APawn>(OtherActor);
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

    /*if (DetectedPawn)
    {
        UE_LOG(LogCS, Log, TEXT("%s is Pawn"), *OtherActor->GetName());
    }*/
    if (DetectedCharacter)
    {
        UE_LOG(LogCS, Log, TEXT("%s is Character"), *OtherActor->GetName());

        UCharacterMovementComponent* MovementComp = DetectedCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            if (MovementComp->GravityScale < 0)
            {
                return;
            }
            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 1.0f));
            MovementComp->GravityScale *= -1.0f;
            UE_LOG(LogCS, Log, TEXT("Gravity scale changed to: %f"), MovementComp->GravityScale);
        }
    }
}
