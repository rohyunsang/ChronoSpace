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

	virtual FVector GetGravityDirection(AActor* Actor) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Sphere", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereTrigger;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Sphere")
	float GravityInfluenceRange = 400.0f;
};
