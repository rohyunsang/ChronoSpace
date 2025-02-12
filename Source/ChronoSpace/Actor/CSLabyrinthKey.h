// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSLabyrinthKey.generated.h"

UCLASS()
class CHRONOSPACE_API ACSLabyrinthKey : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSLabyrinthKey();

	UFUNCTION()
	void OnTriggerBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnTriggerEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Interact();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> InteractionPromptComponent;

	float TriggerRange = 100.0f;
};
