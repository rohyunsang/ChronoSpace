// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSRotatingActor.h"

// Sets default values
ACSRotatingActor::ACSRotatingActor()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh 컴포넌트 생성 및 루트 컴포넌트로 설정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

}

// Called when the game starts or when spawned
void ACSRotatingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSRotatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 회전 속도 조정
	FRotator RotationDelta = FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f);

	// 로컬 좌표 기준으로 회전 적용
	AddActorLocalRotation(RotationDelta);
}

