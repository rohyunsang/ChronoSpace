// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSMoveObjectSwitch.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ChronoSpace.h"


ACSMoveObjectSwitch::ACSMoveObjectSwitch()
{
	// Tick Ȱ��ȭ
	PrimaryActorTick.bCanEverTick = true;

	// �⺻�� ����
	MoveSpeed = 200.0f;           // 200 ���� / ��
	RotationSpeed = 5.0f;        // 5�� / ��
	MoveTolerance = 5.0f;         // 5 ���� ��� ����
	RotationTolerance = 1.0f;     // 1�� ��� ����
	bIsMoving = false;
	bMovingToTarget = false;

	// ��Ʈ��ũ ���� ����
	bReplicates = true;
	SetReplicateMovement(true);
}

void ACSMoveObjectSwitch::BeginPlay()
{
	Super::BeginPlay();

	// ���������� �ʱ� ��ġ/ȸ�� ����
	if (HasAuthority())
	{
		InitializeObjectPositions();
	}
}

void ACSMoveObjectSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���������� �̵� ���� ����
	if (HasAuthority() && bIsMoving)
	{
		UpdateMovement(DeltaTime);
	}
}

void ACSMoveObjectSwitch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSMoveObjectSwitch, bIsMoving);
	DOREPLIFETIME(ACSMoveObjectSwitch, bMovingToTarget);
	DOREPLIFETIME(ACSMoveObjectSwitch, MoveObjects);
}

void ACSMoveObjectSwitch::Interact()
{
	if (!HasAuthority())
	{
		return;
	}

	UE_LOG(LogCS, Log, TEXT("[NetMode: %d] Interact MoveObjectSwitch - Current State: %s"),
		GetWorld()->GetNetMode(), bIsInteracted ? TEXT("ON") : TEXT("OFF"));

	// �θ� Ŭ������ ��ȣ�ۿ� (����ġ ���� ����)
	Super::Interact();

	// �̵� ����
	StartMovement();
}

void ACSMoveObjectSwitch::SetInteracted(bool bInInteracted)
{
	if (!HasAuthority())
	{
		return;
	}

	bool bStateChanged = (bIsInteracted != bInInteracted);
	bIsInteracted = bInInteracted;
	SetMaterial();

	// ���°� ����� ��쿡�� �̵� ����
	if (bStateChanged)
	{
		StartMovement();
	}
}

void ACSMoveObjectSwitch::InitializeObjectPositions()
{
	for (FMoveObjectData& MoveData : MoveObjects)
	{
		if (IsValid(MoveData.TargetActor))
		{
			MoveData.InitialLocation = MoveData.TargetActor->GetActorLocation();
			MoveData.InitialRotation = MoveData.TargetActor->GetActorRotation();
			MoveData.bIsAtTarget = false;

			UE_LOG(LogCS, Log, TEXT("Initialized Actor %s - Location: %s, Rotation: %s"),
				*MoveData.TargetActor->GetName(),
				*MoveData.InitialLocation.ToString(),
				*MoveData.InitialRotation.ToString());
		}
	}
}

void ACSMoveObjectSwitch::StartMovement()
{
	if (bIsMoving)
	{
		UE_LOG(LogCS, Warning, TEXT("Movement already in progress, ignoring new request"));
		return;
	}

	// �̵��� ��ȿ�� ������Ʈ�� �ִ��� Ȯ��
	bool bHasValidTargets = false;
	for (const FMoveObjectData& MoveData : MoveObjects)
	{
		if (IsValid(MoveData.TargetActor))
		{
			bHasValidTargets = true;
			break;
		}
	}

	if (!bHasValidTargets)
	{
		UE_LOG(LogCS, Warning, TEXT("No valid target actors to move"));
		return;
	}

	// �̵� ���� ���� (����ġ ���¿� ����)
	bMovingToTarget = bIsInteracted;
	bIsMoving = true;

	UE_LOG(LogCS, Log, TEXT("Starting movement - Direction: %s"),
		bMovingToTarget ? TEXT("To Target") : TEXT("To Initial"));

	// Ŭ���̾�Ʈ�� �̵� ���� �˸�
	NetMulticastStartMovement(bMovingToTarget);
}

