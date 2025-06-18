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

    // UI ������Ʈ��
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MainTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UBorder* TextBackground;

public:
    // �ؽ�Ʈ ���� �� ǥ��
    UFUNCTION(BlueprintCallable, Category = "Text Display")
    void ShowText(const FString& TextToShow);

    // �ؽ�Ʈ �����
    UFUNCTION(BlueprintCallable, Category = "Text Display")
    void HideText();

    // Ÿ���� ȿ���� �ؽ�Ʈ ǥ��
    UFUNCTION(BlueprintCallable, Category = "Text Display")
    void ShowTextWithTypewriter(const FString& TextToShow, float TypeSpeed = 0.05f);

protected:
    // �ؽ�Ʈ ��Ÿ�� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
    FLinearColor TextColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
    int32 FontSize = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
    FLinearColor BackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.7f);

    // Ÿ���� ȿ�� ����
    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    FString FullText;

    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    FString CurrentDisplayText;

    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    int32 CurrentCharIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Typewriter")
    bool bIsTyping = false;

    // Ÿ�̸� �ڵ�
    FTimerHandle TypewriterTimer;

private:
    // Ÿ���� ȿ�� ������Ʈ
    UFUNCTION()
    void UpdateTypewriter();

public:
    // �ؽ�Ʈ ��Ÿ�� ������Ʈ
    UFUNCTION(BlueprintCallable, Category = "Text Style")
    void UpdateTextStyle();

    // ���� �ʱ�ȭ
    UFUNCTION(BlueprintCallable, Category = "Widget")
    void InitializeWidget();

    // ���� ���� Ȯ��
    UFUNCTION(BlueprintPure, Category = "Widget State")
    bool IsTextVisible() const;

    UFUNCTION(BlueprintPure, Category = "Widget State")
    bool IsTyping() const { return bIsTyping; }

    // �̺�Ʈ ���ε�
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
    // �������Ʈ���� �������̵� ������ �̺�Ʈ��
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnTextChanged(const FString& NewText);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnWidgetShown();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnWidgetHidden();
};