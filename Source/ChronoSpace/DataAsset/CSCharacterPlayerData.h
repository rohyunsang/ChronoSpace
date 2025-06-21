// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CSCharacterPlayerData.generated.h"


/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSCharacterPlayerData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCSCharacterPlayerData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpZVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinAnalogWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector CameraOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TSubclassOf<class UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FRotator MeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capsule")
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capsule")
	float CapsuleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	float TriggerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	float TriggerHeight;
};
