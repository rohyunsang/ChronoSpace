// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CSCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "CSF_CharacterFrameData.h"
#include "CSCharacterPlayer.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionDelegate);

UENUM()
enum class EAbilityIndex : uint8
{

	ReverseGravity = 1,
	BlackHole = 2,
	WhiteHole = 3,
	WeakenGravity10P = 4,
	WeakenGravity50P = 5,

	ChronoControl = 100,
	TimeRewind = 101,

	ScaleSmall = 200,
	ScaleNormal = 201,
	ScaleLarge = 202

};

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
	virtual void Tick(float DeltaSeconds) override;
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
	TObjectPtr<class UInputAction> ReverseGravityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

	TObjectPtr<class UInputAction> BlackHoleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WhiteHoleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WeakenGravity10PAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WeakenGravity50PAction;

	TObjectPtr<class UInputAction> ChronoControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AbilityPreviewAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TimeRewindAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ScaleSmallAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ScaleNormalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ScaleLargeAction;

// ASC Section
protected:
	void SetupGASInputComponent();

	// Input Pressed RPC **************************************
	void GASInputPressed(int32 InputId);

	UFUNCTION(Server, Reliable)
	void ServerGASInputPressed(int32 InputId);

	void HandleGASInputPressed(int32 InputId);
	// ********************************************************

	// Input Released RPC *************************************
	void GASInputReleased(int32 InputId);

	UFUNCTION(Server, Reliable)
	void ServerGASInputReleased(int32 InputId);

	void HandleGASInputReleased(int32 InputId);
	// ********************************************************

	void SetASC();
	void SetGASAbilities();

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray< TSubclassOf<class UGameplayAbility> > StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap< int32, TSubclassOf<class UGameplayAbility> > StartInputAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSGASWidgetComponent> EnergyBar;

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


// Character Frame Datas

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transform History")
	TArray<FCSF_CharacterFrameData> TransformHistory;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	float RecordInterval = 0.03f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	int32 MaxHistorySize = 99; 

	void RecordTransform();

// Interaction Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSPlayerInteractionComponent> InteractionComponent;

	//FInteractionDelegate OnInteract;

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	void Interact();
	void EndInteract();

	bool bIsInteracted;

	float TimeSinceLastRecord = 0.0f; 


// Character Scaling 

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCSCharacterScaleComponent> ScaleComponent;
};
