// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CSHPDisplayWidget.generated.h"

class ACSPlayerState;

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSHPDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// UI ������Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category = "HP Display")
	void UpdateHP(float CurrentHP, float MaxHP);

	UFUNCTION(BlueprintCallable, Category = "HP Display")
	void SetPlayerState(ACSPlayerState* PlayerState);

protected:
	// UI ������Ʈ��
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	// ���ε��� �÷��̾� ����
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	TObjectPtr<ACSPlayerState> BoundPlayerState;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Health ��ȭ �ݹ�
	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);
};
