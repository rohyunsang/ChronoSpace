// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "CSTextWidget.generated.h"


/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UCSTextWidget(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;

    // UI 컴포넌트들
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MainTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UBorder* TextBackground;

public:
    // 텍스트 설정 및 표시
    UFUNCTION(BlueprintCallable, Category = "Text Display")
    void ShowText(const FString& TextToShow);

    // 텍스트 숨기기
    UFUNCTION(BlueprintCallable, Category = "Text Display")
    void HideText();

    // 타이핑 효과로 텍스트 표시
    UFUNCTION(BlueprintCallable, Category = "Text Display")
    void ShowTextWithTypewriter(const FString& TextToShow, float TypeSpeed = 0.05f);

protected:
    // 텍스트 스타일 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
    FLinearColor TextColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
    int32 FontSize = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
    FLinearColor BackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.7f);

    // 타이핑 효과 관련
    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    FString FullText;

    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    FString CurrentDisplayText;

    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    int32 CurrentCharIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    bool bIsTyping = false;

    // 타이머 핸들
    FTimerHandle TypewriterTimer;

private:
    // 타이핑 효과 업데이트
    UFUNCTION()
    void UpdateTypewriter();

public:
    // 텍스트 스타일 업데이트
    UFUNCTION(BlueprintCallable, Category = "Text Style")
    void UpdateTextStyle();

    // 위젯 초기화
    UFUNCTION(BlueprintCallable, Category = "Widget")
    void InitializeWidget();

    // 현재 상태 확인
    UFUNCTION(BlueprintPure, Category = "Widget State")
    bool IsTextVisible() const;

    UFUNCTION(BlueprintPure, Category = "Widget State")
    bool IsTyping() const { return bIsTyping; }

    // 이벤트 바인딩
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTextShowCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTextHideCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTypewriterCompleted);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTextShowCompleted OnTextShowCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTextHideCompleted OnTextHideCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTypewriterCompleted OnTypewriterCompleted;

protected:
    // 블루프린트에서 오버라이드 가능한 이벤트들
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnTextChanged(const FString& NewText);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnWidgetShown();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnWidgetHidden();
};