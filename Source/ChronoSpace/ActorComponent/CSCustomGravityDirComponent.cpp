// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CSCustomGravityDirComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/CSGravityCore.h"
#include "ChronoSpace.h"

UCSCustomGravityDirComponent::UCSCustomGravityDirComponent()
{
	SetIsReplicatedByDefault(true);
	OrgGravityDirection = FVector(0.0f, 0.0f, -1.0f);
}


// Called when the game starts
void UCSCustomGravityDirComponent::BeginPlay()
{
	Super::BeginPlay();

	if ( GetOwner() )
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());

		if ( OwnerCharacter && OwnerCharacter->HasAuthority() )
		{
			//UE_LOG(LogCS, Log, TEXT("[NetMode : %d] BeginPlay"), GetNetMode());
			OwnerCharacter->OnActorBeginOverlap.AddDynamic(this, &UCSCustomGravityDirComponent::OnActorBeginOverlapCallback);
			OwnerCharacter->OnActorEndOverlap.AddDynamic(this, &UCSCustomGravityDirComponent::OnActorEndOverlapCallback);
		}

		if ( OwnerCharacter->HasAuthority() )
		{
			GetWorld()->GetTimerManager().SetTimer(GravityCheckTimerHandle, this, &UCSCustomGravityDirComponent::CheckGravity, 0.1f, true);
		}
	}
}

void UCSCustomGravityDirComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCSCustomGravityDirComponent, CurrentGravityDirection);
}

FVector UCSCustomGravityDirComponent::GetDirection()
{
	if ( CurrentGravityCore )
	{
		FVector CoreLocation = CurrentGravityCore->GetActorLocation();
		FVector CharacterLocation = OwnerCharacter->GetActorLocation();

		return (CoreLocation - CharacterLocation).GetSafeNormal();
	}

	return FVector();
} 

void UCSCustomGravityDirComponent::OnRep_CurrentGravityDirection()
{
	//UE_LOG(LogCS, Log, TEXT("[NetMode : %d] OnRep_CurrentGravityDirection, (%f, %f, %f)"), GetNetMode(), CurrentGravityDirection.X, CurrentGravityDirection.Y, CurrentGravityDirection.Z);
	if (OwnerCharacter)
	{
		FVector CurrentDirection = OwnerCharacter->GetCharacterMovement()->GetGravityDirection();
		if ( !( (CurrentDirection - CurrentGravityDirection) / 1000.0f ).IsNearlyZero() )
		{
			OwnerCharacter->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
			OwnerCharacter->GetCharacterMovement()->SetGravityDirection(CurrentGravityDirection);
		}
	}
}

void UCSCustomGravityDirComponent::OnActorBeginOverlapCallback(AActor* OverlappedActor, AActor* OtherActor)
{
	ACSGravityCore* Core = Cast<ACSGravityCore>(OtherActor);

	if ( Core )
	{
		CurrentGravityCore = Core;
	}
}

void UCSCustomGravityDirComponent::OnActorEndOverlapCallback(AActor* OverlappedActor, AActor* OtherActor)
{
	ACSGravityCore* Core = Cast<ACSGravityCore>(OtherActor);
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Core)
	{
		CurrentGravityDirection = OrgGravityDirection;
		Character->GetCharacterMovement()->SetGravityDirection(OrgGravityDirection);
		CurrentGravityCore = nullptr;
	}
}

void UCSCustomGravityDirComponent::CheckGravity()
{
	if (OwnerCharacter == nullptr) return;

	if (OwnerCharacter->HasAuthority() && CurrentGravityCore)
	{
		CurrentGravityDirection = GetDirection();
		OwnerCharacter->GetCharacterMovement()->SetGravityDirection(CurrentGravityDirection);
	}
}

