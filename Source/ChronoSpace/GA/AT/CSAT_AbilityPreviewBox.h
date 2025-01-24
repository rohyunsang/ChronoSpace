// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/AT/CSAT_DurationTask.h"
#include "CSAT_AbilityPreviewBox.generated.h"

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

protected:
    void SetSteticMeshMaterial(class UMaterial* Material, float Scale);
    void HandleLeftMouseClick();
    void HandleRightMouseClick();

private:
    UPROPERTY()
    TObjectPtr<class UBoxComponent> PreviewBox;

    UPROPERTY(EditAnywhere, Category = "Box")
    float BoxExtentSize = 200.0f;

    UPROPERTY()
    TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

    bool bIsAbilityPreviewActive = true; // Preview ป๓ลย

};
