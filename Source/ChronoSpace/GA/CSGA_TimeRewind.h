// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CSGA_TimeRewind.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGA_TimeRewind : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	UCSGA_TimeRewind();

	/** Gameplay Ability �ߵ� �� ȣ�� */
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	/** Ability Task�� �Ϸ�Ǿ��� �� ȣ���� ��������Ʈ �Լ� */
	UFUNCTION()
	void OnTimeRewindFinishedDelegate();


};
