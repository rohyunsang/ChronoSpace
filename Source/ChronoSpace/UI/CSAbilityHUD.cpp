// Fill out your copyright notice in the Description page of Project Settings.

#include "CSAbilityHUD.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/CSStageManager.h"

void UCSAbilityHUD::NativeConstruct()
{
    Super::NativeConstruct();

    UCSStageManager* StageManager = Cast<UCSStageManager>(GetGameInstance());
    if (StageManager)
    {
        // 델리게이트가 NULL이 아닌 경우에만 바인딩
        if (!StageManager->OnStageChanged.IsBound())
        {
            StageManager->OnStageChanged.AddDynamic(this, &UCSAbilityHUD::InitializeAbilityUI);
        }
    }

    // 초기 UI 로드
    InitializeAbilityUI();
}

void UCSAbilityHUD::InitializeAbilityUI()
{
    // 스테이지 매니저 가져오기
    UCSStageManager* StageManager = Cast<UCSStageManager>(GetGameInstance());
    if (!StageManager) return;

    // 현재 사용 가능한 어빌리티 가져오기
    AvailableAbilities = StageManager->GetAvailableAbilities();
    SelectedAbilityIndex = 0;

    // 기존 UI 제거
    AbilityContainer->ClearChildren();

    for (int32 i = 0; i < AvailableAbilities.Num(); i++)
    {
        // 위젯 생성
        UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, AbilityWidgetClass);
        if (NewWidget)
        {
            UImage* Icon = Cast<UImage>(NewWidget->GetWidgetFromName(TEXT("AbilityIcon")));
            UTextBlock* Text = Cast<UTextBlock>(NewWidget->GetWidgetFromName(TEXT("AbilityText")));

            if (Icon)
            {
                // 아이콘은 나중에 설정 가능 (예: TMap<FString, UTexture2D*> 이용) 

            }
            if (Text)
            {
                Text->SetText(FText::FromString(AvailableAbilities[i]));
            }

            AbilityContainer->AddChild(NewWidget);
        }
    }
}

// 마우스 휠로 어빌리티 선택
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

// 마우스 왼쪽 버튼으로 어빌리티 실행
void UCSAbilityHUD::UseSelectedAbility()
{
    if (AvailableAbilities.Num() > 0)
    {
        FString SelectedAbility = AvailableAbilities[SelectedAbilityIndex];
        UE_LOG(LogTemp, Warning, TEXT("사용된 어빌리티: %s"), *SelectedAbility);
    }
}