// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/AT/CSAT_DurationTask.h"
#include "CSAT_ChronoControl.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_ChronoControl : public UCSAT_DurationTask
{
	GENERATED_BODY()
public:
	UCSAT_ChronoControl();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ReverseGravity", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCSAT_ChronoControl* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_ChronoControl> InTargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	virtual void SpawnAndInitializeTargetActor() override;
	virtual void FinalizeTargetActor() override;
	virtual void EndTargetActor() override;

	UPROPERTY()
	TObjectPtr< class ACSTA_ChronoControl > SpawnedTargetActor;
};
