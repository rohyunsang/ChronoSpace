// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CSAT_DurationTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReverseGravityEndDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_DurationTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UCSAT_DurationTask() {};

	FORCEINLINE void SetDurtionTime(float InDurtionTime) { DurationTime = InDurtionTime; }

	UPROPERTY(BlueprintAssignable)
	FReverseGravityEndDelegate OnComplete;

protected:
	UFUNCTION()
	void OnTargetActorReadyCallback();

	virtual void SpawnAndInitializeTargetActor() {};
	virtual void FinalizeTargetActor() {};
	virtual void EndTargetActor() {};

	UPROPERTY()
	TSubclassOf<class AGameplayAbilityTargetActor> TargetActorClass;

	FTimerHandle EndTimer; 

	float DurationTime = 1.0f;
};
