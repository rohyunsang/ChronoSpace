// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CSGASWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGASWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
protected:
	virtual void InitWidget() override;
};
