// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSLabyrinthKey.h"
#include "Character/CSCharacterPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Physics/CSCollision.h"
#include "Subsystem/CSLabyrinthKeyWorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "ChronoSpace.h"

// Sets default values
ACSLabyrinthKey::ACSLabyrinthKey()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	// Static Mesh
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetCollisionProfileName(CPROFILE_CSCAPSULE);
	StaticMeshComp->SetIsReplicated(true);

	// SphereTrigger
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTrigger"));
	SphereTrigger->SetSphereRadius(TriggerRange, true);
	SphereTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereTrigger->SetupAttachment(RootComponent);
	SphereTrigger->SetCollisionProfileName(CPROFILE_OVERLAPALL);
	SphereTrigger->SetIsReplicated(true);

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/MapClockworkLabyrinth/Key/SM_SM_LabyrinthKey.SM_SM_LabyrinthKey'"));
	if (StaticMeshRef.Object)
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}*/

	// Widget
	InteractionPromptComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptComponent"));
	InteractionPromptComponent->SetupAttachment(SphereTrigger);
	InteractionPromptComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	/*static ConstructorHelpers::FClassFinder<UUserWidget> InteractionPromptWidgetRef(TEXT("/Game/Blueprint/UI/BP_InteractionPrompt.BP_InteractionPrompt_C"));
	if (InteractionPromptWidgetRef.Class)
	{
		InteractionPromptComponent->SetWidgetClass(InteractionPromptWidgetRef.Class);
		InteractionPromptComponent->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionPromptComponent->SetDrawSize(FVector2D(500.0f, 30.f));
		InteractionPromptComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}*/

	InteractionPromptComponent->SetVisibility(false);

	SetActive(false);
}

void ACSLabyrinthKey::BeginInteraction()
{
	InteractionPromptComponent->SetVisibility(true);
}

void ACSLabyrinthKey::EndInteraction()
{
	InteractionPromptComponent->SetVisibility(false);
}

void ACSLabyrinthKey::Interact()
{
	UCSLabyrinthKeyWorldSubsystem* LabyrinthKeySubsystem = GetWorld()->GetSubsystem<UCSLabyrinthKeyWorldSubsystem>();

	if (LabyrinthKeySubsystem)
	{
		LabyrinthKeySubsystem->SetLabyrinthKeyCount((LabyrinthKeySubsystem->GetLabyrinthKeyCount()) + 1);

		UE_LOG(LogCS, Log, TEXT("ACSLabyrinthKey - Interact : %d"), LabyrinthKeySubsystem->GetLabyrinthKeyCount());
	}

	Destroy();
}

void ACSLabyrinthKey::OnRep_bIsActive()
{
	if ( GetWorld() )
	{
		UE_LOG(LogCS, Log, TEXT("[NetMode : %d] OnRep_bIsActive, %d"), GetWorld()->GetNetMode(), bIsActive);
	}
	
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
	//SetActorTickEnabled(bIsActive);
}

void ACSLabyrinthKey::SetActive(bool bActive)
{
	if ( HasAuthority() )
	{
		bIsActive = bActive;
		OnRep_bIsActive();
	}
}

void ACSLabyrinthKey::BeginPlay()
{
	Super::BeginPlay();

	if (!StaticMesh.IsValid())
	{
		StaticMesh.LoadSynchronous();
	}

	if (StaticMesh.IsValid())
	{
		StaticMeshComp->SetStaticMesh(StaticMesh.Get());
	}
	else
	{
		UE_LOG(LogCS, Error, TEXT("StaticMesh failed to load in ACSLabyrinthKey"));
	}

	if ( !InteractionPromptWidgetClass.IsValid() )
	{
		InteractionPromptWidgetClass.LoadSynchronous();
	}

	if ( InteractionPromptWidgetClass.IsValid() )
	{
		InteractionPromptComponent->SetWidgetClass(InteractionPromptWidgetClass.Get());
		InteractionPromptComponent->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionPromptComponent->SetDrawSize(FVector2D(500.0f, 30.f));
		InteractionPromptComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogCS, Error, TEXT("InteractionPromptWidgetClass failed to load in ACSLabyrinthKey")); 
	}

}

void ACSLabyrinthKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ACSLabyrinthKey, bIsActive);
}