// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSGravitySwitch.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "ChronoSpace.h"

ACSGravitySwitch::ACSGravitySwitch()
{
}

void ACSGravitySwitch::Interact()
{
	Super::Interact();

	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		ACharacter* Char = *It;
		
		FVector CharGravity = Char->GetCharacterMovement()->GetGravityDirection();
		Char->GetCharacterMovement()->SetGravityDirection(-1.0f * CharGravity);
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
