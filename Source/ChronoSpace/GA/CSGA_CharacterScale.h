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

    // �����Ƽ Ȱ��ȭ
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    // �����Ƽ ����
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // ũ�� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Character Scale")
    void ChangeCharacterScale(ECharacterScaleType NewScaleType);

protected:
    // ��ǥ ������ Ÿ�� (�������Ʈ���� ����)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    ECharacterScaleType TargetScaleType;

    // ���� ������ Ÿ��
    UPROPERTY(BlueprintReadOnly, Category = "Character Scale")
    ECharacterScaleType CurrentScaleType;

    // �� ������ Ÿ�Ժ� ũ�� ��
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float NormalScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float LargeScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float SmallScale;

    // ������ ��ȭ �ӵ�
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
    // Ÿ�̸� �ڵ�
    FTimerHandle ScaleTransitionTimer;

    // ������ ������ ���� ������
    float StartScale;
    float TargetScale;
    float TransitionTime;
    float ElapsedTime;

    // ���� ������ ���� �Լ�
    void ApplyScale(float NewScale);

    // ������ ���� ������Ʈ
    void UpdateScaleTransition();
	
};
