// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSGameUIWidget.h"
#include "UI/CSHPDisplayWidget.h"
#include "Game/CSGameState.h"
#include "Player/CSPlayerState.h"
#include "Components/PanelWidget.h"
#include "Engine/World.h"

void UCSGameUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 게임 상태 참조 획득
	if (UWorld* World = GetWorld())
	{
		GameState = World->GetGameState<ACSGameState>();
		if (GameState)
		{
			// 플레이어 업데이트 델리게이트 바인딩
			GameState->OnPlayersUpdated.AddDynamic(this, &UCSGameUIWidget::OnPlayersUpdated);

			// 초기 UI 설정
			RefreshPlayerUI();
		}
	}
}

void UCSGameUIWidget::NativeDestruct()
{
	// 델리게이트 언바인딩
	if (GameState)
	{
		GameState->OnPlayersUpdated.RemoveAll(this);
	}

	ClearHPDisplayWidgets();

	Super::NativeDestruct();
}

void UCSGameUIWidget::RefreshPlayerUI()
{
	if (GameState)
	{
		TArray<ACSPlayerState*> PlayerStates = GameState->GetAllMyPlayerStates();
		OnPlayersUpdated(PlayerStates);
	}
}

void UCSGameUIWidget::OnPlayersUpdated(const TArray<ACSPlayerState*>& PlayerStates)
{
	ClearHPDisplayWidgets();
	CreateHPDisplayWidgets(PlayerStates);
}

void UCSGameUIWidget::CreateHPDisplayWidgets(const TArray<ACSPlayerState*>& PlayerStates)
{
	if (!HPDisplayWidgetClass || !PlayersContainer)
	{
		return;
	}

	for (ACSPlayerState* PlayerState : PlayerStates)
	{
		if (PlayerState)
		{
			// HP 디스플레이 위젯 생성
			UCSHPDisplayWidget* HPWidget = CreateWidget<UCSHPDisplayWidget>(this, HPDisplayWidgetClass);
			if (HPWidget)
			{
				HPWidget->SetPlayerState(PlayerState);
				PlayersContainer->AddChild(HPWidget);
				HPDisplayWidgets.Add(HPWidget);
			}
		}
	}
}

void UCSGameUIWidget::ClearHPDisplayWidgets()
{
	for (UCSHPDisplayWidget* HPWidget : HPDisplayWidgets)
	{
		if (HPWidget)
		{
			HPWidget->RemoveFromParent();
		}
	}
	HPDisplayWidgets.Empty();

	if (PlayersContainer)
	{
		PlayersContainer->ClearChildren();
	}
}