// Fill out your copyright notice in the Description page of Project Settings.


#include "CSTextWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "TimerManager.h"
#include "Engine/World.h"

UCSTextWidget::UCSTextWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 기본값 설정
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
    // 텍스트 스타일 적용
    UpdateTextStyle();

    // 초기에는 숨김
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

    // 타이핑 효과 중지
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        bIsTyping = false;
    }

    // 텍스트 설정
    MainTextBlock->SetText(FText::FromString(TextToShow));
    FullText = TextToShow;
    CurrentDisplayText = TextToShow;

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Showing text: %s"), *TextToShow);

    // 위젯 즉시 표시
    SetVisibility(ESlateVisibility::Visible);

    // 이벤트 호출
    OnTextShowCompleted.Broadcast();
    OnTextChanged(TextToShow);
    OnWidgetShown();
}

void UCSTextWidget::HideText()
{
    if (!IsVisible())
        return;

    // 타이핑 효과 중지
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        bIsTyping = false;
    }

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Hiding text"));

    // 즉시 숨김
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

    // 기존 타이핑 효과 중지
    if (bIsTyping)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
    }

    // 초기화
    FullText = TextToShow;
    CurrentDisplayText = TEXT("");
    CurrentCharIndex = 0;
    bIsTyping = true;

    // 위젯 표시
    SetVisibility(ESlateVisibility::Visible);
    MainTextBlock->SetText(FText::FromString(CurrentDisplayText));

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Starting typewriter effect: %s"), *TextToShow);

    // 타이핑 효과 시작
    GetWorld()->GetTimerManager().SetTimer(
        TypewriterTimer,
        this,
        &UCSTextWidget::UpdateTypewriter,
        TypeSpeed,
        true
    );

    // 블루프린트 이벤트 호출
    OnWidgetShown();
}

void UCSTextWidget::UpdateTypewriter()
{
    if (CurrentCharIndex >= FullText.Len())
    {
        // 타이핑 완료
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        bIsTyping = false;

        UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Typewriter effect completed"));

        OnTypewriterCompleted.Broadcast();
        OnTextShowCompleted.Broadcast();
        return;
    }

    // 다음 문자 추가
    CurrentDisplayText += FullText[CurrentCharIndex];
    CurrentCharIndex++;

    // 텍스트 업데이트
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

    // 텍스트 스타일 적용
    FSlateFontInfo FontInfo = MainTextBlock->GetFont();
    FontInfo.Size = FontSize;
    MainTextBlock->SetFont(FontInfo);
    MainTextBlock->SetColorAndOpacity(TextColor);

    // 배경 색상 적용
    TextBackground->SetBrushColor(BackgroundColor);

    UE_LOG(LogTemp, Warning, TEXT("CSTextWidget: Text style updated"));
}

bool UCSTextWidget::IsTextVisible() const
{
    return GetVisibility() == ESlateVisibility::Visible ||
        GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}