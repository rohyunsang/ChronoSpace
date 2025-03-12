// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSAbilityHUD.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UCSAbilityHUD::NativeConstruct()
{
    Super::NativeConstruct();

    //  스테이지 매니저 가져오기
    UCSStageGameInstanceSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UCSStageGameInstanceSubsystem>();
    if (StageSubsystem)
    {
        //  스테이지가 변경될 때 UI를 업데이트하도록 이벤트 바인딩
        if (!StageSubsystem->OnStageChanged.IsBound())
        {
            StageSubsystem->OnStageChanged.AddDynamic(this, &UCSAbilityHUD::InitializeAbilityUI);
        }
    }

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

    //  기존 UI 제거
    AbilityContainer->ClearChildren();

    for (int32 i = 0; i < AvailableAbilities.Num(); i++)
    {
        //  어빌리티 UI 위젯 생성
        UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, AbilityWidgetClass);
        if (NewWidget)
        {
            UImage* Icon = Cast<UImage>(NewWidget->GetWidgetFromName(TEXT("AbilityIcon")));
            UTextBlock* Text = Cast<UTextBlock>(NewWidget->GetWidgetFromName(TEXT("AbilityText")));

            if (Icon)
            {
                //  아이콘을 설정할 수 있도록 (어빌리티 별 아이콘을 매핑해야 함)
                // Icon->SetBrushFromTexture(SomeAbilityIcon);
            }
            if (Text)
            {
                Text->SetText(FText::FromString(AvailableAbilities[i]));
            }

            AbilityContainer->AddChild(NewWidget);
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
