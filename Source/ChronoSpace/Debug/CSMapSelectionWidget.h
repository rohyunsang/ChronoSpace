// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSMapSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSMapSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

protected:

    UPROPERTY(meta = (BindWidget))
    class UButton* MapButton1;

    UPROPERTY(meta = (BindWidget))
    class UButton* MapButton2;

    UPROPERTY(meta = (BindWidget))
    class UButton* MapButton3;

    UPROPERTY(EditAnywhere, Category = "Map")
    FName Map1Name = "Map_A";

    UPROPERTY(EditAnywhere, Category = "Map")
    FName Map2Name = "Map_B";

    UPROPERTY(EditAnywhere, Category = "Map")
    FName Map3Name = "Map_C";

    // Çì´õ
    UFUNCTION()
    void OnMap1Clicked();

    UFUNCTION()
    void OnMap2Clicked();

    UFUNCTION()
    void OnMap3Clicked();

    void TravelToMap(FName MapName);
};
