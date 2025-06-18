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

    //  초기 UI 로드
    InitializeAbilityUI();
}

void UCSAbilityHUD::InitializeAbilityUI()
{
    // 스테이지 서브시스템 가져오기
    UCSStageGameInstanceSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UCSStageGameInstanceSubsystem>();
    if (!StageSubsystem) return;

    //  현재 사용 가능한 어빌리티 가져오기
    AvailableAbilities = StageSubsystem->GetAvailableAbilities();
    SelectedAbilityIndex = 0;

    // 기존 UI 제거
    AbilityContainer->ClearChildren();

    // 어빌리티 목록을 UI에 추가
    for (int32 i = 0; i < AvailableAbilities.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("AvailableAbilities: %s"), *AvailableAbilities[i]);

        // 새로운 TextBlock 생성
        UTextBlock* NewText = NewObject<UTextBlock>(this);
        if (NewText)
        {
            // 텍스트 설정
            NewText->SetText(FText::FromString(AvailableAbilities[i]));

            // 텍스트 스타일 (폰트 크기 조정)
            FSlateFontInfo FontInfo = NewText->Font;
            FontInfo.Size = 24;  // 폰트 크기 설정
            NewText->SetFont(FontInfo);

            // AbilityContainer (VerticalBox)에 추가
            AbilityContainer->AddChild(NewText);
        }
    }
}

//  마우스 휠로 어빌리티 선택 변경
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

//  마우스 클릭으로 어빌리티 실행
void UCSAbilityHUD::UseSelectedAbility()
{
    if (AvailableAbilities.Num() > 0)
    {
        FString SelectedAbility = AvailableAbilities[SelectedAbilityIndex];
        UE_LOG(LogTemp, Warning, TEXT("사용된 어빌리티: %s"), *SelectedAbility);

        //  어빌리티 실행 로직 추가 가능 (예: AbilitySystem 적용)
    }
}
