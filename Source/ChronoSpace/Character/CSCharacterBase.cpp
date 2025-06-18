// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/CSGASEnergyBarUserWidget.h"
#include "ActorComponent/CSCustomGravityDirComponent.h"
#include "Physics/CSCollision.h"

// Sets default values
ACSCharacterBase::ACSCharacterBase()
{
	// Enable Replication
	bReplicates = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CSCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1500.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 250.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Chibi_characters/Meshes/MainCharacter.MainCharacter'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	// /Script/Engine.SkeletalMesh'/Game/Chibi_characters/Meshes/MainCharacter.MainCharacter'
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Gravity Core
	CustomGravityDirComponent = CreateDefaultSubobject<UCSCustomGravityDirComponent>(TEXT("CustomGravityDirComponent"));
}

void ACSCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
}


void ACSCharacterBase::AttachWindUpKeyToSocket()
{
	// 1. 블루프린트 클래스 로드
	const FString WindUpKeyBPPath = TEXT("/Game/Blueprint/Character/WindUpKey.WindUpKey_C");
	UClass* WindUpKeyBPClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WindUpKeyBPPath));

	if (!WindUpKeyBPClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blueprint class not found! Check the path."));
		return;
	}

	// 2. 블루프린트 액터 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* WindUpKeyActor = GetWorld()->SpawnActor<AActor>(WindUpKeyBPClass, SpawnParams);
	if (!WindUpKeyActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn WindUpKey BP Actor."));
		return;
	}

	// 3. 크기 조정 (스폰된 블루프린트의 루트 컴포넌트에 적용)
	FVector DesiredScale(0.12f, 0.12f, 0.12f);
	WindUpKeyActor->SetActorScale3D(DesiredScale);

	// 4. 소켓 위치 가져오기
	FName SocketName = TEXT("pelvisSocket");
	FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName, RTS_World);

	// 5. 소켓에 블루프린트 액터 부착
	WindUpKeyActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	UE_LOG(LogTemp, Log, TEXT("Blueprint Actor attached to socket: %s"), *SocketName.ToString());
}