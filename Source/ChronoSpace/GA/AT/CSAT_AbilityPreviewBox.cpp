// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/AT/CSAT_AbilityPreviewBox.h"
#include "GA/CSGA_AbilityPreviewBox.h"
#include "GA/CSGA_ChronoControl.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/World.h"
#include "ChronoSpace.h"

UCSAT_AbilityPreviewBox::UCSAT_AbilityPreviewBox()
{
    // TickTask 활성화
    bTickingTask = true;
}

UCSAT_AbilityPreviewBox* UCSAT_AbilityPreviewBox::CreateTask(UGameplayAbility* OwningAbility)
{
    UCSAT_AbilityPreviewBox* Task = NewAbilityTask<UCSAT_AbilityPreviewBox>(OwningAbility);
    return Task;
}

void UCSAT_AbilityPreviewBox::SetSteticMeshMaterial(UMaterial* Material, float Scale)
{
    if (Material)
    {
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

        if (DynamicMaterial)
        {
            float OutBaseValue = 2.0f;
            DynamicMaterial->GetScalarParameterValue(FName(TEXT("Tiling")), OutBaseValue);
            DynamicMaterial->SetScalarParameterValue(FName(TEXT("Tiling")), OutBaseValue * Scale);
            StaticMeshComp->SetMaterial(0, DynamicMaterial);
        }
    }
}

