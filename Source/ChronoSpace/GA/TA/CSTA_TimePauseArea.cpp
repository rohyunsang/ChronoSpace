// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_TimePauseArea.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "Physics/CSCollision.h"
#include "ChronoSpace.h"


ACSTA_TimePauseArea::ACSTA_TimePauseArea()
{
	TimePauseTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TimePauseTrigger"));
	RootComponent = TimePauseTrigger;
	TimePauseTrigger->SetBoxExtent(FVector(200.0f, 200.0f, 1000.0f));
	TimePauseTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TimePauseTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	TimePauseTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_TimePauseArea::OnTriggerBeginOverlap);
	TimePauseTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_TimePauseArea::OnTriggerEndOverlap);
}



void ACSTA_TimePauseArea::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto Act = ActorsThatIsPausedTime.CreateIterator(); Act; ++Act)
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

        ActorsThatIsPausedTime.FindAndRemoveChecked(Act.Value()->GetFName());
    }
    Super::EndPlay(EndPlayReason);
}

void ACSTA_TimePauseArea::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_TimePauseArea::ConfirmTargetingAndContinue()
{
	//UE_LOG(LogCS, Log, TEXT("ConfirmTargetingAndContinue"));
	if (SourceActor)
	{
		OnComplete.Broadcast();
	}
}

void ACSTA_TimePauseArea::BeginPlay()
{
    Super::BeginPlay();

    if (TimePauseTrigger)
    {
        FVector BoxLocation = TimePauseTrigger->GetComponentLocation();
        FVector BoxExtent = TimePauseTrigger->GetScaledBoxExtent(); // 스케일이 적용된 박스 크기
        FQuat   BoxRotation = TimePauseTrigger->GetComponentRotation().Quaternion();

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

void ACSTA_TimePauseArea::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
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

            ActorsThatIsPausedTime.Emplace(OtherActor->GetFName(), OtherActor);
        }
    }
}

void ACSTA_TimePauseArea::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

            ActorsThatIsPausedTime.FindAndRemoveChecked(OtherActor->GetFName());
        }
    }
}
