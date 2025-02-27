// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSTA_MultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_MultiTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ACSTA_MultiTrace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug;
};
