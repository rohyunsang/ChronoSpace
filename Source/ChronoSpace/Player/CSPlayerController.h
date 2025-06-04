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

	// UI ���� Ŭ���� - Blueprint���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCSGameUIWidget> GameUIWidgetClass;

	// ���� UI ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UCSGameUIWidget> GameUIWidget;

	// UI ���� �Լ���
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideGameUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleGameUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshGameUI();

	// ���� UI ���� Ȯ��
	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsGameUIVisible() const;

	/*
	// UI ���� ���� ���� (Blueprint��)
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

	// PlayerState ��ȭ ����
	virtual void OnRep_PlayerState() override;

private:
	

	void SetupInputMode();
	void CreateGameUI();
	void InitializeUI();

	// UI ���� ���� Ÿ�̸�
	FTimerHandle UICreationTimerHandle;

};
