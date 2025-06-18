// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CSGravityCore.h"
#include "CSGravityCoreSphere.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSGravityCoreSphere : public ACSGravityCore
{
	GENERATED_BODY()
	
public:
	ACSGravityCoreSphere();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Sphere", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Sphere", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Sphere")
	float GravityInfluenceRange = 400.0f;
};
