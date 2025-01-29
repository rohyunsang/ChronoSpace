// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/CSTA_BoxTrigger.h"
#include "CSTA_WeakenGravityBox.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_WeakenGravityBox : public ACSTA_BoxTrigger
{
	GENERATED_BODY()

public:
	ACSTA_WeakenGravityBox();

	virtual void BeginPlay() override;

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	FORCEINLINE void SetGravityCoef(float InGravityCoef) { GravityCoef = InGravityCoef; }

protected:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSaturationSetting();

	void SaturationSetting();
	void HandleSaturationSetting();

	UPROPERTY(EditAnywhere, Category = "GravityCoef")
	float GravityCoef = 0.1f;
};
