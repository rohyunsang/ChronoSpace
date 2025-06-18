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

    // Root Component ����
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Collision Box ����
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Visual Mesh ���� (������)
    TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
    TriggerMesh->SetupAttachment(CollisionBox);
    TriggerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // �⺻�� ����
    TargetLevelName = TEXT("Level2");
    SpawnPointTag = TEXT("");
    bTriggerOnce = true;
    TransferDelay = 0.0f;
    bHasTriggered = false;
}

void ACSLevelTransferTrigger::BeginPlay()
{
    Super::BeginPlay();

    // Overlap �̺�Ʈ ���ε�
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
    // �÷��̾����� Ȯ��
    if (!IsPlayer(OtherActor))
        return;

    // ���������� ó��
    if (!HasAuthority())
        return;

    // �̹� Ʈ���ŵǾ��� �� ���� �۵��ϴ� �����̸� ����
    if (bHasTriggered && bTriggerOnce)
        return;

    // ��ǥ ���� �̸� üũ
    if (TargetLevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("CSLevelTransferTrigger: Target level name is empty"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Player entered trigger zone - transferring to %s"), *TargetLevelName);

    // Ʈ���� ���� ������Ʈ
    bHasTriggered = true;

    // �̺�Ʈ ��ε�ĳ��Ʈ (��� Ŭ���̾�Ʈ���� ����ǵ��� �Ϸ��� Multicast RPC ���)
    OnLevelTransferStarted.Broadcast(TargetLevelName);

    // ���� �ð��� ������ Ÿ�̸� ���, ������ ��� ����
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

    // ��Ƽ�÷��̾���� ���������� ���� ��ȯ�� ó���ؾ� ��
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Not server, ignoring level transfer"));
        return;
    }

    // ���� ����Ʈ �±װ� ������ �ɼǿ� �߰�
    FString Options = TEXT("");
    if (!SpawnPointTag.IsEmpty())
    {
        Options = FString::Printf(TEXT("?SpawnTag=%s"), *SpawnPointTag);
    }

    UE_LOG(LogTemp, Warning, TEXT("CSLevelTransferTrigger: Server executing level transfer to: %s%s"), *TargetLevelName, *Options);

    // ��Ƽ�÷��̾ ���� ���� Ʈ���� ���
    UWorld* World = GetWorld();
    if (World)
    {
        // bAbsolute�� false�� �����Ͽ� ��� Ŭ���̾�Ʈ�� �Բ� �̵�
        World->ServerTravel(TargetLevelName + Options, false);
    }
}

bool ACSLevelTransferTrigger::IsPlayer(AActor* Actor)
{
    if (!Actor)
        return false;

    // Character���� Ȯ��
    ACharacter* Character = Cast<ACharacter>(Actor);
    if (!Character)
        return false;

    // PlayerController�� �ִ��� Ȯ��
    APawn* Pawn = Cast<APawn>(Actor);
    if (Pawn && Pawn->IsPlayerControlled())
        return true;

    return false;
}

void ACSLevelTransferTrigger::ResetTrigger()
{
    bHasTriggered = false;

    // ��� Ÿ�̸� Ŭ����
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
