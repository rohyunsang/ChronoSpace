// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CSPushingCharacterComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CSCharacterPatrol.h"
#include "Character/CSCharacterPlayer.h"
#include "ChronoSpace.h"

UCSPushingCharacterComponent::UCSPushingCharacterComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = true;
}

void UCSPushingCharacterComponent::SetTrigger(UCapsuleComponent* InTrigger)
{
	Trigger = InTrigger;
}


void UCSPushingCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if ( Trigger )
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &UCSPushingCharacterComponent::OnComponentBeginOverlapCallback);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &UCSPushingCharacterComponent::OnComponentEndOverlapCallback);
	}
}

// Called every frame
void UCSPushingCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (OwnerCharacter->GetCharacterMovement()->Velocity.IsNearlyZero())
	{
		return;
	}

	FVector GravityDir = OwnerCharacter->GetCharacterMovement()->GetGravityDirection().GetSafeNormal();
	FVector ForwardDir = OwnerCharacter->GetActorForwardVector();
	FVector FlattenedForward = ForwardDir - (ForwardDir | GravityDir) * GravityDir;
	float Strength = 1000000.0f;
	FVector Power = FlattenedForward * Strength;

	for (auto Char = CharsInPushing.CreateIterator(); Char; ++Char)
	{
		if (IsValid(Char.Value()))
		{
			Char.Value()->GetCharacterMovement()->AddForce(Power);
		}
	}
}

void UCSPushingCharacterComponent::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Cast<ACSCharacterPlayer>(OtherActor) &&
		nullptr == Cast<ACSCharacterPatrol>(OtherActor))
	{
		ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);

		if (OverlappedCharacter)
		{
			CharsInPushing.Emplace(OverlappedCharacter->GetFName(), OverlappedCharacter);
		}
	}
}

void UCSPushingCharacterComponent::OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (nullptr == Cast<ACSCharacterPlayer>(OtherActor))
	{
		ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);

		if (OverlappedCharacter)
		{
			CharsInPushing.Remove(OverlappedCharacter->GetFName());
		}
	}
}

