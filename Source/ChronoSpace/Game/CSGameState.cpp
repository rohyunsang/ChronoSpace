// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CSGameState.h"
#include "Player/CSPlayerState.h"



ACSGameState::ACSGameState()
{
	SetReplicates(true);
}

TArray<ACSPlayerState*> ACSGameState::GetAllMyPlayerStates() const
{
	TArray<ACSPlayerState*> MyPlayerStates;

	for (APlayerState* PS : PlayerArray)
	{
		if (ACSPlayerState* MyPS = Cast<ACSPlayerState>(PS))
		{
			MyPlayerStates.Add(MyPS);
		}
	}

	return MyPlayerStates;
}

ACSPlayerState* ACSGameState::GetMyPlayerState(int32 PlayerIndex) const
{
	TArray<ACSPlayerState*> MyPlayerStates = GetAllMyPlayerStates();

	if (MyPlayerStates.IsValidIndex(PlayerIndex))
	{
		return MyPlayerStates[PlayerIndex];
	}

	return nullptr;
}

void ACSGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	BroadcastPlayersUpdated();
}

void ACSGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	BroadcastPlayersUpdated();
}

void ACSGameState::BroadcastPlayersUpdated()
{
	OnPlayersUpdated.Broadcast(GetAllMyPlayerStates());
}