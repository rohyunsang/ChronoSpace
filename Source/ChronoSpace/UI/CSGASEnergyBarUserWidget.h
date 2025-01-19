// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CSGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "CSGASEnergyBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGASEnergyBarUserWidget : public UCSGASUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnEnergyChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxEnergyChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateEnergyBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbEnergyBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtEnergyStat;

	float CurrentEnergy = 0.0f;
	float CurrentMaxEnergy = 0.1f;

	FLinearColor EnergyColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};
