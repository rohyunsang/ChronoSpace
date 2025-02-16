// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACSPlayerController();

	void ShakeCamera();

protected:
	UPROPERTY()
	TSubclassOf<class UCameraShakeBase> CameraShake;
};
