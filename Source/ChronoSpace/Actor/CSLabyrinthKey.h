// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CSInteractionInterface.h"
#include "CSLabyrinthKey.generated.h"

UCLASS()
class CHRONOSPACE_API ACSLabyrinthKey : public AActor, public ICSInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACSLabyrinthKey();

	UPROPERTY(ReplicatedUsing = OnRep_bIsActive)
	bool bIsActive;

	virtual void BeginInteraction() override;
	virtual void EndInteraction() override;
	virtual void Interact() override;

	void SetActive(bool bActive);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> InteractionPromptComponent;

	UFUNCTION()
	void OnRep_bIsActive();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float TriggerRange = 80.0f;
};
