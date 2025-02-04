// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CSGA_AbilityPreviewBox.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGA_AbilityPreviewBox : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCSGA_AbilityPreviewBox();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	FORCEINLINE const float GetDurationTime() const { return DurationTime; }
	FORCEINLINE void SetDurationTime(float InDuarionTime) { DurationTime = InDuarionTime; }
private:
	void ActivateTask();

	UFUNCTION()
	void StopActivateTask();

	UFUNCTION()
	void RunAbility(float BoxSize);

	UFUNCTION()
	void StopAbility();
	

	UPROPERTY(EditAnywhere, Category = "Duration")
	float DurationTime = 1.1f;
};
