// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSCharacterPatrol.h"
#include "ChronoSpace.h"
#include "Player/CSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/CSCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Character/CSCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CSAttributeSet.h"
#include "Components/StaticMeshComponent.h"
#include "DataAsset/CSCharacterPatrolData.h"
#include "GA/CSGA_GiveDamage.h"

ACSCharacterPatrol::ACSCharacterPatrol()
{
	AIControllerClass = ACSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;

	CurrentPatrolIdx = 0;
	PatrolPosesLength = 0;
	OverlappedPlayerCount = 0;

	// Trigger
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	Trigger->SetupAttachment(GetCapsuleComponent());
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACSCharacterPatrol::OnTriggerBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACSCharacterPatrol::OnTriggerEndOverlap);

	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UCSAttributeSet>(TEXT("AttributeSet"));
}

FVector ACSCharacterPatrol::GetPatrolPos()
{
	if (PatrolPosesLength == 0)
	{
		return GetActorLocation();
	}

	int8 PrevIdx = CurrentPatrolIdx;
	CurrentPatrolIdx = (CurrentPatrolIdx + 1) % PatrolPosesLength;
	//UE_LOG(LogCS, Log, TEXT("%d"), CurrentPatrolIdx);

	if ( PatrolPoses.IsValidIndex(PrevIdx) )
	{
		return PatrolPoses[PrevIdx];
	}
	else
	{
		return FVector(0.0f, 0.0f, 0.0f);
	}
}

void ACSCharacterPatrol::BeginPlay()
{
	Super::BeginPlay();

	if ( HasAuthority() )
	{
		PatrolPosesLength = PatrolPoses.Num();

		if (ASC)
		{
			FGameplayAbilitySpec DamageSpec(UCSGA_GiveDamage::StaticClass());
			ASC->GiveAbility(FGameplayAbilitySpec(DamageSpec));
		}
	}
}

void ACSCharacterPatrol::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	SetData();
}

ACSCharacterPlayer* ACSCharacterPatrol::GetCharacterPlayer()
{
	return CharacterPlayer;
}

void ACSCharacterPatrol::SetData()
{
	if (Data == nullptr) return;

	GetCharacterMovement()->RotationRate = Data->RotationRate;
	GetCharacterMovement()->JumpZVelocity = Data->JumpZVelocity;
	GetCharacterMovement()->AirControl = Data->AirControl;
	GetCharacterMovement()->MaxWalkSpeed = Data->MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = Data->MinAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = Data->BrakingDecelerationWalking;

	GetCapsuleComponent()->SetCapsuleSize(Data->CapsuleRadius, Data->CapsuleHeight);

	GetMesh()->SetSkeletalMesh(Data->Mesh);
	GetMesh()->SetAnimInstanceClass(Data->AnimInstance);
	GetMesh()->SetRelativeLocation(Data->MeshLocation);
	GetMesh()->SetRelativeRotation(Data->MeshRotation);

	Trigger->SetBoxExtent(Data->TriggerExtent);
	Trigger->SetRelativeLocationAndRotation(Data->TriggerLocation, Data->TriggerRotation);
}

void ACSCharacterPatrol::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if ( HasAuthority() )
	{
		ACSCharacterPlayer* OverlappedPlayer = Cast<ACSCharacterPlayer>(OtherActor);

		if (OverlappedPlayer)
		{
			CharacterPlayer = OverlappedPlayer;
			Cast<ACSAIController>(GetController())->ActiveMove(false);
			++OverlappedPlayerCount;
		}
	}	
}

void ACSCharacterPatrol::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ( HasAuthority() )
	{
		ACSCharacterPlayer* OverlappedPlayer = Cast<ACSCharacterPlayer>(OtherActor);

		if (OverlappedPlayer)
		{
			--OverlappedPlayerCount;
			if (OverlappedPlayerCount > 0)
			{
				OverlappedPlayerCount = 0;
				Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
			else
			{
				Cast<ACSAIController>(GetController())->ActiveMove(true);
				CharacterPlayer = nullptr;
			}
		}
	}
}

void ACSCharacterPatrol::ActivateGiveDamage()
{
	if ( HasAuthority() && ASC )
	{
		ASC->TryActivateAbilityByClass( UCSGA_GiveDamage::StaticClass() );
	}
}


