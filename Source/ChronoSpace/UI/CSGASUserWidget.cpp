// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCSGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UCSGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
