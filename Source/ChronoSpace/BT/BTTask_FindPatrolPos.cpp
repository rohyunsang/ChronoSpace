// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_FindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/CSAIController.h"
#include "Character/CSCharacterPatrol.h"
#include "ChronoSpace.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Owner Is Controller
	ACSCharacterPatrol* Patrol = Cast<ACSCharacterPatrol>(OwnerComp.GetAIOwner()->GetPawn());

	if ( Patrol )
	{
		//UE_LOG(LogCS, Log, TEXT("UBTTask_FindPatrolPos ExecuteTask - %f, %f, %f"), Patrol->GetPatrolPos().X, Patrol->GetPatrolPos().Y, Patrol->GetPatrolPos().Z);
		Cast<ACSAIController>(OwnerComp.GetAIOwner())->ActiveMove(true);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ACSAIController::PatrolPosKey, Patrol->GetPatrolPos());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
