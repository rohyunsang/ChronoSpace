// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CSSwitchBase.h"
#include "Engine/Engine.h"
#include "CSMoveObjectSwitch.generated.h"

USTRUCT(BlueprintType)
struct FMoveObjectData
{
	GENERATED_BODY()

	// �̵���ų Actor ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	TObjectPtr<AActor> TargetActor;

	// === ��ġ ���� ===
	// ��ǥ ��ġ (����� ��ġ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	FVector TargetLocation;

	// �ʱ� ��ġ (��Ÿ�ӿ� ������)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move Object")
	FVector InitialLocation;

	// === ȸ�� ���� ===
	// ��ǥ ȸ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	FRotator TargetRotation;

	// �ʱ� ȸ�� (��Ÿ�ӿ� ������)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move Object")
	FRotator InitialRotation;

	// === ���� �ɼ� ===
	// ��ġ �̵� Ȱ��ȭ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	bool bEnableLocationMovement;

	// ȸ�� Ȱ��ȭ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	bool bEnableRotationMovement;

	// �̵� �Ϸ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move Object")
	bool bIsAtTarget;

	FMoveObjectData()
	{
		TargetActor = nullptr;
		TargetLocation = FVector::ZeroVector;
		InitialLocation = FVector::ZeroVector;
		TargetRotation = FRotator::ZeroRotator;
		InitialRotation = FRotator::ZeroRotator;
		bEnableLocationMovement = true;
		bEnableRotationMovement = true;  // �⺻������ ȸ���� ��Ȱ��ȭ
		bIsAtTarget = false;
	}
};

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API ACSMoveObjectSwitch : public ACSSwitchBase
{
	GENERATED_BODY()
	
public:
	ACSMoveObjectSwitch();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetInteracted(bool bInInteracted);

protected:
	// �̵� ���� ��Ʈ��ũ �Լ���
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastStartMovement(bool bMoveToTarget);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastUpdateMovement(const TArray<FVector>& NewLocations, const TArray<FRotator>& NewRotations);


	// �̵� ����
	void StartMovement();
	void UpdateMovement(float DeltaTime);
	void InitializeObjectPositions();

protected:
	// �̵���ų ������Ʈ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	TArray<FMoveObjectData> MoveObjects;

	// �̵� �ӵ� (����/��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// ȸ�� �ӵ� (��/��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	// �̵� �Ϸ� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float MoveTolerance;

	// ȸ�� �Ϸ� ��� ���� (��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float RotationTolerance;

	// ���� �̵� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;

	// �̵� ���� (true: ��ǥ��, false: �ʱ� ��ġ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	bool bMovingToTarget;

};
