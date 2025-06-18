// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSPlayerController.generated.h"

class UCSGameUIWidget;

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACSPlayerController();

	// UI 위젯 클래스 - Blueprint에서 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCSGameUIWidget> GameUIWidgetClass;

	// 게임 UI 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UCSGameUIWidget> GameUIWidget;

	// UI 관리 함수들
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideGameUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleGameUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshGameUI();

	// 현재 UI 상태 확인
	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsGameUIVisible() const;

	/*
	// UI 위젯 직접 접근 (Blueprint용)
	UFUNCTION(BlueprintPure, Category = "UI")
	UCSGameUIWidget* GetGameUIWidget() const { return GameUIWidget; }
	*/

	void ShakeCamera();

protected:
	UPROPERTY()
	TSubclassOf<class UCameraShakeBase> CameraShake;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// PlayerState 변화 감지
	virtual void OnRep_PlayerState() override;

private:
	

	void SetupInputMode();
	void CreateGameUI();
	void InitializeUI();

	// UI 생성 지연 타이머
	FTimerHandle UICreationTimerHandle;

};
