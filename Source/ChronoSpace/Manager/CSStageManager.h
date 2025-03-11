// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CSStageManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageChanged);


/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSStageManager : public UGameInstance
{
    GENERATED_BODY()

public:
    // 기본 생성자
    UCSStageManager();

    // 현재 스테이지 (L_Main, L_Stage1 등)
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    FString CurrentStage;

    // 스테이지별 사용 가능한 어빌리티 목록 
    // 에러뜨지만 사용가능. UPROPERTY() 없어야함. 
    TMap<FString, TArray<FString>> StageAbilities;

    // 스테이지 변경 시 호출되는 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Stage")
    FOnStageChanged OnStageChanged;

    // 스테이지 변경 함수
    UFUNCTION(BlueprintCallable, Category = "Stage")
    void ChangeStage(FString NewStage);

    // 현재 스테이지에서 사용 가능한 어빌리티 가져오기
    UFUNCTION(BlueprintCallable, Category = "Stage")
    TArray<FString> GetAvailableAbilities();

    // 초기 데이터 설정
    UFUNCTION(BlueprintCallable, Category = "Stage")
    void InitializeStageAbilities();
};
