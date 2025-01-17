// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSTA_ReverseGravityBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTatgetActorDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_ReverseGravityBox : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ACSTA_ReverseGravityBox();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	FTatgetActorDelegate OnComplete;

protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
};
