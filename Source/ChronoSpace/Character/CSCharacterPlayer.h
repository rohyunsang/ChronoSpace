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

	ReverseGravity = 1,
	BlackHole = 2,
	WhiteHole = 3,
	WeakenGravity10P = 4,
	WeakenGravity50P = 5,
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

	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetDead() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCSGASWidgetComponent> EnergyBar;

// Trigger Section
protected:
	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> Trigger;

	UPROPERTY()
	TMap< FName, TObjectPtr<ACharacter> > CharsInPushing;

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
	// 3�ʰ� Transform ����
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transform History")
	TArray<FCSF_CharacterFrameData> TransformHistory;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	float RecordInterval = 0.03f; // Transform ��� �ֱ� (0.03��)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	int32 MaxHistorySize = 99; // 3�� ������ Transform ��� (0.03�� ����)

	// ���� Transform�� ���
	void RecordTransform();

	float TimeSinceLastRecord = 0.0f; // ������ ��� ���� ��� �ð�

};
