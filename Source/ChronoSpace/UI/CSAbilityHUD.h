// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/CSStageManager.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "CSAbilityHUD.generated.h"

UCLASS()
class CHRONOSPACE_API UCSAbilityHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    // 어빌리티 UI를 표시할 컨테이너
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* AbilityContainer;

    // 어빌리티 UI 요소 (이미지 + 텍스트)
    UPROPERTY(EditDefaultsOnly, Category = "AbilityUI")
    TSubclassOf<UUserWidget> AbilityWidgetClass;

    // 현재 사용 가능한 어빌리티 목록
    UPROPERTY(BlueprintReadWrite, Category = "AbilityUI")
    TArray<FString> AvailableAbilities;

    // 현재 선택된 어빌리티 인덱스
    UPROPERTY(BlueprintReadWrite, Category = "AbilityUI")
    int32 SelectedAbilityIndex;

    // UI 초기화 함수 (스테이지 변경 시 호출됨)
    UFUNCTION(BlueprintCallable, Category = "AbilityUI")
    void InitializeAbilityUI();

protected:
    virtual void NativeConstruct() override;

private:
    // 마우스 휠 입력 처리
    UFUNCTION()
    void ChangeAbilitySelection(float AxisValue);

    // 어빌리티 실행
    UFUNCTION()
    void UseSelectedAbility();
};