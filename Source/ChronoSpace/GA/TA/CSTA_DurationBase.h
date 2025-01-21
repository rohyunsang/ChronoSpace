// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSTA_DurationBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTatgetActorEndedDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_DurationBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ACSTA_DurationBase();

	FTatgetActorEndedDelegate OnComplete;
};