void UCSAT_AbilityPreviewBox::Activate()
{
    Super::Activate();

    CreatePreviewBox();
    CreateStaticMesh();

    // 입력 바인딩 추가
    if (Ability && Ability->GetCurrentActorInfo()->PlayerController.IsValid())
    {
        APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
        if (PlayerController && PlayerController->InputComponent)
        {
            PlayerController->InputComponent->BindAction("WheelUp", IE_Pressed, this, &UCSAT_AbilityPreviewBox::HandleWheelUp);
            PlayerController->InputComponent->BindAction("WheelDown", IE_Pressed, this, &UCSAT_AbilityPreviewBox::HandleWheelDown);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("PreviewBox and StaticMeshComp created dynamically with material"));
}


void UCSAT_AbilityPreviewBox::CreatePreviewBox()
{
    PreviewBox = NewObject<UBoxComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (PreviewBox)
    {
        PreviewBox->RegisterComponent();
        PreviewBox->SetBoxExtent(FVector(BoxExtentSize, BoxExtentSize, BoxExtentSize));
        PreviewBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        PreviewBox->AttachToComponent(GetWorld()->GetFirstPlayerController()->GetPawn()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        PreviewBox->SetRelativeLocation(FVector(350.0f, 0.0f, 100.0f));
    }
}

void UCSAT_AbilityPreviewBox::CreateStaticMesh()
{
    StaticMeshComp = NewObject<UStaticMeshComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (StaticMeshComp)
    {
        StaticMeshComp->RegisterComponent();
        StaticMeshComp->AttachToComponent(PreviewBox, FAttachmentTransformRules::KeepRelativeTransform);

        // 런타임에서 Static Mesh 로드
        UStaticMesh* StaticMeshRef = Cast<UStaticMesh>(
            StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Mesh/StaticMesh/SM_Cube.SM_Cube"))
        );
        if (StaticMeshRef)
        {
            StaticMeshComp->SetStaticMesh(StaticMeshRef);
        }

        FVector BoxExtent(BoxExtentSize, BoxExtentSize, BoxExtentSize);
        float HalfSizeOfSide = 50.0f; // 기본 박스 크기
        FVector LocationOffset = FVector(-HalfSizeOfSide, -HalfSizeOfSide, -HalfSizeOfSide);
        FVector MeshScale = BoxExtent / HalfSizeOfSide;

        StaticMeshComp->SetRelativeLocation(LocationOffset * MeshScale);
        StaticMeshComp->SetRelativeScale3D(MeshScale);

        // 런타임에서 메테리얼 로드
        if ( Cast<UCSGA_AbilityPreviewBox>(Ability)->Ability == EAbilityIndex::ChronoControl )
        {
            UMaterial* MaterialRef = Cast<UMaterial>(
                StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("/Game/Material/MAT_ChronoControlPreview.MAT_ChronoControlPreview"))
            );
            if (MaterialRef)
            {
                SetSteticMeshMaterial(MaterialRef, MeshScale.X);
            }
        }
        else if( Cast<UCSGA_AbilityPreviewBox>(Ability)->Ability == EAbilityIndex::ReverseGravity )
        {
            UMaterial* MaterialRef = Cast<UMaterial>(
                StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("/Game/Material/MAT_AntyGravityPreview.MAT_AntyGravityPreview"))
            );
            if (MaterialRef)
            {
                SetSteticMeshMaterial(MaterialRef, MeshScale.X);
            }
        }
        else if ( Cast<UCSGA_AbilityPreviewBox>(Ability)->Ability == EAbilityIndex::WeakenGravity50P )
        {
            UMaterial* MaterialRef = Cast<UMaterial>(
                StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("/Game/Material/MAT_WeakenGravityPreview50P.MAT_WeakenGravityPreview50P"))
            );
            if (MaterialRef)
            {
                SetSteticMeshMaterial(MaterialRef, MeshScale.X);
            }
        }
        else
        {
            UMaterial* MaterialRef = Cast<UMaterial>(
                StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("/Game/Material/MAT_WeakenGravityPreview10P.MAT_WeakenGravityPreview10P"))
            );
            if (MaterialRef)
            {
                SetSteticMeshMaterial(MaterialRef, MeshScale.X);
            }
        }

        
    }
}
void UCSAT_AbilityPreviewBox::PlayerFollowPreviewBox()
{
    if (!PreviewBox || !StaticMeshComp || !Ability || !Ability->GetCurrentActorInfo())
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewBox or StaticMeshComp or Ability is null"));
        return;
    }

    AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
    if (!AvatarActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AvatarActor is null"));
        return;
    }

    // 허용된 크기 값 배열과 오프셋 매핑
    TArray<float> YOffsets = { 150.0f, 100.0f, 50.0f, 0.0f, -50.0f, -100.0f, -150.0f, -200.0f, -250.0f, -300.0f };

    // PreviewBox 크기 가져오기
    float CurrentSize = PreviewBox->GetUnscaledBoxExtent().X;

    // 현재 크기에 해당하는 오프셋을 찾음
    int32 CurrentIndex = AllowedSizes.IndexOfByPredicate([CurrentSize](float Size) {
        return FMath::IsNearlyEqual(Size, CurrentSize, KINDA_SMALL_NUMBER);
        });

    float YOffset = 0.0f;
    if (CurrentIndex != INDEX_NONE)
    {
        YOffset = YOffsets[CurrentIndex];
    }

    // 위치 계산
    FVector ForwardVector = AvatarActor->GetActorForwardVector();
    FVector RightVector = AvatarActor->GetActorRightVector(); // 플레이어의 오른쪽 방향 벡터
    FVector ActorLocation = AvatarActor->GetActorLocation();

    // ForwardVector와 RightVector를 기반으로 PlayerOffset 계산
    FVector ForwardOffset = ForwardVector * 350.0f; // 플레이어 앞 방향으로 350 단위
    FVector RightOffset = RightVector * YOffset;   // 오른쪽 방향으로 YOffset 만큼 이동
    FVector UpOffset(0.0f, 0.0f, 100.0f);          // 위쪽으로 100 단위

    FVector NewLocation = ActorLocation + ForwardOffset + RightOffset + UpOffset;

    // 박스 위치 업데이트
    PreviewBox->SetWorldLocation(NewLocation);

    // 디버그 로그
    //UE_LOG(LogTemp, Log, TEXT("ActorLocation: X=%.2f, Y=%.2f, Z=%.2f"), ActorLocation.X, ActorLocation.Y, ActorLocation.Z);
    //UE_LOG(LogTemp, Log, TEXT("ForwardOffset: X=%.2f, Y=%.2f, Z=%.2f"), ForwardOffset.X, ForwardOffset.Y, ForwardOffset.Z);
    //UE_LOG(LogTemp, Log, TEXT("RightOffset: X=%.2f, Y=%.2f, Z=%.2f"), RightOffset.X, RightOffset.Y, RightOffset.Z);
    //UE_LOG(LogTemp, Log, TEXT("NewLocation: X=%.2f, Y=%.2f, Z=%.2f, YOffset: %.2f"), NewLocation.X, NewLocation.Y, NewLocation.Z, YOffset);
}

