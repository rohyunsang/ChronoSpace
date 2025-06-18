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
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	void UpdateEnergyBar();
protected:
	virtual void OnEnergyChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxEnergyChanged(const FOnAttributeChangeData& ChangeData);

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
