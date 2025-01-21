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
	UCSAT_DurationTask();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ReverseGravity", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCSAT_DurationTask* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_DurationBase> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	FORCEINLINE void SetDurtionTime(float InDurtionTime) { DurationTime = InDurtionTime; }

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	UPROPERTY(BlueprintAssignable)
	FReverseGravityEndDelegate OnComplete;

protected:
	void EndTargetActor();

	UFUNCTION()
	void OnTargetActorReadyCallback();

	UPROPERTY()
	TSubclassOf<class ACSTA_DurationBase> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class ACSTA_DurationBase> SpawnedTargetActor;

	FTimerHandle EndTimer; 

	float DurationTime = 1.0f;
};
