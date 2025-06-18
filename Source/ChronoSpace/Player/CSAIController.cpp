// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CSAIController.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ChronoSpace.h"

const FName ACSAIController::PlayerKey(TEXT("Player"));
const FName ACSAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACSAIController::bShouldStopMoveKey(TEXT("bShouldStopMove"));

ACSAIController::ACSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObjRef(TEXT("/Script/AIModule.BlackboardData'/Game/BehaviorTree/BB_CSCharacterPatrol.BB_CSCharacterPatrol'"));
	if ( BBObjRef.Succeeded() )
	{
		BBAsset = BBObjRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObjRef(TEXT("/Script/AIModule.BehaviorTree'/Game/BehaviorTree/BT_CSCharacterPatrol.BT_CSCharacterPatrol'"));
	if (BTObjRef.Succeeded())
	{
		BTAsset = BTObjRef.Object;
	}
}

void ACSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UBlackboardComponent* BBComponent = Blackboard;

	if ( UseBlackboard(BBAsset, BBComponent) )
	{
		if ( !RunBehaviorTree(BTAsset) )
		{
			UE_LOG(LogCS, Log, TEXT("RunBehaviorTree Failed"));
		}
	}
}

void ACSAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACSAIController::ActiveMove(bool bIsActive)
{
	UBlackboardComponent* BBComponent = GetBlackboardComponent();

	if ( BBComponent )
	{
		BBComponent->SetValueAsBool(bShouldStopMoveKey, !bIsActive);
	}
}
