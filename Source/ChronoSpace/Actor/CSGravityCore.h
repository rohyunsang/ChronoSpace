// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSGravityCore.generated.h"

UCLASS()
class CHRONOSPACE_API ACSGravityCore : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSGravityCore();

	UFUNCTION(BlueprintCallable)
	virtual FVector GetGravityDirection(AActor* Actor);
};
