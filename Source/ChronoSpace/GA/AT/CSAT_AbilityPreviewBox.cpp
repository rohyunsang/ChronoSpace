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
    // TickTask Ȱ��ȭ
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

    // �Է� ���ε� �߰�
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
        StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        StaticMeshComp->RegisterComponent();
        StaticMeshComp->AttachToComponent(PreviewBox, FAttachmentTransformRules::KeepRelativeTransform);

        // ��Ÿ�ӿ��� Static Mesh �ε�
        UStaticMesh* StaticMeshRef = Cast<UStaticMesh>(
            StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Mesh/StaticMesh/SM_Cube.SM_Cube"))
        );
        if (StaticMeshRef)
        {
            StaticMeshComp->SetStaticMesh(StaticMeshRef);
        }

        FVector BoxExtent(BoxExtentSize, BoxExtentSize, BoxExtentSize);
        float HalfSizeOfSide = 50.0f; // �⺻ �ڽ� ũ��
        FVector LocationOffset = FVector(-HalfSizeOfSide, -HalfSizeOfSide, -HalfSizeOfSide);
        FVector MeshScale = BoxExtent / HalfSizeOfSide;

        StaticMeshComp->SetRelativeLocation(LocationOffset * MeshScale);
        StaticMeshComp->SetRelativeScale3D(MeshScale);

        // ��Ÿ�ӿ��� ���׸��� �ε�
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

    // ���� ũ�� �� �迭�� ������ ����
    TArray<float> YOffsets = { 150.0f, 100.0f, 50.0f, 0.0f, -50.0f, -100.0f, -150.0f, -200.0f, -250.0f, -300.0f };

    // PreviewBox ũ�� ��������
    float CurrentSize = PreviewBox->GetUnscaledBoxExtent().X;

    // ���� ũ�⿡ �ش��ϴ� �������� ã��
    int32 CurrentIndex = AllowedSizes.IndexOfByPredicate([CurrentSize](float Size) {
        return FMath::IsNearlyEqual(Size, CurrentSize, KINDA_SMALL_NUMBER);
        });

    float YOffset = 0.0f;
    if (CurrentIndex != INDEX_NONE)
    {
        YOffset = YOffsets[CurrentIndex];
    }

    // ��ġ ���
    FVector ForwardVector = AvatarActor->GetActorForwardVector();
    FVector RightVector = AvatarActor->GetActorRightVector(); // �÷��̾��� ������ ���� ����
    FVector ActorLocation = AvatarActor->GetActorLocation();

    // ForwardVector�� RightVector�� ������� PlayerOffset ���
    FVector ForwardOffset = ForwardVector * 350.0f; // �÷��̾� �� �������� 350 ����
    FVector UpOffset(0.0f, 0.0f, 100.0f + (CurrentSize - 200.0f));          // �������� 100 ����

    FVector NewLocation = ActorLocation + ForwardOffset /*+ RightOffset*/ + UpOffset;

    // �ڽ� ��ġ ������Ʈ
    PreviewBox->SetWorldLocation(NewLocation);

    // ����� �α�
    //UE_LOG(LogTemp, Log, TEXT("ActorLocation: X=%.2f, Y=%.2f, Z=%.2f"), ActorLocation.X, ActorLocation.Y, ActorLocation.Z);
    //UE_LOG(LogTemp, Log, TEXT("ForwardOffset: X=%.2f, Y=%.2f, Z=%.2f"), ForwardOffset.X, ForwardOffset.Y, ForwardOffset.Z);
    //UE_LOG(LogTemp, Log, TEXT("RightOffset: X=%.2f, Y=%.2f, Z=%.2f"), RightOffset.X, RightOffset.Y, RightOffset.Z);
    //UE_LOG(LogTemp, Log, TEXT("NewLocation: X=%.2f, Y=%.2f, Z=%.2f, YOffset: %.2f"), NewLocation.X, NewLocation.Y, NewLocation.Z, YOffset);
}

