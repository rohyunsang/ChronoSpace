// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSLevelTransferTrigger.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ACSLevelTransferTrigger::ACSLevelTransferTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root Component 설정
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Collision Box 생성
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Visual Mesh 생성 (선택적)
    TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
    TriggerMesh->SetupAttachment(CollisionBox);
    TriggerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 기본값 설정
    TargetLevelName = TEXT("Level2");
    SpawnPointTag = TEXT("");
    bTriggerOnce = true;
    TransferDelay = 0.0f;
    bHasTriggered = false;
}

void ACSLevelTransferTrigger::BeginPlay()
{
    Super::BeginPlay();

    // Overlap 이벤트 바인딩
    if (CollisionBox)
    {
        CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACSLevelTransferTrigger::OnOverlapBegin);
        CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACSLevelTransferTrigger::OnOverlapEnd);
    }

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Initialized with target level: %s"), *TargetLevelName);
}

void ACSLevelTransferTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어인지 확인
    if (!IsPlayer(OtherActor))
        return;

    // 서버에서만 처리
    if (!HasAuthority())
        return;

    // 이미 트리거되었고 한 번만 작동하는 설정이면 무시
    if (bHasTriggered && bTriggerOnce)
        return;

    // 목표 레벨 이름 체크
    if (TargetLevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("CSLevelTransferTrigger: Target level name is empty"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Player entered trigger zone - transferring to %s"), *TargetLevelName);

    // 트리거 상태 업데이트
    bHasTriggered = true;

    // 이벤트 브로드캐스트 (모든 클라이언트에서 실행되도록 하려면 Multicast RPC 사용)
    OnLevelTransferStarted.Broadcast(TargetLevelName);

    // 지연 시간이 있으면 타이머 사용, 없으면 즉시 실행
    if (TransferDelay > 0.0f)
    {
        FTimerHandle DelayTimer;
        GetWorldTimerManager().SetTimer(
            DelayTimer,
            this,
            &ACSLevelTransferTrigger::ExecuteLevelTransfer,
            TransferDelay,
            false
        );
    }
    else
    {
        ExecuteLevelTransfer();
    }
}

void ACSLevelTransferTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!IsPlayer(OtherActor))
        return;

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Player left trigger zone"));
}

void ACSLevelTransferTrigger::ExecuteLevelTransfer()
{
    if (TargetLevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("CSLevelTransferTrigger: Cannot execute transfer - target level name is empty"));
        return;
    }

    // 멀티플레이어에서는 서버에서만 레벨 전환을 처리해야 함
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Not server, ignoring level transfer"));
        return;
    }

    // 스폰 포인트 태그가 있으면 옵션에 추가
    FString Options = TEXT("");
    if (!SpawnPointTag.IsEmpty())
    {
        Options = FString::Printf(TEXT("?SpawnTag=%s"), *SpawnPointTag);
    }

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Server executing level transfer to: %s%s"), *TargetLevelName, *Options);

    // 멀티플레이어를 위한 서버 트래블 사용
    UWorld* World = GetWorld();
    if (World)
    {
        // bAbsolute를 false로 설정하여 모든 클라이언트가 함께 이동
        World->ServerTravel(TargetLevelName + Options, false);
    }
}

bool ACSLevelTransferTrigger::IsPlayer(AActor* Actor)
{
    if (!Actor)
        return false;

    // Character인지 확인
    ACharacter* Character = Cast<ACharacter>(Actor);
    if (!Character)
        return false;

    // PlayerController가 있는지 확인
    APawn* Pawn = Cast<APawn>(Actor);
    if (Pawn && Pawn->IsPlayerControlled())
        return true;

    return false;
}

void ACSLevelTransferTrigger::ResetTrigger()
{
    bHasTriggered = false;

    // 모든 타이머 클리어
    GetWorldTimerManager().ClearAllTimersForObject(this);

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Trigger reset"));
}

void ACSLevelTransferTrigger::ForceLevelTransfer()
{
    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Force level transfer triggered"));

    bHasTriggered = true;
    OnLevelTransferStarted.Broadcast(TargetLevelName);
    ExecuteLevelTransfer();
}
