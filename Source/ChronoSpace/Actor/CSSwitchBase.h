// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CSInteractionInterface.h"
#include "CSSwitchBase.generated.h"

UCLASS()
class CHRONOSPACE_API ACSSwitchBase : public AActor, public ICSInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSSwitchBase();

	virtual void BeginInteraction() override;
	virtual void EndInteraction() override;
	virtual void Interact() override;

protected:
	void SetMaterial();

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY()
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY()
	TObjectPtr<class UMaterialInstance> MaterialGlowNonInteracted;

	UPROPERTY()
	TObjectPtr<class UMaterialInstance> MaterialGlowInteracted;

	UPROPERTY()
	TObjectPtr<class UMaterialInstance> MaterialSolidNonInteracted;

	UPROPERTY()
	TObjectPtr<class UMaterialInstance> MaterialSolidInteracted;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> InteractionPromptComponent;

	bool bIsInteracted;
};
