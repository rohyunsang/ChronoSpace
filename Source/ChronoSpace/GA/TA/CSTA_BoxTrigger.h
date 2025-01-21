// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CSTA_BoxTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTatgetActorEndedDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSTA_BoxTrigger : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ACSTA_BoxTrigger();

	virtual void BeginPlay() override;

	FORCEINLINE void SetDurationTime(float InDurationTime) { DurtionTime = InDurationTime; }

	FTatgetActorEndedDelegate OnComplete;

protected:
	void SetSteticMeshMaterial(class UMaterial* Material, float Scale);

	UPROPERTY(VisibleAnywhere, Category = "Box", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxTrigger;

	UPROPERTY()
	TMap<FName, TObjectPtr<AActor> > ActorsInBoxTrigger;

	UPROPERTY(EditAnywhere, Category = "Box")
	float BoxExtentSize = 200.0f;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	FVector MeshScale;
	float DurtionTime = 1.0f;
	bool bShowDebug = true;
	
};
