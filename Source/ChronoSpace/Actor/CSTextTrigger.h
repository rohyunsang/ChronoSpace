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

    // 표시할 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
    FString Text;

    // 이 텍스트가 표시되는 시간 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text", meta = (ClampMin = "0.1"))
    float DisplayDuration = 3.0f;

    // 다음 텍스트로 넘어가는 속도 (초)
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

    // 텍스트 데이터 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Settings")
    TArray<FCSTextData> TextDataArray;

    // 트리거가 한 번만 작동하는지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
    bool bTriggerOnce = true;

    // 트리거가 이미 작동했는지 여부
    UPROPERTY(BlueprintReadOnly, Category = "Trigger Settings")
    bool bHasTriggered = false;

    // 현재 표시 중인 텍스트 인덱스
    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    int32 CurrentTextIndex = 0;

    // 타이머 핸들
    FTimerHandle TextDisplayTimer;
    FTimerHandle TextTransitionTimer;

    // 현재 텍스트를 표시 중인지 여부
    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    bool bIsDisplayingText = false;

    // CSTextWidget 클래스 참조
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings")
    TSubclassOf<class UCSTextWidget> TextWidgetClass;

    // 현재 텍스트 위젯 인스턴스
    UPROPERTY(BlueprintReadOnly, Category = "UI Runtime")
    class UCSTextWidget* CurrentTextWidget;

    // 타이핑 효과 사용 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings")
    bool bUseTypewriterEffect = false;

    // 타이핑 속도 (문자당 시간)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Settings", meta = (ClampMin = "0.01"))
    float TypewriterSpeed = 0.05f;

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
    // 텍스트 시퀀스 시작
    UFUNCTION(BlueprintCallable, Category = "Text")
    void StartTextSequence();

    // 다음 텍스트 표시
    UFUNCTION()
    void ShowNextText();

    // 현재 텍스트 숨기기
    UFUNCTION()
    void HideCurrentText();

    // 텍스트 시퀀스 완료
    UFUNCTION(BlueprintCallable, Category = "Text")
    void CompleteTextSequence();

    // 위젯 생성 및 관리
    UFUNCTION(BlueprintCallable, Category = "UI")
    void CreateTextWidget();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void DestroyTextWidget();

    // 플레이어인지 확인
    UFUNCTION(BlueprintCallable, Category = "Utils")
    bool IsPlayer(AActor* Actor);

public:
    // 텍스트 데이터 추가 (런타임에서 사용 가능)
    UFUNCTION(BlueprintCallable, Category = "Text")
    void AddTextData(const FString& Text, float DisplayDuration = 3.0f, float TransitionSpeed = 1.0f);

    // 트리거 리셋 (재사용 가능하게)
    UFUNCTION(BlueprintCallable, Category = "Trigger")
    void ResetTrigger();

    // 텍스트 시퀀스 강제 중단
    UFUNCTION(BlueprintCallable, Category = "Text")
    void StopTextSequence();

    // Getter 함수들
    UFUNCTION(BlueprintPure, Category = "Text")
    int32 GetTotalTextCount() const { return TextDataArray.Num(); }

    UFUNCTION(BlueprintPure, Category = "Text")
    int32 GetCurrentTextIndex() const { return CurrentTextIndex; }

    UFUNCTION(BlueprintPure, Category = "Text")
    bool IsDisplayingText() const { return bIsDisplayingText; }
};