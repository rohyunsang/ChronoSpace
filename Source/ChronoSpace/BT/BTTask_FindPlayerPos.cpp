// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_FindPlayerPos.h"
#include "Interface/CSCharacterPlayerFinderInterface.h"
#include "Character/CSCharacterPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/CSAIController.h"

UBTTask_FindPlayerPos::UBTTask_FindPlayerPos()
{
	NodeName = TEXT("FindPlayerPos");
}

EBTNodeResult::Type UBTTask_FindPlayerPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ICSCharacterPlayerFinderInterface* Finder = Cast<ICSCharacterPlayerFinderInterface>(OwnerComp.GetAIOwner()->GetPawn());

	if ( Finder )
	{
		ACSCharacterPlayer* Player = Finder->GetCharacterPlayer();

		if ( Player )
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACSAIController::PlayerKey, Player);

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
