// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CSAT_TimePauseArea.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimePauseEndDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_TimePauseArea : public UAbilityTask
{
	GENERATED_BODY()
public:
	UCSAT_TimePauseArea();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TimePauseItem", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCSAT_TimePauseArea* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_TimePauseArea> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	FORCEINLINE void SetDurtionTime(float InDurtionTime) { DurationTime = InDurtionTime; }

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	UPROPERTY(BlueprintAssignable)
	FTimePauseEndDelegate OnComplete;

protected:
	void EndTargetActor();

	UFUNCTION()
	void OnTargetActorReadyCallback();

	UPROPERTY()
	TSubclassOf<class ACSTA_TimePauseArea> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class ACSTA_TimePauseArea> SpawnedTargetActor;

	FTimerHandle EndTimer;

	float DurationTime = 1.0f;
};
