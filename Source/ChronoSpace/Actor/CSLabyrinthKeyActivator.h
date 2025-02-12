// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSLabyrinthKeyActivator.generated.h"

UCLASS()
class CHRONOSPACE_API ACSLabyrinthKeyActivator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSLabyrinthKeyActivator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetActorActive(AActor* Actor, bool bActive);

	void SetLabyrinthKey();

	UPROPERTY()
	TArray< TObjectPtr<class ACSLabyrinthKey> > Keys;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MaxKeyCount")
	int32 MaxKeyCount;
};
