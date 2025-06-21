// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/CSCharacterPlayerData.h"

UCSCharacterPlayerData::UCSCharacterPlayerData()
{
	RotationRate = FRotator(1500.0f, 500.0f, 0.0f);
	JumpZVelocity = 250.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 500.f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;

	TargetArmLength = 400.0f;
	CameraOffset = FVector();

	MeshLocation = FVector(0.0f, 0.0f, -100.0f);
	MeshRotation = FRotator(0.0f, -90.0f, 0.0f);

	CapsuleRadius = 42.0f;
	CapsuleHeight = 96.0f;

	TriggerRadius = 50.0f;
	TriggerHeight = 100.0f;
}
