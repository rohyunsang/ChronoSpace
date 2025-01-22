// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSTA_TimePauseArea.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetActorDelegate);


/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_TimePauseArea : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ACSTA_TimePauseArea();
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	FORCEINLINE void SetDurationTime(float InDurationTime) { DurtionTime = InDurationTime; }

	FTargetActorDelegate OnComplete;

protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TObjectPtr<class UBoxComponent> TimePauseTrigger;

	UPROPERTY()
	TMap<FName, TObjectPtr<AActor>> ActorsThatIsPausedTime;

	float DurtionTime = 1.0f;
	
};