void UCSAT_AbilityPreviewBox::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    PlayerFollowPreviewBox();

    // �Է� ó��
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

    // ���� BoxExtent�� X ũ�⸦ �����ɴϴ�.
    float CurrentSize = PreviewBox->GetUnscaledBoxExtent().X;

    // ���� ũ���� �ε����� ã���ϴ�.
    int32 CurrentIndex = AllowedSizes.IndexOfByPredicate([CurrentSize](float Size) {
        return FMath::IsNearlyEqual(Size, CurrentSize, KINDA_SMALL_NUMBER);
        });

    // ���� �ε����� ��ȿ�� ��� ũ�� ����
    if (CurrentIndex != INDEX_NONE)
    {
        if (bIncrease && CurrentIndex < AllowedSizes.Num() - 1)
        {
            CurrentIndex++; // ũ�� ����
        }
        else if (!bIncrease && CurrentIndex > 0)
        {
            CurrentIndex--; // ũ�� ����
        }
    }

    // ���ο� ũ�⸦ ����
    float NewSize = AllowedSizes[CurrentIndex];
    FVector NewExtent(NewSize, NewSize, NewSize);
    PreviewBox->SetBoxExtent(NewExtent);

    // Static Mesh ũ�⵵ ����
    float HalfSizeOfSide = 50.0f;
    FVector MeshScale = NewExtent / HalfSizeOfSide; // �⺻ �ڽ� ũ�⸦ �������� ������ ���
    StaticMeshComp->SetRelativeScale3D(MeshScale);
    FVector LocationOffset = FVector(-HalfSizeOfSide, -HalfSizeOfSide, -HalfSizeOfSide);
    StaticMeshComp->SetRelativeLocation(LocationOffset * MeshScale);

    UE_LOG(LogTemp, Log, TEXT("PreviewBox Scale Adjusted: Current Size: %.2f, New Size: %.2f"), CurrentSize, NewSize);
    UE_LOG(LogTemp, Log, TEXT("PreviewBox Adjusted Extent: X=%.2f, Y=%.2f, Z=%.2f"), NewExtent.X, NewExtent.Y, NewExtent.Z);
}

void UCSAT_AbilityPreviewBox::HandleLeftMouseClick()   // ���ο� �����Ƽ ����
{
    float CurrentBoxSize = PreviewBox->GetUnscaledBoxExtent().X; // ���� �ڽ� ũ��
    RunAbility.Broadcast(CurrentBoxSize); // ���� �ڽ� ũ�⸦ �Ű������� ����
    EndTask();
}

void UCSAT_AbilityPreviewBox::HandleRightMouseClick()  // �����Ƽ ���� ��� 
{
    // ���� ó��
    StopAbility.Broadcast();
    EndTask();
}

void UCSAT_AbilityPreviewBox::HandleWheelUp()
{
    AdjustPreviewBoxScale(true); // ũ�� ����
    UE_LOG(LogTemp, Log, TEXT("Wheel Up Pressed"));
}

void UCSAT_AbilityPreviewBox::HandleWheelDown()
{
    AdjustPreviewBoxScale(false); // ũ�� ����
    UE_LOG(LogTemp, Log, TEXT("Wheel Down Pressed"));
}

void UCSAT_AbilityPreviewBox::OnDestroy(bool AbilityEnded)
{
    if (PreviewBox)
    {
        PreviewBox->DestroyComponent();
        PreviewBox = nullptr; // Null�� �����Ͽ� �ٽ� �����ǵ��� ��
    }

    if (StaticMeshComp)
    {
        StaticMeshComp->DestroyComponent();
        StaticMeshComp = nullptr; // Null�� �����Ͽ� �ٽ� �����ǵ��� ��
    }
    
    UE_LOG(LogTemp, Log, TEXT("UCSAT_AbilityPreviewBox destroyed"));

    Super::OnDestroy(AbilityEnded);
}

