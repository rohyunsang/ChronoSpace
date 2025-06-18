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

	// 이동시킬 Actor 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	TObjectPtr<AActor> TargetActor;

	// === 위치 관련 ===
	// 목표 위치 (상대적 위치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	FVector TargetLocation;

	// 초기 위치 (런타임에 설정됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move Object")
	FVector InitialLocation;

	// === 회전 관련 ===
	// 목표 회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	FRotator TargetRotation;

	// 초기 회전 (런타임에 설정됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move Object")
	FRotator InitialRotation;

	// === 설정 옵션 ===
	// 위치 이동 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	bool bEnableLocationMovement;

	// 회전 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object")
	bool bEnableRotationMovement;

	// 이동 완료 여부
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
		bEnableRotationMovement = true;  // 기본적으로 회전은 비활성화
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
	// 이동 관련 네트워크 함수들
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastStartMovement(bool bMoveToTarget);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastUpdateMovement(const TArray<FVector>& NewLocations, const TArray<FRotator>& NewRotations);


	// 이동 로직
	void StartMovement();
	void UpdateMovement(float DeltaTime);
	void InitializeObjectPositions();

protected:
	// 이동시킬 오브젝트들의 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	TArray<FMoveObjectData> MoveObjects;

	// 이동 속도 (단위/초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// 회전 속도 (도/초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	// 이동 완료 허용 오차
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float MoveTolerance;

	// 회전 완료 허용 오차 (도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	float RotationTolerance;

	// 현재 이동 중인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;

	// 이동 방향 (true: 목표로, false: 초기 위치로)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Move Object", meta = (AllowPrivateAccess = "true"))
	bool bMovingToTarget;

};
