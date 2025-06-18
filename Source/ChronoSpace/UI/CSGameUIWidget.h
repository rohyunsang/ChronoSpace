// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSGameUIWidget.generated.h"

class UCSHPDisplayWidget;
class ACSGameState;
class ACSPlayerState;

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGameUIWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// HP UI 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCSHPDisplayWidget> HPDisplayWidgetClass;

	// UI 업데이트 함수들
	UFUNCTION(BlueprintCallable, Category = "Game UI")
	void RefreshPlayerUI();

protected:
	// UI 컨테이너
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> PlayersContainer;

	// HP 디스플레이 위젯들
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TArray<TObjectPtr<UCSHPDisplayWidget>> HPDisplayWidgets;

	// 게임 상태 참조
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TObjectPtr<ACSGameState> GameState;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 플레이어 업데이트 콜백
	UFUNCTION()
	void OnPlayersUpdated(const TArray<ACSPlayerState*>& PlayerStates);

private:
	void CreateHPDisplayWidgets(const TArray<ACSPlayerState*>& PlayerStates);
	void ClearHPDisplayWidgets();
};
