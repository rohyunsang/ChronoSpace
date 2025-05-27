#include "CSStageGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

UCSStageGameInstanceSubsystem::UCSStageGameInstanceSubsystem()
{
    CurrentStage = "L_Main";  // �⺻ �������� ����

    //  AbilityHUDClass�� NULL�̸� �⺻ �������Ʈ Ŭ���� ����
    if (!AbilityHUDClass)
    {
        // /Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UI/BP_AbilityHUD.BP_AbilityHUD'
        static ConstructorHelpers::FClassFinder<UCSAbilityHUD> HUD_BP(TEXT("/Game/Blueprint/UI/BP_AbilityHUD"));
        if (HUD_BP.Succeeded())
        {
            AbilityHUDClass = HUD_BP.Class;
            UE_LOG(LogTemp, Warning, TEXT("AbilityHUDClass initialized from Blueprint!"));
        }
    }
}

void UCSStageGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    //  �������� �����Ƽ ������ �ʱ�ȭ
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
    OnStageChanged.Broadcast(); //  UI ������Ʈ �� ��Ÿ ��� ����

    // ���� ���� ����
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
    //  ���� ������ �߰�
    FStageAbilityList Stage1Abilities;
    Stage1Abilities.Abilities = { "ReverseGravity", "TimeStop" };

    FStageAbilityList Stage2Abilities;
    Stage2Abilities.Abilities = { "TimeStop" };

    StageAbilities.Add("L_Stage1", Stage1Abilities);
    StageAbilities.Add("L_Stage2", Stage2Abilities);
}
