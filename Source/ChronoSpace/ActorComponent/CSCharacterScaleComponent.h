// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GA/CSGA_CharacterScale.h"
#include "CSCharacterScaleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterScaleChanged, ECharacterScaleType, OldScale, ECharacterScaleType, NewScale);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOSPACE_API UCSCharacterScaleComponent : public UActorComponent
{
	GENERATED_BODY()
public:
    // Sets default values for this component's properties
    UCSCharacterScaleComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 현재 스케일 타입 가져오기
    UFUNCTION(BlueprintPure, Category = "Character Scale")
    ECharacterScaleType GetCurrentScaleType() const { return CurrentScaleType; }

    // 스케일 변경 요청 (서버 RPC)
    UFUNCTION(BlueprintCallable, Category = "Character Scale")
    void RequestScaleChange(ECharacterScaleType NewScaleType);

    // 현재 스케일 값 가져오기
    UFUNCTION(BlueprintPure, Category = "Character Scale")
    float GetCurrentScaleValue() const;

    // 스케일 변경 이벤트
    UPROPERTY(BlueprintAssignable, Category = "Character Scale")
    FOnCharacterScaleChanged OnCharacterScaleChanged;

protected:
    // 리플리케이트되는 현재 스케일 타입
    UPROPERTY(ReplicatedUsing = OnRep_CurrentScaleType, BlueprintReadOnly, Category = "Character Scale")
    ECharacterScaleType CurrentScaleType;

    // 리플리케이트되는 현재 스케일 값
    UPROPERTY(ReplicatedUsing = OnRep_CurrentScaleValue, BlueprintReadOnly, Category = "Character Scale")
    float CurrentScaleValue;

    // 스케일 설정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float NormalScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float LargeScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float SmallScale;

    // 스케일 변화 속도
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float ScaleTransitionSpeed;

private:
    // 서버 RPC - 스케일 변경
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerChangeScale(ECharacterScaleType NewScaleType);

    // 멀티캐스트 RPC - 스케일 적용
    UFUNCTION(NetMulticast, Reliable)
    void MulticastApplyScale(ECharacterScaleType NewScaleType, float NewScale);

    // 리플리케이션 함수들
    UFUNCTION()
    void OnRep_CurrentScaleType(ECharacterScaleType OldScaleType);

    UFUNCTION()
    void OnRep_CurrentScaleValue();

    // 실제 스케일 적용
    void ApplyScaleToCharacter(float NewScale);

    // 스케일 타입에서 값 계산
    float GetScaleValueFromType(ECharacterScaleType ScaleType) const;

    // 스케일 보간을 위한 변수들
    float StartScale;
    float TargetScale;
    float TransitionTime;
    float ElapsedTime;
    bool bIsTransitioning;

    // 타이머 핸들
    FTimerHandle ScaleTransitionTimer;

    // 스케일 보간 업데이트
    void UpdateScaleTransition();

		
};
