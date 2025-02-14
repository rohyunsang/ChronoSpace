// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSLabyrinthKeyAltar.h"
#include "Character/CSCharacterPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Physics/CSCollision.h"
#include "Subsystem/CSLabyrinthKeyWorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "ChronoSpace.h"
#include "Engine/World.h"

// Sets default values
ACSLabyrinthKeyAltar::ACSLabyrinthKeyAltar()
{
	bReplicates = true;

	// Static Mesh
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetCollisionProfileName(CPROFILE_CSCAPSULE);
	StaticMeshComp->SetIsReplicated(true);

	// SphereTrigger
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("GravitySphereTrigger"));
	SphereTrigger->SetSphereRadius(TriggerRange, true);
	SphereTrigger->SetupAttachment(StaticMeshComp);
	SphereTrigger->SetRelativeLocation(FVector(40.0f, 60.0f, 0.0f));
	SphereTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	SphereTrigger->SetIsReplicated(true);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/SM_SM_Altar.SM_SM_Altar'"));
	if (StaticMeshRef.Object)
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSLabyrinthKeyAltar::OnTriggerBeginOverlapCallback);
	SphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSLabyrinthKeyAltar::OnTriggerEndOverlapCallback);

	// Widget
	InteractionPromptComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptComponent"));
	InteractionPromptComponent->SetupAttachment(SphereTrigger);
	InteractionPromptComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionPromptWidgetRef(TEXT("/Game/Blueprint/UI/BP_InteractionPromptAltar.BP_InteractionPromptAltar_C"));
	if (InteractionPromptWidgetRef.Class)
	{
		InteractionPromptComponent->SetWidgetClass(InteractionPromptWidgetRef.Class);
		InteractionPromptComponent->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionPromptComponent->SetDrawSize(FVector2D(500.0f, 30.f));
		InteractionPromptComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	InteractionPromptComponent->SetVisibility(false);

	RequiredKeyCount = 5;
}

void ACSLabyrinthKeyAltar::OnTriggerBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ACSCharacterPlayer* Player = Cast<ACSCharacterPlayer>(OtherActor);

	if (Player)
	{
		Player->OnInteract.Clear();
		InteractionPromptComponent->SetVisibility(true);
		Player->OnInteract.AddDynamic(this, &ACSLabyrinthKeyAltar::Interact);
	}
}

void ACSLabyrinthKeyAltar::OnTriggerEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACSCharacterPlayer* Player = Cast<ACSCharacterPlayer>(OtherActor);

	if (Player)
	{
		InteractionPromptComponent->SetVisibility(false);
		Player->OnInteract.Clear();
	}
}

void ACSLabyrinthKeyAltar::Interact()
{
	UCSLabyrinthKeyWorldSubsystem* LabyrinthKeySubsystem = GetWorld()->GetSubsystem<UCSLabyrinthKeyWorldSubsystem>();

	if (LabyrinthKeySubsystem)
	{
		int NowKeyCount = LabyrinthKeySubsystem->GetLabyrinthKeyCount();
		
		if ( NowKeyCount >= RequiredKeyCount )
		{
			ChangeLevel();
		}
	}
}

void ACSLabyrinthKeyAltar::ChangeLevel()
{
	if ( HasAuthority() && GetWorld() )
	{
		GetWorld()->ServerTravel(TEXT("BaseMap?listen"));
	}
}
