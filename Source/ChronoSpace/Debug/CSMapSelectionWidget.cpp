// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/CSMapSelectionWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


void UCSMapSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MapButton1) MapButton1->OnClicked.AddDynamic(this, &UCSMapSelectionWidget::OnMap1Clicked);
    if (MapButton2) MapButton2->OnClicked.AddDynamic(this, &UCSMapSelectionWidget::OnMap2Clicked);
    if (MapButton3) MapButton3->OnClicked.AddDynamic(this, &UCSMapSelectionWidget::OnMap3Clicked);
}

void UCSMapSelectionWidget::OnMap1Clicked() { TravelToMap(Map1Name); }
void UCSMapSelectionWidget::OnMap2Clicked() { TravelToMap(Map2Name); }
void UCSMapSelectionWidget::OnMap3Clicked() { TravelToMap(Map3Name); }


void UCSMapSelectionWidget::TravelToMap(FName MapName)
{
    if (MapName.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("CSMapSelectionWidget: Cannot execute transfer - map name is empty"));
        return;
    }

    // 멀티플레이어에서는 서버에서만 레벨 전환을 처리해야 함
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("CSMapSelectionWidget: World is null"));
        return;
    }

    // 서버 권한 체크 (UserWidget에서는 GetAuthGameMode로 체크)
    if (!World->GetAuthGameMode())
    {
        UE_LOG(LogTemp, Warning, TEXT("CSMapSelectionWidget: Not server, ignoring level transfer"));
        return;
    }

    // 맵 경로 생성
    FString MapPath = FString::Printf(TEXT("/Game/Maps/%s"), *MapName.ToString());

    UE_LOG(LogTemp, Warning, TEXT("CSMapSelectionWidget: Server executing level transfer to: %s"), *MapPath);

    // 멀티플레이어를 위한 서버 트래블 사용
    // bAbsolute를 false로 설정하여 모든 클라이언트가 함께 이동
    World->ServerTravel(MapPath, false);
}