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

    // ���� �������� �̸�
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    FString CurrentStage;

    // �������� ���� �̺�Ʈ (UI ������Ʈ � Ȱ�� ����)
    UPROPERTY(BlueprintAssignable, Category = "Stage")
    FOnStageChanged OnStageChanged;

    // �������� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Stage")
    void ChangeStage(FString NewStage);

    // ���������� ��� ������ �����Ƽ ���
    UPROPERTY()
    TMap<FString, FStageAbilityList> StageAbilities;

    // ���� ������������ ��� ������ �����Ƽ ��������
    UFUNCTION(BlueprintCallable, Category = "Stage")
    TArray<FString> GetAvailableAbilities();

    // �������� �����Ƽ ������ �ʱ�ȭ
    void InitializeStageAbilities();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCSAbilityHUD> AbilityHUDClass;
};
