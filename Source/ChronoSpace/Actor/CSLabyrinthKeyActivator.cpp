// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSLabyrinthKeyActivator.h"
#include "EngineUtils.h"
#include "Actor/CSLabyrinthKey.h"
#include "ChronoSpace.h"

// Sets default values
ACSLabyrinthKeyActivator::ACSLabyrinthKeyActivator()
{
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MaxKeyCount = 10;
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
void ACSLabyrinthKeyActivator::SetLabyrinthKey()
{
	for (TActorIterator<ACSLabyrinthKey> It(GetWorld()); It; ++It)
	{
		ACSLabyrinthKey* LabyrinthKey = *It;

		if ( LabyrinthKey )
		{
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

		Keys[Idx]->SetActive(true);

		++ActivatedKeysCount;
	}
}


