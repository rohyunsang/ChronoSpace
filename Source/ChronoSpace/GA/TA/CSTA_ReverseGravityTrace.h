// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSTA_ReverseGravityTrace.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_ReverseGravityTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ACSTA_ReverseGravityTrace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
}; 
 