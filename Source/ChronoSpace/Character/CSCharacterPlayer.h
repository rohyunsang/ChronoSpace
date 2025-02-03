// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CSCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "CSF_CharacterFrameData.h"
#include "CSCharacterPlayer.generated.h"

UENUM()
enum class EAbilityIndex : uint8
{
	ReverseGravity = 0,
	BlackHole = 1,

	ChronoControl = 100,
	TimeRewind = 101,

	AbilityPreviewBox = 200 
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
	virtual void Tick(float DeltaTime) override; 

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
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReverseGravityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChronoControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AbilityPreviewAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TimeRewindAction;

// ASC Section
protected:
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray< TSubclassOf<class UGameplayAbility> > StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap< int32, TSubclassOf<class UGameplayAbility> > StartInputAbilities;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCSGASWidgetComponent> EnergyBar;


// Character Frame Datas
	// 3초간 Transform 저장
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transform History")
	TArray<FCSF_CharacterFrameData> TransformHistory;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	float RecordInterval = 0.01f; // Transform 기록 주기 (0.01초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	int32 MaxHistorySize = 300; // 3초 동안의 Transform 기록 (0.01초 간격)

	// 현재 Transform을 기록
	void RecordTransform();

	float TimeSinceLastRecord = 0.0f; // 마지막 기록 이후 경과 시간

};
