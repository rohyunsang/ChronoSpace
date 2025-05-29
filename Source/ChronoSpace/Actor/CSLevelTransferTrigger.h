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

    // �̵��� ���� �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transfer")
    FString TargetLevelName = TEXT("Level2");

    // ���� ��ġ �±� (��ǥ �������� ã�� PlayerStart �±�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transfer")
    FString SpawnPointTag = TEXT("");

    // Ʈ���Ű� �� ���� �۵��ϴ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
    bool bTriggerOnce = true;

    // ��ȯ ���� �ð� (��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings", meta = (ClampMin = "0.0"))
    float TransferDelay = 0.0f;

    // Ʈ���Ű� �̹� �۵��ߴ��� ����
    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    bool bHasTriggered = false;

public:
    // Overlap �̺�Ʈ
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    // ���� ��ȯ ����
    UFUNCTION(BlueprintCallable, Category = "Level Transfer")
    void ExecuteLevelTransfer();

    // �÷��̾����� Ȯ��
    UFUNCTION(BlueprintCallable, Category = "Utils")
    bool IsPlayer(AActor* Actor);

public:
    // Ʈ���� ���� (���� �����ϰ�)
    UFUNCTION(BlueprintCallable, Category = "Trigger")
    void ResetTrigger();

    // ���� ��ȯ ���� ����
    UFUNCTION(BlueprintCallable, Category = "Level Transfer")
    void ForceLevelTransfer();

    // �̺�Ʈ ���ε�
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelTransferStarted, const FString&, LevelName);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelTransferStarted OnLevelTransferStarted;

    // Getter �Լ���
    UFUNCTION(BlueprintPure, Category = "State")
    bool HasTriggered() const { return bHasTriggered; }

    UFUNCTION(BlueprintPure, Category = "State")
    FString GetTargetLevelName() const { return TargetLevelName; }
};