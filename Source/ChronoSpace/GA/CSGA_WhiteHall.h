// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CSGA_WhiteHall.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGA_WhiteHall : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCSGA_WhiteHall();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
