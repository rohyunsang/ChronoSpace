// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSLabyrinthKey.h"
#include "Character/CSCharacterPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Physics/CSCollision.h"
#include "Subsystem/CSLabyrinthKeyWorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "ChronoSpace.h"

// Sets default values
ACSLabyrinthKey::ACSLabyrinthKey()
{
	bReplicates = true;

	// SphereTrigger
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("GravitySphereTrigger"));
	SphereTrigger->SetSphereRadius(TriggerRange, true);
	SphereTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RootComponent = SphereTrigger;
	SphereTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	SphereTrigger->SetIsReplicated(true);

	// Static Mesh
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComp->SetupAttachment(SphereTrigger);
	StaticMeshComp->SetCollisionProfileName(CPROFILE_CSCAPSULE);
	StaticMeshComp->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/BlockSphere.BlockSphere'"));
	if (StaticMeshRef.Object)
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	float MeshRadius = 50.0f;
	float MeshScale = (TriggerRange / MeshRadius) * 0.75f;
	StaticMeshComp->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));

	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSLabyrinthKey::OnTriggerBeginOverlapCallback);
	SphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSLabyrinthKey::OnTriggerEndOverlapCallback);

	// Widget
	InteractionPromptComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptComponent"));
	InteractionPromptComponent->SetupAttachment(SphereTrigger);
	InteractionPromptComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionPromptWidgetRef(TEXT("/Game/Blueprint/UI/BP_InteractionPrompt.BP_InteractionPrompt_C"));
	if (InteractionPromptWidgetRef.Class)
	{
		InteractionPromptComponent->SetWidgetClass(InteractionPromptWidgetRef.Class);
		InteractionPromptComponent->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionPromptComponent->SetDrawSize(FVector2D(500.0f, 30.f));
		InteractionPromptComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	InteractionPromptComponent->SetVisibility(false);
}

void ACSLabyrinthKey::OnTriggerBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ACSCharacterPlayer* Player = Cast<ACSCharacterPlayer>(OtherActor);

	if ( Player )
	{
		Player->OnInteract.Clear();
		InteractionPromptComponent->SetVisibility(true);
		Player->OnInteract.AddDynamic(this, &ACSLabyrinthKey::Interact);
	}
}

void ACSLabyrinthKey::OnTriggerEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACSCharacterPlayer* Player = Cast<ACSCharacterPlayer>(OtherActor);

	if (Player)
	{
		InteractionPromptComponent->SetVisibility(false);
		Player->OnInteract.Clear();
	}
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


