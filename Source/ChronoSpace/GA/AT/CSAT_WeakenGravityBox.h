// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/AT/CSAT_DurationTask.h"
#include "CSAT_WeakenGravityBox.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_WeakenGravityBox : public UCSAT_DurationTask
{
	GENERATED_BODY()
	
public:
	UCSAT_WeakenGravityBox();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ReverseGravity", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCSAT_WeakenGravityBox* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ACSTA_WeakenGravityBox> InTargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	FORCEINLINE void SetGravityCoef(float InGravityCoef) { GravityCoef = InGravityCoef; }

protected:
	virtual void SpawnAndInitializeTargetActor() override;
	virtual void FinalizeTargetActor() override;
	virtual void EndTargetActor() override;

	UPROPERTY()
	TObjectPtr< class ACSTA_WeakenGravityBox > SpawnedTargetActor;

	float GravityCoef;
};
