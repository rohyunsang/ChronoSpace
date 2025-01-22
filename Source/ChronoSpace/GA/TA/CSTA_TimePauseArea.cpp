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


void ACSTA_TimePauseArea::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto Act = ActorsThatIsPausedTime.CreateIterator(); Act; ++Act)
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

            // 기존 GravityScale 처리 부분 제거
            // 시간 복원만 수행하고 중복된 논리는 제거했습니다.
        }

        // 목록에서 제거
        ActorsThatIsPausedTime.FindAndRemoveChecked(Act.Value()->GetFName());
    }

    // 부모 클래스의 EndPlay 호출
    Super::EndPlay(EndPlayReason);
}

void ACSTA_TimePauseArea::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

    if (DetectedCharacter)
    {
        // 시간 진행 멈추기
        if (DetectedCharacter->CustomTimeDilation == 0.0f)
        {
            // 이미 멈춘 상태라면 다시 처리하지 않음
            return;
        }
        DetectedCharacter->CustomTimeDilation = 0.0f;

        // 배우를 멈춘 목록에 추가
        ActorsThatIsPausedTime.Emplace(OtherActor->GetFName(), OtherActor);
    }
}

void ACSTA_TimePauseArea::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
    if (DetectedCharacter)
    {
        // 시간 복원
        if (DetectedCharacter->CustomTimeDilation <= 1.001f && DetectedCharacter->CustomTimeDilation >= 0.999f)
        {
            // 이미 시간이 정상 상태라면 다시 처리하지 않음
            return;
        }

        DetectedCharacter->CustomTimeDilation = 1.0f;

        // 멈춘 시간 목록에서 제거
        if (ActorsThatIsPausedTime.Contains(OtherActor->GetFName()))
        {
            ActorsThatIsPausedTime.FindAndRemoveChecked(OtherActor->GetFName());
        }

        // 디버그 로그 출력
        UE_LOG(LogTemp, Log, TEXT("Time resumed for actor: %s"), *OtherActor->GetName());
    }
}
