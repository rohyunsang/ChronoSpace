// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSGravityCoreSphere.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/CSCollision.h"


ACSGravityCoreSphere::ACSGravityCoreSphere()
{
	// GravitySphereTrigger
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("GravitySphereTrigger"));
	SphereTrigger->SetSphereRadius(GravityInfluenceRange, true);
	SphereTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	RootComponent = SphereTrigger;

	// Static Mesh
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComp->SetupAttachment(SphereTrigger);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/BlockSphere.BlockSphere'"));
	if (StaticMeshRef.Object)
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	float MeshRadius = 50.0f;
	float MeshScale = (GravityInfluenceRange / MeshRadius) * 0.75f;
	StaticMeshComp->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));

}