// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSHPDisplayWidget.h"
#include "Player/CSPlayerState.h"



void UCSHPDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCSHPDisplayWidget::NativeDestruct()
{
	// 델리게이트 언바인딩
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
	// 기존 델리게이트 언바인딩
	if (BoundPlayerState)
	{
		BoundPlayerState->OnHealthChanged.RemoveAll(this);
	}

	BoundPlayerState = PlayerState;

	if (BoundPlayerState)
	{
		// 새 델리게이트 바인딩
		BoundPlayerState->OnHealthChanged.AddDynamic(this, &UCSHPDisplayWidget::OnHealthChanged);

		// 플레이어 이름 설정
		if (PlayerNameText)
		{
			FString PlayerName = BoundPlayerState->GetPlayerName();
			if (PlayerName.IsEmpty())
			{
				PlayerName = TEXT("Player");
			}
			PlayerNameText->SetText(FText::FromString(PlayerName));
		}

		// 초기 HP 값 설정
		UpdateHP(BoundPlayerState->GetHealth(), BoundPlayerState->GetMaxHealth());
	}
}

void UCSHPDisplayWidget::OnHealthChanged(float Health, float MaxHealth)
{
	UpdateHP(Health, MaxHealth);
}