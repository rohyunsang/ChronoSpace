// Fill out your copyright notice in the Description page of Project Settings.


#include "CSTextTrigger.h"
#include "UI/CSTextWidget.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

ACSTextTrigger::ACSTextTrigger()
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

    // 기본 텍스트 데이터 설정
    FCSTextData DefaultText;
    DefaultText.Text = TEXT("Welcome to the game!");
    DefaultText.DisplayDuration = 3.0f;
    DefaultText.TransitionSpeed = 1.0f;
    TextDataArray.Add(DefaultText);

    // 초기화
    bHasTriggered = false;
    CurrentTextIndex = 0;
    bIsDisplayingText = false;
    CurrentTextWidget = nullptr;
    bUseTypewriterEffect = false;
    TypewriterSpeed = 0.05f;
}

void ACSTextTrigger::BeginPlay()
{
    Super::BeginPlay();

    // Overlap 이벤트 바인딩
    if (CollisionBox)
    {
        CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACSTextTrigger::OnOverlapBegin);
        CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACSTextTrigger::OnOverlapEnd);
    }
}

void ACSTextTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어인지 확인
    if (!IsPlayer(OtherActor))
        return;

    // 이미 트리거되었고 한 번만 작동하는 설정이면 무시
    if (bHasTriggered && bTriggerOnce)
        return;

    // 이미 텍스트를 표시 중이면 무시
    if (bIsDisplayingText)
        return;

    // 텍스트 데이터가 없으면 무시
    if (TextDataArray.Num() == 0)
        return;

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Player entered trigger zone"));

    // 텍스트 시퀀스 시작
    StartTextSequence();
}

void ACSTextTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!IsPlayer(OtherActor))
        return;

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Player left trigger zone"));
}

void ACSTextTrigger::StartTextSequence()
{
    if (TextDataArray.Num() == 0)
        return;

    bHasTriggered = true;
    bIsDisplayingText = true;
    CurrentTextIndex = 0;

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Starting text sequence with %d texts"), TextDataArray.Num());

    // 첫 번째 텍스트 표시
    ShowNextText();
}

void ACSTextTrigger::ShowNextText()
{
    // 범위 체크
    if (CurrentTextIndex >= TextDataArray.Num())
    {
        CompleteTextSequence();
        return;
    }

    const FCSTextData& CurrentText = TextDataArray[CurrentTextIndex];

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Showing text %d: %s"), CurrentTextIndex, *CurrentText.Text);

    // 위젯이 없으면 생성
    if (!CurrentTextWidget && TextWidgetClass)
    {
        CurrentTextWidget = CreateWidget<UCSTextWidget>(GetWorld(), TextWidgetClass);
        if (CurrentTextWidget)
        {
            CurrentTextWidget->AddToViewport(100); // 높은 Z-Order로 설정
            UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Created new text widget"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CSTextTrigger: Failed to create text widget"));
            return;
        }
    }

    // 텍스트 표시
    if (CurrentTextWidget)
    {
        if (bUseTypewriterEffect)
        {
            CurrentTextWidget->ShowTextWithTypewriter(CurrentText.Text, TypewriterSpeed);
        }
        else
        {
            CurrentTextWidget->ShowText(CurrentText.Text);
        }
    }

    // 텍스트 표시 시간 후에 숨기기
    GetWorldTimerManager().SetTimer(
        TextDisplayTimer,
        this,
        &ACSTextTrigger::HideCurrentText,
        CurrentText.DisplayDuration,
        false
    );
}

void ACSTextTrigger::HideCurrentText()
{
    // 텍스트 숨기기
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText();
    }

    const FCSTextData& CurrentText = TextDataArray[CurrentTextIndex];
    CurrentTextIndex++;

    // 다음 텍스트가 있으면 전환 시간 후에 표시
    if (CurrentTextIndex < TextDataArray.Num())
    {
        GetWorldTimerManager().SetTimer(
            TextTransitionTimer,
            this,
            &ACSTextTrigger::ShowNextText,
            CurrentText.TransitionSpeed,
            false
        );
    }
    else
    {
        // 모든 텍스트를 표시했으면 시퀀스 완료
        CompleteTextSequence();
    }
}

void ACSTextTrigger::CompleteTextSequence()
{
    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Text sequence completed"));

    bIsDisplayingText = false;
    CurrentTextIndex = 0;

    // 모든 타이머 클리어
    GetWorldTimerManager().ClearTimer(TextDisplayTimer);
    GetWorldTimerManager().ClearTimer(TextTransitionTimer);

    // 위젯 정리
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText();
        // 위젯은 유지하되 숨김 상태로 전환
    }
}

bool ACSTextTrigger::IsPlayer(AActor* Actor)
{
    if (!Actor)
        return false;

    // Character인지 확인 (일반적으로 플레이어는 Character 클래스)
    ACharacter* Character = Cast<ACharacter>(Actor);
    if (!Character)
        return false;

    // 추가적으로 PlayerController가 있는지 확인
    APawn* Pawn = Cast<APawn>(Actor);
    if (Pawn && Pawn->IsPlayerControlled())
        return true;

    return false;
}

void ACSTextTrigger::AddTextData(const FString& Text, float DisplayDuration, float TransitionSpeed)
{
    FCSTextData NewTextData;
    NewTextData.Text = Text;
    NewTextData.DisplayDuration = FMath::Max(0.1f, DisplayDuration);
    NewTextData.TransitionSpeed = FMath::Max(0.1f, TransitionSpeed);

    TextDataArray.Add(NewTextData);

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Added new text data. Total count: %d"), TextDataArray.Num());
}

void ACSTextTrigger::ResetTrigger()
{
    bHasTriggered = false;
    bIsDisplayingText = false;
    CurrentTextIndex = 0;

    // 모든 타이머 클리어
    GetWorldTimerManager().ClearTimer(TextDisplayTimer);
    GetWorldTimerManager().ClearTimer(TextTransitionTimer);

    // 위젯 숨기기
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText(); // 즉시 숨김
    }

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Trigger reset"));
}

void ACSTextTrigger::StopTextSequence()
{
    if (!bIsDisplayingText)
        return;

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Text sequence stopped"));

    bIsDisplayingText = false;
    CurrentTextIndex = 0;

    // 모든 타이머 클리어
    GetWorldTimerManager().ClearTimer(TextDisplayTimer);
    GetWorldTimerManager().ClearTimer(TextTransitionTimer);

    // 위젯 숨기기
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText();
    }
}

void ACSTextTrigger::CreateTextWidget()
{
    if (!CurrentTextWidget && TextWidgetClass)
    {
        CurrentTextWidget = CreateWidget<UCSTextWidget>(GetWorld(), TextWidgetClass);
        if (CurrentTextWidget)
        {
            CurrentTextWidget->AddToViewport(100);
            UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Manual text widget creation successful"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CSTextTrigger: Failed to manually create text widget"));
        }
    }
}

void ACSTextTrigger::DestroyTextWidget()
{
    if (CurrentTextWidget)
    {
        CurrentTextWidget->RemoveFromParent();
        CurrentTextWidget = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Text widget destroyed"));
    }
}