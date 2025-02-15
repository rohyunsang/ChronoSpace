// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CSAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACSAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void ActiveMove(bool bIsActive);

	static const FName PlayerPosKey;
	static const FName PatrolPosKey;
	static const FName bShouldStopMoveKey;
	
private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
};
