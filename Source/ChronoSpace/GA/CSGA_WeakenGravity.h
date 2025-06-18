// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CSGA_WeakenGravity.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CHRONOSPACE_API UCSGA_WeakenGravity : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCSGA_WeakenGravity();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	FORCEINLINE const float GetDurationTime() const { return DurationTime; }
	FORCEINLINE void SetDurationTime(float InDuarionTime) { DurationTime = InDuarionTime; }

private:
	void ActivateTask();

	UFUNCTION()
	void StopActivateTask();

	UPROPERTY(EditAnywhere, Category = "Duration")
	float DurationTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "GravityCoef")
	float GravityCoef = 0.1f;
};
