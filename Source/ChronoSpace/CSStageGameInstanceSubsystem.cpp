#include "CSStageGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

UCSStageGameInstanceSubsystem::UCSStageGameInstanceSubsystem()
{
    CurrentStage = "L_Main";  // 기본 스테이지 설정
}

void UCSStageGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    //  스테이지 어빌리티 데이터 초기화
    InitializeStageAbilities();

    UE_LOG(LogTemp, Warning, TEXT("CSStageGameInstanceSubsystem Initialized!"));
}

void UCSStageGameInstanceSubsystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Warning, TEXT("CSStageGameInstanceSubsystem Deinitialized!"));
}

void UCSStageGameInstanceSubsystem::ChangeStage(FString NewStage)
{
    UE_LOG(LogTemp, Warning, TEXT("ChangeStage called with: %s"), *NewStage);

    CurrentStage = NewStage;
    OnStageChanged.Broadcast(); //  UI 업데이트 및 기타 기능 수행

    // 레벨 변경 실행
    UGameplayStatics::OpenLevel(GetWorld(), *NewStage);
}

TArray<FString> UCSStageGameInstanceSubsystem::GetAvailableAbilities()
{
    if (StageAbilities.Contains(CurrentStage))
    {
        return StageAbilities[CurrentStage].Abilities;
    }
    return TArray<FString>();
}

void UCSStageGameInstanceSubsystem::InitializeStageAbilities()
{
    //  예제 데이터 추가
    FStageAbilityList Stage1Abilities;
    Stage1Abilities.Abilities = { "ReverseGravity", "TimeStop" };

    FStageAbilityList Stage2Abilities;
    Stage2Abilities.Abilities = { "TimeStop" };

    StageAbilities.Add("L_Stage1", Stage1Abilities);
    StageAbilities.Add("L_Stage2", Stage2Abilities);
}
