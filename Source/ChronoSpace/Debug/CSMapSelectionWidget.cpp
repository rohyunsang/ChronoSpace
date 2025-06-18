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

    // ��Ƽ�÷��̾���� ���������� ���� ��ȯ�� ó���ؾ� ��
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("CSMapSelectionWidget: World is null"));
        return;
    }

    // ���� ���� üũ (UserWidget������ GetAuthGameMode�� üũ)
    if (!World->GetAuthGameMode())
    {
        UE_LOG(LogTemp, Warning, TEXT("CSMapSelectionWidget: Not server, ignoring level transfer"));
        return;
    }

    // �� ��� ����
    FString MapPath = FString::Printf(TEXT("/Game/Maps/%s"), *MapName.ToString());

    UE_LOG(LogTemp, Warning, TEXT("CSMapSelectionWidget: Server executing level transfer to: %s"), *MapPath);

    // ��Ƽ�÷��̾ ���� ���� Ʈ���� ���
    // bAbsolute�� false�� �����Ͽ� ��� Ŭ���̾�Ʈ�� �Բ� �̵�
    World->ServerTravel(MapPath, false);
}