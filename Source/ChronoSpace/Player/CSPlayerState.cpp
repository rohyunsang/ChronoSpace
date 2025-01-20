// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CSPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CSAttributeSet.h"

ACSPlayerState::ACSPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UCSAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACSPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
