// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSCharacterPatrol.h"
#include "ChronoSpace.h"
#include "Player/CSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/CSCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Character/CSCharacterPlayer.h"

ACSCharacterPatrol::ACSCharacterPatrol()
{
	AIControllerClass = ACSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;

	CurrentPatrolIdx = 0;
	PatrolPosesLength = 0;
	OverlappedPlayerCount = 0;

	// Trigger
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetBoxExtent(FVector(800.0f, 700.0f, 130.0f));
	Trigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	Trigger->SetupAttachment(GetCapsuleComponent());
	Trigger->SetRelativeLocation(FVector(700.0f, 0.0f, 130.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACSCharacterPatrol::OnTriggerBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACSCharacterPatrol::OnTriggerEndOverlap);
	Trigger->bVisualizeComponent = true;
}

FVector ACSCharacterPatrol::GetPatrolPos()
{
	int8 PrevIdx = CurrentPatrolIdx;
	CurrentPatrolIdx = (CurrentPatrolIdx + 1) % PatrolPosesLength;
	//UE_LOG(LogCS, Log, TEXT("%d"), CurrentPatrolIdx);
	return PatrolPoses[PrevIdx];
}

void ACSCharacterPatrol::BeginPlay()
{
	Super::BeginPlay();

	PatrolPosesLength = PatrolPoses.Num();
}

ACSCharacterPlayer* ACSCharacterPatrol::GetCharacterPlayer()
{
	return CharacterPlayer;
}

void ACSCharacterPatrol::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ACSCharacterPlayer* OverlappedPlayer = Cast<ACSCharacterPlayer>(OtherActor);

	if ( OverlappedPlayer )
	{
		CharacterPlayer = OverlappedPlayer;
		Cast<ACSAIController>(GetController())->ActiveMove(false);
		++OverlappedPlayerCount;
	}
}

void ACSCharacterPatrol::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACSCharacterPlayer* OverlappedPlayer = Cast<ACSCharacterPlayer>(OtherActor);

	if (OverlappedPlayer)
	{
		--OverlappedPlayerCount;
		if ( OverlappedPlayerCount > 0 )
		{
			OverlappedPlayerCount = 0;
			Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Cast<ACSAIController>(GetController())->ActiveMove(true);
			CharacterPlayer = nullptr;
		}
	}
}
