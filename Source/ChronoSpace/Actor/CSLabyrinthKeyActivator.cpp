// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSLabyrinthKeyActivator.h"
#include "EngineUtils.h"
#include "Actor/CSLabyrinthKey.h"
#include "ChronoSpace.h"

// Sets default values
ACSLabyrinthKeyActivator::ACSLabyrinthKeyActivator()
{
	bReplicates = true;
	MaxKeyCount = 3;
}

// Called when the game starts or when spawned
void ACSLabyrinthKeyActivator::BeginPlay()
{
	Super::BeginPlay();
	
	if ( HasAuthority() )
	{
		SetLabyrinthKey();
	}
}

void ACSLabyrinthKeyActivator::SetActorActive(AActor* Actor, bool bActive)
{
	Actor->SetActorHiddenInGame(!bActive);
	Actor->SetActorEnableCollision(bActive);
	//SetActorTickEnabled(bActive);
}

void ACSLabyrinthKeyActivator::SetLabyrinthKey()
{
	for (TActorIterator<ACSLabyrinthKey> It(GetWorld()); It; ++It)
	{
		ACSLabyrinthKey* LabyrinthKey = *It;

		if ( LabyrinthKey )
		{
			SetActorActive(LabyrinthKey, false);
			Keys.Emplace(LabyrinthKey);
		}
	}

	int8 length = Keys.Num();

	if ( length < MaxKeyCount )
	{
		return;
	}

	//UE_LOG(LogCS, Log, TEXT("SetLabyrinthKey : %d"), length);

	int32 ActivatedKeysCount = 0;
	while ( ActivatedKeysCount < MaxKeyCount )
	{
		int8 Idx = FMath::RandRange(0, length);

		if ( !Keys.IsValidIndex(Idx) || Keys[Idx]->GetActorEnableCollision() )
		{
			continue;
		}

		SetActorActive(Keys[Idx], true);

		++ActivatedKeysCount;
	}
}


