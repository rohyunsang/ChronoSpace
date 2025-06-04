// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CSGameState.generated.h"

class ACSPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayersUpdated, const TArray<ACSPlayerState*>&, PlayerStates);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACSGameState();

	// 플레이어 상태 관리
	UFUNCTION(BlueprintCallable, Category = "Players")
	TArray<ACSPlayerState*> GetAllMyPlayerStates() const;

	UFUNCTION(BlueprintCallable, Category = "Players")
	ACSPlayerState* GetMyPlayerState(int32 PlayerIndex) const;

	// 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Players")
	FOnPlayersUpdated OnPlayersUpdated;

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

private:
	void BroadcastPlayersUpdated();
};
