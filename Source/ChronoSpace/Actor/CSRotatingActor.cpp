// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSRotatingActor.h"

// Sets default values
ACSRotatingActor::ACSRotatingActor()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
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

	// ȸ�� �ӵ� ����
	FRotator RotationDelta = FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f);

	// ���� ��ǥ �������� ȸ�� ����
	AddActorLocalRotation(RotationDelta);
}

