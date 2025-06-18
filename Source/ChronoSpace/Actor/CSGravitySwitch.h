// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CSSwitchBase.h"
#include "CSGravitySwitch.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSGravitySwitch : public ACSSwitchBase
{
	GENERATED_BODY()
	
public:
	ACSGravitySwitch();

	virtual void Interact() override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSetGravity(ACharacter* Char, FVector Gravity);

	void SetInteracted(bool bInInteracted);
};