void UCSAT_AbilityPreviewBox::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    PlayerFollowPreviewBox();

    // 입력 처리
    if (Ability->GetCurrentActorInfo()->PlayerController->IsInputKeyDown(EKeys::LeftMouseButton))
    {
        HandleLeftMouseClick();
        return;
    }
    if (Ability->GetCurrentActorInfo()->PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
    {
        HandleRightMouseClick();
        return;
    }
}

void UCSAT_AbilityPreviewBox::AdjustPreviewBoxScale(bool bIncrease)
{
    if (!PreviewBox || !StaticMeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewBox or StaticMeshComp is null during scale adjustment"));
        return;
    }

    // 현재 BoxExtent의 X 크기를 가져옵니다.
    float CurrentSize = PreviewBox->GetUnscaledBoxExtent().X;

    // 현재 크기의 인덱스를 찾습니다.
    int32 CurrentIndex = AllowedSizes.IndexOfByPredicate([CurrentSize](float Size) {
        return FMath::IsNearlyEqual(Size, CurrentSize, KINDA_SMALL_NUMBER);
        });

    // 현재 인덱스가 유효한 경우 크기 조정
    if (CurrentIndex != INDEX_NONE)
    {
        if (bIncrease && CurrentIndex < AllowedSizes.Num() - 1)
        {
            CurrentIndex++; // 크기 증가
        }
        else if (!bIncrease && CurrentIndex > 0)
        {
            CurrentIndex--; // 크기 감소
        }
    }

    // 새로운 크기를 설정
    float NewSize = AllowedSizes[CurrentIndex];
    FVector NewExtent(NewSize, NewSize, NewSize);
    PreviewBox->SetBoxExtent(NewExtent);

    // Static Mesh 크기도 변경
    FVector MeshScale = NewExtent / 50.0f; // 기본 박스 크기를 기준으로 스케일 계산
    StaticMeshComp->SetRelativeScale3D(MeshScale);

    UE_LOG(LogTemp, Log, TEXT("PreviewBox Scale Adjusted: Current Size: %.2f, New Size: %.2f"), CurrentSize, NewSize);
    UE_LOG(LogTemp, Log, TEXT("PreviewBox Adjusted Extent: X=%.2f, Y=%.2f, Z=%.2f"), NewExtent.X, NewExtent.Y, NewExtent.Z);
}

void UCSAT_AbilityPreviewBox::HandleLeftMouseClick()   // 새로운 어빌리티 시작
{
    float CurrentBoxSize = PreviewBox->GetUnscaledBoxExtent().X; // 현재 박스 크기
    RunAbility.Broadcast(CurrentBoxSize); // 현재 박스 크기를 매개변수로 전달
    EndTask();
}

void UCSAT_AbilityPreviewBox::HandleRightMouseClick()  // 어빌리티 실행 취소 
{
    // 종료 처리
    StopAbility.Broadcast();
    EndTask();
}

void UCSAT_AbilityPreviewBox::HandleWheelUp()
{
    AdjustPreviewBoxScale(true); // 크기 증가
    UE_LOG(LogTemp, Log, TEXT("Wheel Up Pressed"));
}

void UCSAT_AbilityPreviewBox::HandleWheelDown()
{
    AdjustPreviewBoxScale(false); // 크기 감소
    UE_LOG(LogTemp, Log, TEXT("Wheel Down Pressed"));
}

void UCSAT_AbilityPreviewBox::OnDestroy(bool AbilityEnded)
{
    if (PreviewBox)
    {
        PreviewBox->DestroyComponent();
        PreviewBox = nullptr; // Null로 설정하여 다시 생성되도록 함
    }

    if (StaticMeshComp)
    {
        StaticMeshComp->DestroyComponent();
        StaticMeshComp = nullptr; // Null로 설정하여 다시 생성되도록 함
    }
    
    UE_LOG(LogTemp, Log, TEXT("UCSAT_AbilityPreviewBox destroyed"));

    Super::OnDestroy(AbilityEnded);
}

