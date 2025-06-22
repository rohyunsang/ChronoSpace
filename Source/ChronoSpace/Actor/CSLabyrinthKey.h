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
	virtual void BeginPlay() override;

// Trigger
protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereTrigger;

// Mesh
protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TSoftObjectPtr<class UStaticMesh> StaticMesh;


// Interaction
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> InteractionPromptComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSoftClassPtr<UUserWidget> InteractionPromptWidgetClass;


protected:
	UFUNCTION()
	void OnRep_bIsActive();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	float TriggerRange = 80.0f;
};
