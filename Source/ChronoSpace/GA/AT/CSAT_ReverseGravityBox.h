// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CSAT_ReverseGravityBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReverseGravityEndDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_ReverseGravityBox : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UCSAT_ReverseGravityBox();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ReverseGravity", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCSAT_ReverseGravityBox* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_ReverseGravityBox> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	UPROPERTY(BlueprintAssignable)
	FReverseGravityEndDelegate OnComplete;

protected:
	void EndTargetActor();

	UFUNCTION()
	void OnTargetActorReadyCallback();

	UPROPERTY()
	TSubclassOf<class ACSTA_ReverseGravityBox> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class ACSTA_ReverseGravityBox> SpawnedTargetActor;

	FTimerHandle EndTimer; 
};
