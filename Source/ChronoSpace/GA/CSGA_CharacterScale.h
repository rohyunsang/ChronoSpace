// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "CSGA_CharacterScale.generated.h"

UENUM(BlueprintType)
enum class ECharacterScaleType : uint8
{
    Small UMETA(DisplayName = "Small"),
    Normal UMETA(DisplayName = "Normal"),
    Large UMETA(DisplayName = "Large")
};


/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSGA_CharacterScale : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UCSGA_CharacterScale();

    // 어빌리티 활성화
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    // 어빌리티 종료
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // 크기 변경 함수
    UFUNCTION(BlueprintCallable, Category = "Character Scale")
    void ChangeCharacterScale(ECharacterScaleType NewScaleType);

protected:
    // 목표 스케일 타입 (블루프린트에서 설정)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    ECharacterScaleType TargetScaleType;

    // 현재 스케일 타입
    UPROPERTY(BlueprintReadOnly, Category = "Character Scale")
    ECharacterScaleType CurrentScaleType;

    // 각 스케일 타입별 크기 값
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float NormalScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float LargeScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float SmallScale;

    // 스케일 변화 속도
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float ScaleTransitionSpeed;

    // Gameplay Tags
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
    FGameplayTag ScaleLargeTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
    FGameplayTag ScaleSmallTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
    FGameplayTag ScaleNormalTag;

private:
    // 타이머 핸들
    FTimerHandle ScaleTransitionTimer;

    // 스케일 보간을 위한 변수들
    float StartScale;
    float TargetScale;
    float TransitionTime;
    float ElapsedTime;

    // 실제 스케일 적용 함수
    void ApplyScale(float NewScale);

    // 스케일 보간 업데이트
    void UpdateScaleTransition();
	
};
