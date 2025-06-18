// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_BoxTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/CSCollision.h"
#include "DataAsset/CSDA_BoxProperties.h"
#include "ChronoSpace.h"

ACSTA_BoxTrigger::ACSTA_BoxTrigger()
{
    bReplicates = true;

    const FString AssetPath = TEXT("/Game/DataAssets/CSDA_BoxProperties.CSDA_BoxProperties");
    UCSDA_BoxProperties* BoxPropertiesAsset = Cast<UCSDA_BoxProperties>(StaticLoadObject(UCSDA_BoxProperties::StaticClass(), nullptr, *AssetPath));
    
    BoxExtentSize = 200.0f; // default value

    if (BoxPropertiesAsset)
    {
        BoxExtentSize = BoxPropertiesAsset->BoxSize;
        UE_LOG(LogCS, Log, TEXT("BoxExtentSize loaded from DataAsset: %f"), BoxExtentSize);
    }
    else
    {
        UE_LOG(LogCS, Error, TEXT("Failed to load CSDA_BoxProperties at path: %s"), *AssetPath);
    }


	// Trigger
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	RootComponent = BoxTrigger;
	BoxTrigger->SetBoxExtent(FVector(BoxExtentSize, BoxExtentSize, BoxExtentSize));
	BoxTrigger->SetRelativeLocation(FVector(350.0f, 0.0f, 100.0f + (BoxExtentSize - 200.0f)));
	BoxTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
    BoxTrigger->SetIsReplicated(true);

	//BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerBeginOverlap);
	//BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerEndOverlap);

    // Static Mesh
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComp->SetupAttachment(BoxTrigger);
    StaticMeshComp->SetIsReplicated(true);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/SM_Cube.SM_Cube'"));
    if ( StaticMeshRef.Object )
    {
        StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
    }

    FVector BoxExtent = FVector(BoxExtentSize, BoxExtentSize, BoxExtentSize);
    float HalfSizeOfSide = 50.0f; // SM_Cube �⺻ ����� 100x100x100 -> �ݰ�(Extent) ���� 50
    FVector LocationOffset = FVector(-HalfSizeOfSide, -HalfSizeOfSide, -HalfSizeOfSide);
    MeshScale = BoxExtent / HalfSizeOfSide;

    StaticMeshComp->SetRelativeLocation(LocationOffset * MeshScale);
    StaticMeshComp->SetRelativeScale3D(MeshScale);

    // ���� Ŭ�������� SetSteticMeshMaterial ȣ���ؼ� ���׸��� ����
    // SetSteticMeshMaterial(~, MeshScale.X)
}

void ACSTA_BoxTrigger::BeginPlay()
{
	Super::BeginPlay();
    
    UE_LOG(LogCS, Log, TEXT("BoxTrigger, %f, %f, %f"), BoxTrigger->GetComponentLocation().X, BoxTrigger->GetComponentLocation().Y, BoxTrigger->GetComponentLocation().Z);

	if (!bShowDebug) return;

    if (BoxTrigger)
    {
        FVector BoxLocation = BoxTrigger->GetComponentLocation();
        FVector BoxExtent = BoxTrigger->GetScaledBoxExtent(); // �������� ����� �ڽ� ũ��
        FQuat   BoxRotation = BoxTrigger->GetComponentRotation().Quaternion();

        DrawDebugBox(
            GetWorld(),
            BoxLocation,
            BoxExtent,
            BoxRotation,
            FColor::Green,
            false,  // ���� ǥ��
            1,
            0,     // ����� �� �켱����
            2.0f   // �� �β�
        );
    }
}

void ACSTA_BoxTrigger::SetSteticMeshMaterial(class UMaterial* Material, float Scale)
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
