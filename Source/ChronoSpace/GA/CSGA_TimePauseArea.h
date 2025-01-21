// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CSGA_TimePauseArea.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGA_TimePauseArea : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCSGA_TimePauseArea();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

private:
	void ActivateTask();

	UFUNCTION()
	void StopActivateTask();
};
