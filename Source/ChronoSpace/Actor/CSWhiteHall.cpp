// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSWhiteHall.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACSWhiteHall::ACSWhiteHall()
{
	bReplicates = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/MaterialSphere.MaterialSphere'"));
	if (StaticMeshRef.Object)
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MI_WhiteHole.MI_WhiteHole'"));
	if (MaterialRef.Object)
	{
		StaticMeshComp->SetMaterial(0, MaterialRef.Object);
	}
}


