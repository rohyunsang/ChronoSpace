// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CSInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCSInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHRONOSPACE_API ICSInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginInteraction() = 0;

	virtual void EndInteraction() = 0;

	virtual void Interact() = 0;
};
