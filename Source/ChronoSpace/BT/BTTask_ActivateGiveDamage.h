// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateGiveDamage.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UBTTask_ActivateGiveDamage : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateGiveDamage();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
