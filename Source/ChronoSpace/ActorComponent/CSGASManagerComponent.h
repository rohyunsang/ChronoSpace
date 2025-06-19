// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSGASManagerComponent.generated.h"

UENUM()
enum class EAbilityIndex : int32
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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOSPACE_API UCSGASManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSGASManagerComponent();

	void SetASC(class UAbilitySystemComponent* InASC, class ACSPlayerState* PS);

	void SetGASAbilities();

	void SetupGASInputComponent(class UEnhancedInputComponent* InputComponent);

protected:
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


	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray< TSubclassOf<class UGameplayAbility> > StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap< EAbilityIndex, TSubclassOf<class UGameplayAbility> > StartInputAbilities; 

// Action
protected:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
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
		
};
