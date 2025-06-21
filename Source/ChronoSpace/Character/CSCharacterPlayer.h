// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CSCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "CSF_CharacterFrameData.h"
#include "CSCharacterPlayer.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionDelegate);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSCharacterPlayer : public ACSCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACSCharacterPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS") 
	TObjectPtr<class UCSGASManagerComponent> GASManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformRecord")
	TObjectPtr<class UCSTransformRecordComponent> TransformRecordComponent;

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AbilityPreviewAction;

// ASC Section
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSGASWidgetComponent> EnergyBar;

	*/
	
// Trigger Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSPushingCharacterComponent> PushingCharacterComponent;

// WhiteHall
public:
	FORCEINLINE class ACSWhiteHall* GetWhiteHall() { return WhiteHall; }
	void SetWhiteHall(class ACSWhiteHall* InWhiteHall) { WhiteHall = InWhiteHall; }
	void ClearWhiteHall();

// Misc
protected:
	UPROPERTY()
	TObjectPtr<class ACSWhiteHall> WhiteHall;

// Interaction Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSPlayerInteractionComponent> InteractionComponent;

// Character Scaling 

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSCharacterScaleComponent> ScaleComponent;


// HP UI
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RequestUIRefresh();
};
