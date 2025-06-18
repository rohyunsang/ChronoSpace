// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSHPDisplayWidget.h"
#include "Player/CSPlayerState.h"



void UCSHPDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCSHPDisplayWidget::NativeDestruct()
{
	// ��������Ʈ ����ε�
	if (BoundPlayerState)
	{
		BoundPlayerState->OnHealthChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UCSHPDisplayWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	if (HealthBar)
	{
		float HealthPercent = MaxHP > 0 ? CurrentHP / MaxHP : 0.0f;
		HealthBar->SetPercent(HealthPercent);
	}

	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%.0f/%.0f"), CurrentHP, MaxHP);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

void UCSHPDisplayWidget::SetPlayerState(ACSPlayerState* PlayerState)
{
	// ���� ��������Ʈ ����ε�
	if (BoundPlayerState)
	{
		BoundPlayerState->OnHealthChanged.RemoveAll(this);
	}

	BoundPlayerState = PlayerState;

	if (BoundPlayerState)
	{
		// �� ��������Ʈ ���ε�
		BoundPlayerState->OnHealthChanged.AddDynamic(this, &UCSHPDisplayWidget::OnHealthChanged);

		// �÷��̾� �̸� ����
		if (PlayerNameText)
		{
			FString PlayerName = BoundPlayerState->GetPlayerName();
			if (PlayerName.IsEmpty())
			{
				PlayerName = TEXT("Player");
			}
			PlayerNameText->SetText(FText::FromString(PlayerName));
		}

		// �ʱ� HP �� ����
		UpdateHP(BoundPlayerState->GetHealth(), BoundPlayerState->GetMaxHealth());
	}
}

void UCSHPDisplayWidget::OnHealthChanged(float Health, float MaxHealth)
{
	UpdateHP(Health, MaxHealth);
}