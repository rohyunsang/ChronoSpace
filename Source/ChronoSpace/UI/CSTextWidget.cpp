// Fill out your copyright notice in the Description page of Project Settings.


#include "CSTextWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "TimerManager.h"
#include "Engine/World.h"

UCSTextWidget::UCSTextWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // �⺻�� ����
    TextColor = FLinearColor::White;
    FontSize = 24;
    BackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.7f);
    CurrentCharIndex = 0;
    bIsTyping = false;
}

void UCSTextWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeWidget();
}

void UCSTextWidget::InitializeWidget()
{
    // �ؽ�Ʈ ��Ÿ�� ����
    UpdateTextStyle();

    // �ʱ⿡�� ����
    SetVisibility(ESlateVisibility::Hidden);

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Widget initialized"));
}

void UCSTextWidget::ShowText(const FString& TextToShow)
{
    if (!MainTextBlock)
    {
        UE_LOG(LogTemp, Error, TEXT("CSTextWidget: MainTextBlock is null"));
        return;
    }

    // Ÿ���� ȿ�� ����
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        bIsTyping = false;
    }

    // �ؽ�Ʈ ����
    MainTextBlock->SetText(FText::FromString(TextToShow));
    FullText = TextToShow;
    CurrentDisplayText = TextToShow;

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Showing text: %s"), *TextToShow);

    // ���� ��� ǥ��
    SetVisibility(ESlateVisibility::Visible);

    // �̺�Ʈ ȣ��
    OnTextShowCompleted.Broadcast();
    OnTextChanged(TextToShow);
    OnWidgetShown();
}

void UCSTextWidget::HideText()
{
    if (!IsVisible())
        return;

    // Ÿ���� ȿ�� ����
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        bIsTyping = false;
    }

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Hiding text"));

    // ��� ����
    SetVisibility(ESlateVisibility::Hidden);

    OnTextHideCompleted.Broadcast();
    OnWidgetHidden();

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Text hidden successfully"));
}

void UCSTextWidget::ShowTextWithTypewriter(const FString& TextToShow, float TypeSpeed)
{
    if (!MainTextBlock)
    {
        UE_LOG(LogTemp, Error, TEXT("CSTextWidget: MainTextBlock is null"));
        return;
    }

    // ���� Ÿ���� ȿ�� ����
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
    }

    // �ʱ�ȭ
    FullText = TextToShow;
    CurrentDisplayText = TEXT("");
    CurrentCharIndex = 0;
    bIsTyping = true;

    // ���� ǥ��
    SetVisibility(ESlateVisibility::Visible);
    MainTextBlock->SetText(FText::FromString(CurrentDisplayText));

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Starting typewriter effect: %s"), *TextToShow);

    // Ÿ���� ȿ�� ����
    GetWorld()->GetTimerManager().SetTimer(
        TypewriterTimer,
        this,
        &UCSTextWidget::UpdateTypewriter,
        TypeSpeed,
        true
    );

    // �������Ʈ �̺�Ʈ ȣ��
    OnWidgetShown();
}

void UCSTextWidget::UpdateTypewriter()
{
    if (CurrentCharIndex >= FullText.Len())
    {
        // Ÿ���� �Ϸ�
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        bIsTyping = false;

        UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Typewriter effect completed"));

        OnTypewriterCompleted.Broadcast();
        OnTextShowCompleted.Broadcast();
        return;
    }

    // ���� ���� �߰�
    CurrentDisplayText += FullText[CurrentCharIndex];
    CurrentCharIndex++;

    // �ؽ�Ʈ ������Ʈ
    if (MainTextBlock)
    {
        MainTextBlock->SetText(FText::FromString(CurrentDisplayText));
        OnTextChanged(CurrentDisplayText);
    }
}

void UCSTextWidget::UpdateTextStyle()
{
    if (!MainTextBlock || !TextBackground)
        return;

    // �ؽ�Ʈ ��Ÿ�� ����
    FSlateFontInfo FontInfo = MainTextBlock->GetFont();
    FontInfo.Size = FontSize;
    MainTextBlock->SetFont(FontInfo);
    MainTextBlock->SetColorAndOpacity(TextColor);

    // ��� ���� ����
    TextBackground->SetBrushColor(BackgroundColor);

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Text style updated"));
}

bool UCSTextWidget::IsTextVisible() const
{
    return GetVisibility() == ESlateVisibility::Visible ||
        GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}