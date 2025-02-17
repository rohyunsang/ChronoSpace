// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_ActivateGiveDamage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/CSAIController.h"
#include "Character/CSCharacterPatrol.h"
#include "ChronoSpace.h"

UBTTask_ActivateGiveDamage::UBTTask_ActivateGiveDamage()
{
	NodeName = TEXT("ActivateGiveDamage");
}

EBTNodeResult::Type UBTTask_ActivateGiveDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACSCharacterPatrol* Patrol = Cast<ACSCharacterPatrol>(OwnerComp.GetAIOwner()->GetPawn());

	if (Patrol)
	{
		Patrol->ActivateGiveDamage();
	}

	return EBTNodeResult::Succeeded;
}
