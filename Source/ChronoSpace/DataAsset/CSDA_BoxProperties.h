// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CSDA_BoxProperties.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CHRONOSPACE_API UCSDA_BoxProperties : public UDataAsset
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Properties")
	float BoxSize;

};