void ACSMoveObjectSwitch::UpdateMovement(float DeltaTime)
{
	bool bAllReachedDestination = true;
	TArray<FVector> NewLocations;
	TArray<FRotator> NewRotations;

	for (FMoveObjectData& MoveData : MoveObjects)
	{
		if (!IsValid(MoveData.TargetActor))
		{
			NewLocations.Add(FVector::ZeroVector);
			NewRotations.Add(FRotator::ZeroRotator);
			continue;
		}

		// === ��ġ �̵� ó�� ===
		FVector CurrentLocation = MoveData.TargetActor->GetActorLocation();
		FVector TargetPosition = bMovingToTarget ? MoveData.TargetLocation : MoveData.InitialLocation;
		FVector NewLocation = CurrentLocation;

		bool bLocationReached = true;
		if (MoveData.bEnableLocationMovement)
		{
			float DistanceToTarget = FVector::Dist(CurrentLocation, TargetPosition);
			if (DistanceToTarget > MoveTolerance)
			{
				// �ε巴�� ��ġ �̵�
				NewLocation = FMath::VInterpTo(CurrentLocation, TargetPosition, DeltaTime, MoveSpeed / 100.0f);
				bLocationReached = false;
			}
			else
			{
				// ��ǥ�� ���������� ��Ȯ�� ��ġ�� ����
				NewLocation = TargetPosition;
			}
			MoveData.TargetActor->SetActorLocation(NewLocation);
		}

		// === ȸ�� ó�� ===
		FRotator CurrentRotation = MoveData.TargetActor->GetActorRotation();
		FRotator TargetRotationValue = bMovingToTarget ? MoveData.TargetRotation : MoveData.InitialRotation;
		FRotator NewRotation = CurrentRotation;

		bool bRotationReached = true;
		if (MoveData.bEnableRotationMovement)
		{
			// ȸ�� �Ÿ� ��� (���� ����)
			FRotator RotationDifference = TargetRotationValue - CurrentRotation;
			RotationDifference.Normalize(); // -180~180 ������ ����ȭ

			float RotationDistance = FMath::Abs(RotationDifference.Pitch) +
				FMath::Abs(RotationDifference.Yaw) +
				FMath::Abs(RotationDifference.Roll);

			if (RotationDistance > RotationTolerance)
			{
				// �ε巴�� ȸ��
				NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotationValue, DeltaTime, RotationSpeed);
				bRotationReached = false;
			}
			else
			{
				// ��ǥ�� ���������� ��Ȯ�� ȸ������ ����
				NewRotation = TargetRotationValue;
			}
			MoveData.TargetActor->SetActorRotation(NewRotation);
		}

		// ��ġ�� ȸ�� ��� ��ǥ�� �����ߴ��� Ȯ��
		if (!bLocationReached || !bRotationReached)
		{
			bAllReachedDestination = false;
		}
		else
		{
			MoveData.bIsAtTarget = bMovingToTarget;
		}

		NewLocations.Add(NewLocation);
		NewRotations.Add(NewRotation);
	}

	// Ŭ���̾�Ʈ�� ��ġ�� ȸ�� ���� ����
	NetMulticastUpdateMovement(NewLocations, NewRotations);

	// ��� ������Ʈ�� ��ǥ�� ���������� �̵� �Ϸ�
	if (bAllReachedDestination)
	{
		bIsMoving = false;
		UE_LOG(LogCS, Log, TEXT("Movement completed - All objects reached destination"));
	}
}

void ACSMoveObjectSwitch::NetMulticastStartMovement_Implementation(bool bInMoveToTarget)
{
	// Ŭ���̾�Ʈ������ �̵� ���¸� ������Ʈ
	bMovingToTarget = bInMoveToTarget;
	bIsMoving = true;

}

void ACSMoveObjectSwitch::NetMulticastUpdateMovement_Implementation(const TArray<FVector>& NewLocations, const TArray<FRotator>& NewRotations)
{
	// Ŭ���̾�Ʈ���� ��ġ�� ȸ�� ������Ʈ
	if (!HasAuthority())
	{
		for (int32 i = 0; i < MoveObjects.Num() && i < NewLocations.Num() && i < NewRotations.Num(); i++)
		{
			if (IsValid(MoveObjects[i].TargetActor))
			{
				// ��ġ ������Ʈ
				if (MoveObjects[i].bEnableLocationMovement)
				{
					MoveObjects[i].TargetActor->SetActorLocation(NewLocations[i]);
				}

				// ȸ�� ������Ʈ
				if (MoveObjects[i].bEnableRotationMovement)
				{
					MoveObjects[i].TargetActor->SetActorRotation(NewRotations[i]);
				}
			}
		}
	}
}