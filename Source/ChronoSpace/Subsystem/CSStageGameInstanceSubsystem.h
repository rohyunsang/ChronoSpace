// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/CSAbilityHUD.h"
#include "CSStageGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageChanged);

USTRUCT(BlueprintType)
struct FStageAbilityList
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    TArray<FString> Abilities;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CHRONOSPACE_API UCSStageGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    UCSStageGameInstanceSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // 현재 스테이지 이름
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    FString CurrentStage;

    // 스테이지 변경 이벤트 (UI 업데이트 등에 활용 가능)
    UPROPERTY(BlueprintAssignable, Category = "Stage")
    FOnStageChanged OnStageChanged;

    // 스테이지 변경 함수
    UFUNCTION(BlueprintCallable, Category = "Stage")
    void ChangeStage(FString NewStage);

    // 스테이지별 사용 가능한 어빌리티 목록
    UPROPERTY()
    TMap<FString, FStageAbilityList> StageAbilities;

    // 현재 스테이지에서 사용 가능한 어빌리티 가져오기
    UFUNCTION(BlueprintCallable, Category = "Stage")
    TArray<FString> GetAvailableAbilities();

    // 스테이지 어빌리티 데이터 초기화
    void InitializeStageAbilities();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCSAbilityHUD> AbilityHUDClass;
};
