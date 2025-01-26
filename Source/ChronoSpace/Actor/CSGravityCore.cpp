// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSGravityCore.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChronoSpace.h"

ACSGravityCore::ACSGravityCore()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &ACSGravityCore::OnActorBeginOverlapCallback);
	OnActorEndOverlap.AddDynamic(this, &ACSGravityCore::OnActorEndOverlapCallback);
}

void ACSGravityCore::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ( TargetCharacters.Num() == 0 ) return;

	for (auto Character = TargetCharacters.CreateIterator(); Character; ++Character)
	{
		FVector NewDirection = GetGravityDirection(Character.Value());
		Character.Value()->GetCharacterMovement()->SetGravityDirection(NewDirection);
	}
}

FVector ACSGravityCore::GetGravityDirection(ACharacter* Character)
{
	FVector SelfLocation = GetActorLocation();
	FVector TargetLocation = Character->GetActorLocation();

	return (SelfLocation - TargetLocation).GetSafeNormal();
}

void ACSGravityCore::OnActorBeginOverlapCallback(AActor* OverlappedActor, AActor* OtherActor)
{
	if ( ACharacter* Character = Cast<ACharacter>(OtherActor) )
	{
		TargetCharacters.Emplace(Character->GetFName(), Character);
	}
}

void ACSGravityCore::OnActorEndOverlapCallback(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		Character->GetCharacterMovement()->SetGravityDirection(FVector(0.0f, 0.0f, -1.0f));
		TargetCharacters.Remove(Character->GetFName());
	}
}

