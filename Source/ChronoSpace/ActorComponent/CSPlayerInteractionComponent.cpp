// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CSPlayerInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interface/CSInteractionInterface.h"
#include "Character/CSCharacterPlayer.h"
#include "ChronoSpace.h"

// Sets default values for this component's properties
UCSPlayerInteractionComponent::UCSPlayerInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCSPlayerInteractionComponent::SetTrigger(UCapsuleComponent* InTrigger)
{
	Trigger = InTrigger;
}

void UCSPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if ( Trigger )
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &UCSPlayerInteractionComponent::OnComponentBeginOverlapCallback);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &UCSPlayerInteractionComponent::OnComponentEndOverlapCallback);
	}
}

void UCSPlayerInteractionComponent::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICSInteractionInterface* Interaction = Cast<ICSInteractionInterface>(OtherActor);

	if ( Interaction )
	{
		Interaction->BeginInteraction();
		CurrentInteractionActor = OtherActor;
		InteractionActors.Emplace(CurrentInteractionActor.GetFName(), CurrentInteractionActor);
	}
}

void UCSPlayerInteractionComponent::OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ICSInteractionInterface* Interaction = Cast<ICSInteractionInterface>(OtherActor);

	if (Interaction)
	{
		Interaction->EndInteraction();
		InteractionActors.Remove(OtherActor->GetFName());

		ClearCachedActors();

		if ( InteractionActors.Num() > 0 )
		{
			auto Actor = InteractionActors.CreateIterator();
			CurrentInteractionActor = Actor.Value();
		}
		else
		{
			CurrentInteractionActor = nullptr;
		}
	}
}

void UCSPlayerInteractionComponent::ClearCachedActors()
{
	for ( auto  Actor = InteractionActors.CreateIterator(); Actor; ++Actor)
	{
		if ( !IsValid( Actor.Value() ) || Actor.Value()->IsPendingKillPending() ) {
			InteractionActors.Remove(Actor.Key());
		}
	}
}

void UCSPlayerInteractionComponent::ExecInteraction()
{
	if ( CurrentInteractionActor )
	{
		ICSInteractionInterface* Interaction = Cast<ICSInteractionInterface>(CurrentInteractionActor);
		Interaction->Interact();
	}
}




