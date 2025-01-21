// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSDurationBasedTargetActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTatgetActorEndedDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSDurationBasedTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ACSDurationBasedTargetActor();

	FTatgetActorEndedDelegate OnComplete;
};
