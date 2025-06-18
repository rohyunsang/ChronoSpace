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
	// HP UI ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCSHPDisplayWidget> HPDisplayWidgetClass;

	// UI ������Ʈ �Լ���
	UFUNCTION(BlueprintCallable, Category = "Game UI")
	void RefreshPlayerUI();

protected:
	// UI �����̳�
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> PlayersContainer;

	// HP ���÷��� ������
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TArray<TObjectPtr<UCSHPDisplayWidget>> HPDisplayWidgets;

	// ���� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TObjectPtr<ACSGameState> GameState;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// �÷��̾� ������Ʈ �ݹ�
	UFUNCTION()
	void OnPlayersUpdated(const TArray<ACSPlayerState*>& PlayerStates);

private:
	void CreateHPDisplayWidgets(const TArray<ACSPlayerState*>& PlayerStates);
	void ClearHPDisplayWidgets();
};
