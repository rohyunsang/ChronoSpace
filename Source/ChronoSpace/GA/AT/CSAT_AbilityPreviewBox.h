// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/AT/CSAT_DurationTask.h"
#include "CSAT_AbilityPreviewBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityPreviewBoxLeftClick, float, BoxSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityPreviewBoxRightClick);

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSAT_AbilityPreviewBox : public UCSAT_DurationTask
{
	GENERATED_BODY()
public:
    UCSAT_AbilityPreviewBox();

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "AbilityPreviewBox", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UCSAT_AbilityPreviewBox* CreateTask(UGameplayAbility* OwningAbility);

    virtual void Activate() override;
    virtual void TickTask(float DeltaTime) override;
    virtual void OnDestroy(bool AbilityEnded) override;

    FAbilityPreviewBoxLeftClick RunAbility;
    FAbilityPreviewBoxRightClick StopAbility;

protected:
    void SetSteticMeshMaterial(class UMaterial* Material, float Scale);

    void HandleLeftMouseClick();
    void HandleRightMouseClick();

    void HandleWheelUp();
    void HandleWheelDown();

    void CreatePreviewBox();
    void CreateStaticMesh();
    void PlayerFollowPreviewBox();
    void AdjustPreviewBoxScale(bool bIncrease);

private:
    UPROPERTY()
    TObjectPtr<class UBoxComponent> PreviewBox;

    UPROPERTY(EditAnywhere, Category = "Box")
    float BoxExtentSize = 200.0f;

    UPROPERTY()
    TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

    bool bIsAbilityPreviewActive = true; // Preview ป๓ลย
    
    float LastScrollTime = 0.0f;

    TArray<float> AllowedSizes = { 50.0f, 100.0f, 150.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f };
    //bool bIsScaled = false;

};
