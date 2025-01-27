// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSGASWidgetComponent.h"
#include "UI/CSGASUserWidget.h"
#include "ChronoSpace.h"

void UCSGASWidgetComponent::InitWidget()
{
	Super::InitWidget();
	UE_LOG(LogCS, Log, TEXT("InitWidget IS Called"));
	UCSGASUserWidget* GASUserWidget = Cast<UCSGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetOwner(GetOwner());
	}
	else
	{
		UE_LOG(LogCS, Log, TEXT("InitWidget Failed - No GASUserWidget"));
	}
}

void UCSGASWidgetComponent::ActivateGAS()
{
	UCSGASUserWidget* GASUserWidget = Cast<UCSGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
	else
	{
		UE_LOG(LogCS, Log, TEXT("ActivateGAS Failed - No GASUserWidget"));
	}
}
