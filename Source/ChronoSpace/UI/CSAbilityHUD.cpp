// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSAbilityHUD.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/CSStageGameInstanceSubsystem.h"

void UCSAbilityHUD::NativeConstruct()
{
    Super::NativeConstruct();

    //  �ʱ� UI �ε�
    InitializeAbilityUI();
}

void UCSAbilityHUD::InitializeAbilityUI()
{
    // �������� ����ý��� ��������
    UCSStageGameInstanceSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UCSStageGameInstanceSubsystem>();
    if (!StageSubsystem) return;

    //  ���� ��� ������ �����Ƽ ��������
    AvailableAbilities = StageSubsystem->GetAvailableAbilities();
    SelectedAbilityIndex = 0;

    // ���� UI ����
    AbilityContainer->ClearChildren();

    // �����Ƽ ����� UI�� �߰�
    for (int32 i = 0; i < AvailableAbilities.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("AvailableAbilities: %s"), *AvailableAbilities[i]);

        // ���ο� TextBlock ����
        UTextBlock* NewText = NewObject<UTextBlock>(this);
        if (NewText)
        {
            // �ؽ�Ʈ ����
            NewText->SetText(FText::FromString(AvailableAbilities[i]));

            // �ؽ�Ʈ ��Ÿ�� (��Ʈ ũ�� ����)
            FSlateFontInfo FontInfo = NewText->Font;
            FontInfo.Size = 24;  // ��Ʈ ũ�� ����
            NewText->SetFont(FontInfo);

            // AbilityContainer (VerticalBox)�� �߰�
            AbilityContainer->AddChild(NewText);
        }
    }
}

//  ���콺 �ٷ� �����Ƽ ���� ����
void UCSAbilityHUD::ChangeAbilitySelection(float AxisValue)
{
    if (AvailableAbilities.Num() == 0) return;

    if (AxisValue > 0)
    {
        SelectedAbilityIndex = (SelectedAbilityIndex + 1) % AvailableAbilities.Num();
    }
    else if (AxisValue < 0)
    {
        SelectedAbilityIndex = (SelectedAbilityIndex - 1 + AvailableAbilities.Num()) % AvailableAbilities.Num();
    }
}

//  ���콺 Ŭ������ �����Ƽ ����
void UCSAbilityHUD::UseSelectedAbility()
{
    if (AvailableAbilities.Num() > 0)
    {
        FString SelectedAbility = AvailableAbilities[SelectedAbilityIndex];
        UE_LOG(LogTemp, Warning, TEXT("���� �����Ƽ: %s"), *SelectedAbility);

        //  �����Ƽ ���� ���� �߰� ���� (��: AbilitySystem ����)
    }
}
