// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "CSLevelTransferTrigger.generated.h"

UCLASS()
class CHRONOSPACE_API ACSLevelTransferTrigger : public AActor
{
	GENERATED_BODY()
	
public:
    ACSLevelTransferTrigger();

protected:
    virtual void BeginPlay() override;

    // Collision Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;

    // Visual Mesh (optional)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TriggerMesh;

    // 이동할 레벨 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transfer")
    FString TargetLevelName = TEXT("Level2");

    // 스폰 위치 태그 (목표 레벨에서 찾을 PlayerStart 태그)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transfer")
    FString SpawnPointTag = TEXT("");

    // 트리거가 한 번만 작동하는지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
    bool bTriggerOnce = true;

    // 전환 지연 시간 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings", meta = (ClampMin = "0.0"))
    float TransferDelay = 0.0f;

    // 트리거가 이미 작동했는지 여부
    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    bool bHasTriggered = false;

public:
    // Overlap 이벤트
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    // 레벨 전환 실행
    UFUNCTION(BlueprintCallable, Category = "Level Transfer")
    void ExecuteLevelTransfer();

    // 플레이어인지 확인
    UFUNCTION(BlueprintCallable, Category = "Utils")
    bool IsPlayer(AActor* Actor);

public:
    // 트리거 리셋 (재사용 가능하게)
    UFUNCTION(BlueprintCallable, Category = "Trigger")
    void ResetTrigger();

    // 레벨 전환 강제 실행
    UFUNCTION(BlueprintCallable, Category = "Level Transfer")
    void ForceLevelTransfer();

    // 이벤트 바인딩
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelTransferStarted, const FString&, LevelName);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelTransferStarted OnLevelTransferStarted;

    // Getter 함수들
    UFUNCTION(BlueprintPure, Category = "State")
    bool HasTriggered() const { return bHasTriggered; }

    UFUNCTION(BlueprintPure, Category = "State")
    FString GetTargetLevelName() const { return TargetLevelName; }
};