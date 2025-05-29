// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "CSTextTrigger.generated.h"

USTRUCT(BlueprintType)
struct FCSTextData
{
    GENERATED_BODY()

    // ǥ���� �ؽ�Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
    FString Text;

    // �� �ؽ�Ʈ�� ǥ�õǴ� �ð� (��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text", meta = (ClampMin = "0.1"))
    float DisplayDuration = 3.0f;

    // ���� �ؽ�Ʈ�� �Ѿ�� �ӵ� (��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text", meta = (ClampMin = "0.1"))
    float TransitionSpeed = 1.0f;

    FCSTextData()
    {
        Text = TEXT("Default Text");
        DisplayDuration = 3.0f;
        TransitionSpeed = 1.0f;
    }
};


UCLASS()
class CHRONOSPACE_API ACSTextTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSTextTrigger();

protected:
    virtual void BeginPlay() override;

    // Collision Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;

    // Visual Mesh (optional)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TriggerMesh;

    // �ؽ�Ʈ ������ �迭
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Settings")
    TArray<FCSTextData> TextDataArray;

    // Ʈ���Ű� �� ���� �۵��ϴ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
    bool bTriggerOnce = true;

    // Ʈ���Ű� �̹� �۵��ߴ��� ����
    UPROPERTY(BlueprintReadOnly, Category = "Trigger Settings")
    bool bHasTriggered = false;

    // ���� ǥ�� ���� �ؽ�Ʈ �ε���
    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    int32 CurrentTextIndex = 0;

    // Ÿ�̸� �ڵ�
    FTimerHandle TextDisplayTimer;
    FTimerHandle TextTransitionTimer;

    // ���� �ؽ�Ʈ�� ǥ�� ������ ����
    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    bool bIsDisplayingText = false;

    // CSTextWidget Ŭ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings")
    TSubclassOf<class UCSTextWidget> TextWidgetClass;

    // ���� �ؽ�Ʈ ���� �ν��Ͻ�
    UPROPERTY(BlueprintReadOnly, Category = "UI Runtime")
    class UCSTextWidget* CurrentTextWidget;

    // Ÿ���� ȿ�� ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings")
    bool bUseTypewriterEffect = false;

    // Ÿ���� �ӵ� (���ڴ� �ð�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings", meta = (ClampMin = "0.01"))
    float TypewriterSpeed = 0.05f;

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
    // �ؽ�Ʈ ������ ����
    UFUNCTION(BlueprintCallable, Category = "Text")
    void StartTextSequence();

    // ���� �ؽ�Ʈ ǥ��
    UFUNCTION()
    void ShowNextText();

    // ���� �ؽ�Ʈ �����
    UFUNCTION()
    void HideCurrentText();

    // �ؽ�Ʈ ������ �Ϸ�
    UFUNCTION(BlueprintCallable, Category = "Text")
    void CompleteTextSequence();

    // ���� ���� �� ����
    UFUNCTION(BlueprintCallable, Category = "UI")
    void CreateTextWidget();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void DestroyTextWidget();

    // �÷��̾����� Ȯ��
    UFUNCTION(BlueprintCallable, Category = "Utils")
    bool IsPlayer(AActor* Actor);

public:
    // �ؽ�Ʈ ������ �߰� (��Ÿ�ӿ��� ��� ����)
    UFUNCTION(BlueprintCallable, Category = "Text")
    void AddTextData(const FString& Text, float DisplayDuration = 3.0f, float TransitionSpeed = 1.0f);

    // Ʈ���� ���� (���� �����ϰ�)
    UFUNCTION(BlueprintCallable, Category = "Trigger")
    void ResetTrigger();

    // �ؽ�Ʈ ������ ���� �ߴ�
    UFUNCTION(BlueprintCallable, Category = "Text")
    void StopTextSequence();

    // Getter �Լ���
    UFUNCTION(BlueprintPure, Category = "Text")
    int32 GetTotalTextCount() const { return TextDataArray.Num(); }

    UFUNCTION(BlueprintPure, Category = "Text")
    int32 GetCurrentTextIndex() const { return CurrentTextIndex; }

    UFUNCTION(BlueprintPure, Category = "Text")
    bool IsDisplayingText() const { return bIsDisplayingText; }
};