// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CSAT_MultiTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_MultiTrace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UCSAT_MultiTrace();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ReverseGravity", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCSAT_MultiTrace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_MultiTrace> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

	UPROPERTY()
	TSubclassOf<class ACSTA_MultiTrace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class ACSTA_MultiTrace> SpawnedTargetActor;
};
