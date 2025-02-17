// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CSPlayerController.h"
#include "ChronoSpace.h"

ACSPlayerController::ACSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeRef(TEXT("/Game/Blueprint/Camera/BP_CameraShake.BP_CameraShake_C"));
	if ( CameraShakeRef.Succeeded() )
	{
		CameraShake = CameraShakeRef.Class;
	}
}

void ACSPlayerController::ShakeCamera()
{
	ClientStartCameraShake(CameraShake);
}
