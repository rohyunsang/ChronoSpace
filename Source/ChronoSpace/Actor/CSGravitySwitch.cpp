// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSGravitySwitch.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ActorComponent/CSCustomGravityDirComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "ChronoSpace.h"

ACSGravitySwitch::ACSGravitySwitch()
{
	bReplicates = true;
}

void ACSGravitySwitch::Interact()
{
	Super::Interact();

	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		ACharacter* Char = *It;
		
		FVector CharGravity = Char->GetCharacterMovement()->GetGravityDirection();

		if ( bIsInteracted )
		{
			CharGravity.Z = FMath::Abs(CharGravity.Z);
			UCSCustomGravityDirComponent::OrgGravityDirection = FVector(0.0f, 0.0f, 1.0f);
		}
		else
		{
			CharGravity.Z = -1.0f * FMath::Abs(CharGravity.Z);
			UCSCustomGravityDirComponent::OrgGravityDirection = FVector(0.0f, 0.0f, -1.0f);
		}

		Char->GetCharacterMovement()->SetGravityDirection(CharGravity);
	}

	for (TActorIterator<ACSGravitySwitch> It(GetWorld()); It; ++It)
	{
		ACSGravitySwitch* Switch = *It;
		Switch->SetInteracted(bIsInteracted);
	}
}

void ACSGravitySwitch::SetInteracted(bool bInInteracted)
{
	bIsInteracted = bInInteracted;
	SetMaterial();
}
