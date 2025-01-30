// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSGASEnergyBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CSAttributeSet.h"
#include "ChronoSpace.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
// #include "Tag/ABGameplayTag.h"


void UCSGASEnergyBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UCSAttributeSet::GetEnergyAttribute()).AddUObject(this, &UCSGASEnergyBarUserWidget::OnEnergyChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UCSAttributeSet::GetMaxEnergyAttribute()).AddUObject(this, &UCSGASEnergyBarUserWidget::OnMaxEnergyChanged);
		PbEnergyBar->SetFillColorAndOpacity(EnergyColor);
		
		const UCSAttributeSet* CurrentAttributeSet = ASC->GetSet<UCSAttributeSet>();
		
		if (CurrentAttributeSet)
		{
			CurrentEnergy = CurrentAttributeSet->GetEnergy();
			CurrentMaxEnergy = CurrentAttributeSet->GetMaxEnergy();

			if (CurrentMaxEnergy > 0.0f)
			{
				UpdateEnergyBar();
			}
			else
			{
				UE_LOG(LogCS, Warning, TEXT("CurrentMaxEnergy is 0"));
			}
		}
		else
		{
			UE_LOG(LogCS, Warning, TEXT("CurrentAttributeSet is null!"));
		}
	}
	else
	{
		UE_LOG(LogCS, Warning, TEXT("ASC is null! Ensure that the Ability System Component is properly initialized before calling this function."));
	}
}

void UCSGASEnergyBarUserWidget::OnEnergyChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentEnergy = ChangeData.NewValue;
	UpdateEnergyBar();
}

void UCSGASEnergyBarUserWidget::OnMaxEnergyChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxEnergy = ChangeData.NewValue;
	UpdateEnergyBar();
}

void UCSGASEnergyBarUserWidget::UpdateEnergyBar()
{
	//UE_LOG(LogCS, Log, TEXT("UpdateEnergyBar Is Called"));
	if (PbEnergyBar)
	{
		//UE_LOG(LogCS, Log, TEXT("UpdateEnergyBar : %f / %f"), CurrentEnergy, CurrentMaxEnergy); 
		PbEnergyBar->SetPercent(CurrentEnergy / CurrentMaxEnergy);
	}

	if (TxtEnergyStat)
	{
		TxtEnergyStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentEnergy, CurrentMaxEnergy)));
	}
}