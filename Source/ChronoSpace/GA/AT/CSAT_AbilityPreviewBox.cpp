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

    // BoxComponent 생성 및 설정
    PreviewBox = NewObject<UBoxComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (PreviewBox)
    {
        PreviewBox->RegisterComponent();
        PreviewBox->SetBoxExtent(FVector(BoxExtentSize, BoxExtentSize, BoxExtentSize));
        PreviewBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        PreviewBox->AttachToComponent(GetWorld()->GetFirstPlayerController()->GetPawn()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        PreviewBox->SetRelativeLocation(FVector(350.0f, 0.0f, 100.0f));
    }

    // StaticMeshComponent 생성 및 설정
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
        UMaterial* MaterialRef = Cast<UMaterial>(
            StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("/Game/Material/MAT_ChronoControlPreview.MAT_ChronoControlPreview"))
        );
        if (MaterialRef)
        {
            SetSteticMeshMaterial(MaterialRef, MeshScale.X);
        }

    }

    UE_LOG(LogTemp, Log, TEXT("PreviewBox and StaticMeshComp created dynamically with material"));
}


void UCSAT_AbilityPreviewBox::TickTask(float DeltaTime)
{

    Super::TickTask(DeltaTime);

    if (!PreviewBox || !StaticMeshComp || !Ability || !Ability->GetCurrentActorInfo())
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewBox or StaticMeshComp or Ability is null"));
        return; // 유효하지 않은 경우 Tick 종료
    }

    // 플레이어의 앞을 따라다니게 설정
    if (Ability && Ability->GetCurrentActorInfo()->AvatarActor.IsValid())
    {
        AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
        FVector ForwardVector = AvatarActor->GetActorForwardVector();
        FVector ActorLocation = AvatarActor->GetActorLocation();
        //UE_LOG(LogTemp, Log, TEXT("Player Location: X=%.2f, Y=%.2f, Z=%.2f"), ActorLocation.X, ActorLocation.Y, ActorLocation.Z);
        FVector PlayerOffSet(0.0f, 0.0f, 100.0f);
        FVector NewLocation = ActorLocation + PlayerOffSet + ForwardVector * 350.0f ;
        UE_LOG(LogTemp, Log, TEXT("Is Run Tick Task"));

        PreviewBox->SetWorldLocation(NewLocation);
    }

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



void UCSAT_AbilityPreviewBox::HandleLeftMouseClick()   // 새로운 어빌리티 시작.
{
    OnLeftClick.Broadcast();
    EndTask();
}

void UCSAT_AbilityPreviewBox::HandleRightMouseClick()  // 어빌리티 실행 취소 
{
    // 종료 처리
    OnRightClick.Broadcast();
    EndTask();
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

