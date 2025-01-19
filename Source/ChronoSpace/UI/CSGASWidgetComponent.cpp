// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSGASWidgetComponent.h"
#include "UI/CSGASUserWidget.h"

void UCSGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UCSGASUserWidget* GASUserWidget = Cast<UCSGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}