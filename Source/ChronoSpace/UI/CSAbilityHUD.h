// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CSAbilityHUD.generated.h"


/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAbilityHUD : public UUserWidget
{
    GENERATED_BODY()  // GENERATED_BODY() �߰� �� �ϸ� UUserWidget ����� ����� �� ����.

public:
    // **�����Ƽ UI ����Ʈ�� ǥ���� �����̳� (Blueprint���� ���� �ʿ�)**
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* AbilityContainer;

    // **�����Ƽ UI ��� (�̹��� + �ؽ�Ʈ)**
    UPROPERTY(EditDefaultsOnly, Category="AbilityUI")
    TSubclassOf<UUserWidget> AbilityWidgetClass;

    // ���� ���õ� �����Ƽ �ε���
    UPROPERTY(BlueprintReadWrite, Category="AbilityUI")
    int32 SelectedAbilityIndex;

    // ���� ��� ������ �����Ƽ ���
    UPROPERTY(BlueprintReadWrite, Category="AbilityUI")
    TArray<FString> AvailableAbilities;

    // UI �ʱ�ȭ �Լ� (�������� ���� �� ȣ���)
    UFUNCTION(BlueprintCallable, Category="AbilityUI")
    void InitializeAbilityUI();

protected:
    virtual void NativeConstruct() override;

private:
    // ���콺 �� �Է� ó��
    UFUNCTION()
    void ChangeAbilitySelection(float AxisValue);

    // �����Ƽ ����
    UFUNCTION()
    void UseSelectedAbility();
	
};
