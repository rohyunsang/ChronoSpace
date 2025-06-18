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

    // ���� ������ Ÿ�� ��������
    UFUNCTION(BlueprintPure, Category = "Character Scale")
    ECharacterScaleType GetCurrentScaleType() const { return CurrentScaleType; }

    // ������ ���� ��û (���� RPC)
    UFUNCTION(BlueprintCallable, Category = "Character Scale")
    void RequestScaleChange(ECharacterScaleType NewScaleType);

    // ���� ������ �� ��������
    UFUNCTION(BlueprintPure, Category = "Character Scale")
    float GetCurrentScaleValue() const;

    // ������ ���� �̺�Ʈ
    UPROPERTY(BlueprintAssignable, Category = "Character Scale")
    FOnCharacterScaleChanged OnCharacterScaleChanged;

protected:
    // ���ø�����Ʈ�Ǵ� ���� ������ Ÿ��
    UPROPERTY(ReplicatedUsing = OnRep_CurrentScaleType, BlueprintReadOnly, Category = "Character Scale")
    ECharacterScaleType CurrentScaleType;

    // ���ø�����Ʈ�Ǵ� ���� ������ ��
    UPROPERTY(ReplicatedUsing = OnRep_CurrentScaleValue, BlueprintReadOnly, Category = "Character Scale")
    float CurrentScaleValue;

    // ������ ����
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float NormalScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float LargeScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float SmallScale;

    // ������ ��ȭ �ӵ�
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Scale")
    float ScaleTransitionSpeed;

private:
    // ���� RPC - ������ ����
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerChangeScale(ECharacterScaleType NewScaleType);

    // ��Ƽĳ��Ʈ RPC - ������ ����
    UFUNCTION(NetMulticast, Reliable)
    void MulticastApplyScale(ECharacterScaleType NewScaleType, float NewScale);

    // ���ø����̼� �Լ���
    UFUNCTION()
    void OnRep_CurrentScaleType(ECharacterScaleType OldScaleType);

    UFUNCTION()
    void OnRep_CurrentScaleValue();

    // ���� ������ ����
    void ApplyScaleToCharacter(float NewScale);

    // ������ Ÿ�Կ��� �� ���
    float GetScaleValueFromType(ECharacterScaleType ScaleType) const;

    // ������ ������ ���� ������
    float StartScale;
    float TargetScale;
    float TransitionTime;
    float ElapsedTime;
    bool bIsTransitioning;

    // Ÿ�̸� �ڵ�
    FTimerHandle ScaleTransitionTimer;

    // ������ ���� ������Ʈ
    void UpdateScaleTransition();

		
};
