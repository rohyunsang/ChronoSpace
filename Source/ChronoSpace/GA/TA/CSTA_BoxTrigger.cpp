// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_BoxTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/CSCollision.h"
#include "ChronoSpace.h"

ACSTA_BoxTrigger::ACSTA_BoxTrigger()
{
    bReplicates = true;

	// Trigger
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	RootComponent = BoxTrigger;
	BoxTrigger->SetBoxExtent(FVector(BoxExtentSize, BoxExtentSize, BoxExtentSize));
	BoxTrigger->SetRelativeLocation(FVector(350.0f, 0.0f, 100.0f));
	BoxTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
    BoxTrigger->SetIsReplicated(true);

	// 오버랩 이벤트 서브 클래스에서 추가 필요
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
    float HalfSizeOfSide = 50.0f; // SM_Cube 기본 사이즈가 100x100x100 -> 반경(Extent) 기준 50
    FVector LocationOffset = FVector(-HalfSizeOfSide, -HalfSizeOfSide, -HalfSizeOfSide);
    MeshScale = BoxExtent / HalfSizeOfSide;

    StaticMeshComp->SetRelativeLocation(LocationOffset * MeshScale);
    StaticMeshComp->SetRelativeScale3D(MeshScale);

    // 서브 클래스에서 SetSteticMeshMaterial 호출해서 매테리얼 설정
    // SetSteticMeshMaterial(~, MeshScale.X)
}

void ACSTA_BoxTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (!bShowDebug) return;

    if (BoxTrigger)
    {
        FVector BoxLocation = BoxTrigger->GetComponentLocation();
        FVector BoxExtent = BoxTrigger->GetScaledBoxExtent(); // 스케일이 적용된 박스 크기
        FQuat   BoxRotation = BoxTrigger->GetComponentRotation().Quaternion();

        DrawDebugBox(
            GetWorld(),
            BoxLocation,
            BoxExtent,
            BoxRotation,
            FColor::Green,
            false,  // 지속 표시
            1,
            0,     // 디버그 선 우선순위
            2.0f   // 선 두께
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
