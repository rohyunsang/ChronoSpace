// Fill out your copyright notice in the Description page of Project Settings.


#include "CSStageManager.h"
#include "Kismet/GameplayStatics.h"

UCSStageManager::UCSStageManager()
{
    CurrentStage = "L_Main"; // 기본값 설정
}

void UCSStageManager::ChangeStage(FString NewStage)
{
    CurrentStage = NewStage;

    UGameplayStatics::OpenLevel(GetWorld(), *NewStage);

    OnStageChanged.Broadcast(); // 스테이지 변경 시 UI 업데이트 호출
}

TArray<FString> UCSStageManager::GetAvailableAbilities()
{
    if (StageAbilities.Contains(CurrentStage))
    {
        return StageAbilities[CurrentStage];
    }
    return TArray<FString>();
}

// 초기 데이터 설정
void UCSStageManager::InitializeStageAbilities()
{
    // 예제 데이터 추가
    StageAbilities.Add("L_Stage1", { "ReverseGravity", "TimeStop" });
    StageAbilities.Add("L_Stage2", { "TimeStop" });

    OnStageChanged.Broadcast(); // 초기 설정 후 UI 업데이트
}