// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSGravityCore.generated.h"

UCLASS()
class CHRONOSPACE_API ACSGravityCore : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSGravityCore();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	FVector GetGravityDirection(ACharacter* Character);

protected:
	UFUNCTION()
	void OnActorBeginOverlapCallback(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorEndOverlapCallback(AActor* OverlappedActor, AActor* OtherActor);

	TMap<FName, TObjectPtr<ACharacter>> TargetCharacters;
};
