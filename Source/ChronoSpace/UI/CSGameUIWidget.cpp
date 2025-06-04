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

	// ���� ���� ���� ȹ��
	if (UWorld* World = GetWorld())
	{
		GameState = World->GetGameState<ACSGameState>();
		if (GameState)
		{
			// �÷��̾� ������Ʈ ��������Ʈ ���ε�
			GameState->OnPlayersUpdated.AddDynamic(this, &UCSGameUIWidget::OnPlayersUpdated);

			// �ʱ� UI ����
			RefreshPlayerUI();
		}
	}
}

void UCSGameUIWidget::NativeDestruct()
{
	// ��������Ʈ ����ε�
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
			// HP ���÷��� ���� ����
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