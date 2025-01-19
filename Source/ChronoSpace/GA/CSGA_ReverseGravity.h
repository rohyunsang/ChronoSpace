// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CSGA_ReverseGravity.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGA_ReverseGravity : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCSGA_ReverseGravity();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/*UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);*/

private:
	void ActivateTasks();

	UFUNCTION()
	void StopActivateTasks();
};
