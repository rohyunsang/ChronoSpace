// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSWhiteHall.generated.h"

UCLASS()
class CHRONOSPACE_API ACSWhiteHall : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSWhiteHall();

protected:
	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

};
