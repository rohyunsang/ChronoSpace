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

    // �⺻ �ؽ�Ʈ ������ ����
    FCSTextData DefaultText;
    DefaultText.Text = TEXT("Welcome to the game!");
    DefaultText.DisplayDuration = 3.0f;
    DefaultText.TransitionSpeed = 1.0f;
    TextDataArray.Add(DefaultText);

    // �ʱ�ȭ
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

    // Overlap �̺�Ʈ ���ε�
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
    // �÷��̾����� Ȯ��
    if (!IsPlayer(OtherActor))
        return;

    // �̹� Ʈ���ŵǾ��� �� ���� �۵��ϴ� �����̸� ����
    if (bHasTriggered && bTriggerOnce)
        return;

    // �̹� �ؽ�Ʈ�� ǥ�� ���̸� ����
    if (bIsDisplayingText)
        return;

    // �ؽ�Ʈ �����Ͱ� ������ ����
    if (TextDataArray.Num() == 0)
        return;

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Player entered trigger zone"));

    // �ؽ�Ʈ ������ ����
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

    // ù ��° �ؽ�Ʈ ǥ��
    ShowNextText();
}

void ACSTextTrigger::ShowNextText()
{
    // ���� üũ
    if (CurrentTextIndex >= TextDataArray.Num())
    {
        CompleteTextSequence();
        return;
    }

    const FCSTextData& CurrentText = TextDataArray[CurrentTextIndex];

    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Showing text %d: %s"), CurrentTextIndex, *CurrentText.Text);

    // ������ ������ ����
    if (!CurrentTextWidget && TextWidgetClass)
    {
        CurrentTextWidget = CreateWidget<UCSTextWidget>(GetWorld(), TextWidgetClass);
        if (CurrentTextWidget)
        {
            CurrentTextWidget->AddToViewport(100); // ���� Z-Order�� ����
            UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Created new text widget"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CSTextTrigger: Failed to create text widget"));
            return;
        }
    }

    // �ؽ�Ʈ ǥ��
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

    // �ؽ�Ʈ ǥ�� �ð� �Ŀ� �����
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
    // �ؽ�Ʈ �����
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText();
    }

    const FCSTextData& CurrentText = TextDataArray[CurrentTextIndex];
    CurrentTextIndex++;

    // ���� �ؽ�Ʈ�� ������ ��ȯ �ð� �Ŀ� ǥ��
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
        // ��� �ؽ�Ʈ�� ǥ�������� ������ �Ϸ�
        CompleteTextSequence();
    }
}

void ACSTextTrigger::CompleteTextSequence()
{
    UE_LOG(LogTemp, Warning, TEXT("CSTextTrigger: Text sequence completed"));

    bIsDisplayingText = false;
    CurrentTextIndex = 0;

    // ��� Ÿ�̸� Ŭ����
    GetWorldTimerManager().ClearTimer(TextDisplayTimer);
    GetWorldTimerManager().ClearTimer(TextTransitionTimer);

    // ���� ����
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText();
        // ������ �����ϵ� ���� ���·� ��ȯ
    }
}

bool ACSTextTrigger::IsPlayer(AActor* Actor)
{
    if (!Actor)
        return false;

    // Character���� Ȯ�� (�Ϲ������� �÷��̾�� Character Ŭ����)
    ACharacter* Character = Cast<ACharacter>(Actor);
    if (!Character)
        return false;

    // �߰������� PlayerController�� �ִ��� Ȯ��
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

    // ��� Ÿ�̸� Ŭ����
    GetWorldTimerManager().ClearTimer(TextDisplayTimer);
    GetWorldTimerManager().ClearTimer(TextTransitionTimer);

    // ���� �����
    if (CurrentTextWidget)
    {
        CurrentTextWidget->HideText(); // ��� ����
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

    // ��� Ÿ�̸� Ŭ����
    GetWorldTimerManager().ClearTimer(TextDisplayTimer);
    GetWorldTimerManager().ClearTimer(TextTransitionTimer);

    // ���� �����
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