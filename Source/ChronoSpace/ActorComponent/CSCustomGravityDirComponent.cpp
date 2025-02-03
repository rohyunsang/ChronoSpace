// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CSCustomGravityDirComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/CSGravityCore.h"
#include "ChronoSpace.h"

UCSCustomGravityDirComponent::UCSCustomGravityDirComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
			UE_LOG(LogCS, Log, TEXT("[NetMode : %d] BeginPlay"), GetNetMode());
			OwnerCharacter->OnActorBeginOverlap.AddDynamic(this, &UCSCustomGravityDirComponent::OnActorBeginOverlapCallback);
			OwnerCharacter->OnActorEndOverlap.AddDynamic(this, &UCSCustomGravityDirComponent::OnActorEndOverlapCallback);
		}
	}

	
}

void UCSCustomGravityDirComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UCSCustomGravityDirComponent, CurrentGravityDirection);
	DOREPLIFETIME(UCSCustomGravityDirComponent, Pitch);
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

// Called every frame
void UCSCustomGravityDirComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter == nullptr) return;

	UE_LOG(LogCS, Log, TEXT("########################################################"));
	UE_LOG(LogCS, Log, TEXT("[NetMode : %d] TickComponent, (%f, %f, %f)"), GetNetMode(), OwnerCharacter->GetGravityDirection().X, OwnerCharacter->GetGravityDirection().Y, OwnerCharacter->GetGravityDirection().Z);
	UE_LOG(LogCS, Log, TEXT("[NetMode : %d] TickComponent, (%f, %f)"), GetNetMode(), OwnerCharacter->GetActorRotation().Yaw, OwnerCharacter->GetActorRotation().Pitch);
	//UE_LOG(LogCS, Log, TEXT("[NetMode : %d] TickComponent Pitch, (%f)"), GetNetMode(), Pitch);
	UE_LOG(LogCS, Log, TEXT("########################################################"));

	if ( /*OwnerCharacter->HasAuthority() &&*/ CurrentGravityCore )
	{
		CurrentGravityDirection = GetDirection();
		OwnerCharacter->GetCharacterMovement()->SetGravityDirection(CurrentGravityDirection);
		Pitch = OwnerCharacter->GetActorRotation().Pitch;
	}
	

	/*if ( !OwnerCharacter->HasAuthority() )
	{
		OwnerCharacter->GetCharacterMovement()->SetGravityDirection(CurrentGravityDirection);
	}*/
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

