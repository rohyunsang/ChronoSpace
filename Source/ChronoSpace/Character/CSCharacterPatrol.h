// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CSCharacterBase.h"
#include "Interface/CSCharacterPlayerFinderInterface.h"
#include "CSCharacterPatrol.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSCharacterPatrol : public ACSCharacterBase, public ICSCharacterPlayerFinderInterface
{
	GENERATED_BODY()

public:
	ACSCharacterPatrol();
	
	FVector GetPatrolPos();

	virtual void BeginPlay() override;

	virtual class ACSCharacterPlayer* GetCharacterPlayer() override;

protected:

	UPROPERTY()
	TObjectPtr<class ACSCharacterPlayer> Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPos")
	TArray< FVector > PatrolPoses;

	int8 CurrentPatrolIdx;
	int8 PatrolPosesLength;

// Trigger Section
protected:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY()
	TObjectPtr<class ACSCharacterPlayer> CharacterPlayer;

	int32 OverlappedPlayerCount;

// ASC Section
public:
	UFUNCTION()
	void ActivateGiveDamage();

protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UCSAttributeSet> AttributeSet;
};
