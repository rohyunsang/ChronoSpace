// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CSSwitchBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Physics/CSCollision.h"
#include "ChronoSpace.h"

// Sets default values
ACSSwitchBase::ACSSwitchBase()
{
	bReplicates = true;
	bIsInteracted = false;

	// Static Mesh Comp
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetIsReplicated(true);
	RootComponent = StaticMeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/Switch/functional_elements.functional_elements'"));
	if ( StaticMeshRef.Succeeded() )
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	// Material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialGlowNonInteractedRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MI_Switch_GlowNonInteracted.MI_Switch_GlowNonInteracted'"));
	if (MaterialGlowNonInteractedRef.Succeeded())
	{
		MaterialGlowNonInteracted = MaterialGlowNonInteractedRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialGlowInteractedRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MI_Switch_GlowInteracted.MI_Switch_GlowInteracted'"));
	if (MaterialGlowInteractedRef.Succeeded())
	{
		MaterialGlowInteracted = MaterialGlowInteractedRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialSolidNonInteractedRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MI_Switch_SolidNonInteracted.MI_Switch_SolidNonInteracted'"));
	if (MaterialSolidNonInteractedRef.Succeeded())
	{
		MaterialSolidNonInteracted = MaterialSolidNonInteractedRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialSolidInteractedRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MI_Switch_SolidInteracted.MI_Switch_SolidInteracted'"));
	if (MaterialSolidInteractedRef.Succeeded())
	{
		MaterialSolidInteracted = MaterialSolidInteractedRef.Object;
	}

	if (bIsInteracted)
	{
		StaticMeshComp->SetMaterial(1, MaterialSolidInteracted);
		StaticMeshComp->SetMaterial(3, MaterialGlowInteracted);
	}
	else
	{
		StaticMeshComp->SetMaterial(1, MaterialSolidNonInteracted);
		StaticMeshComp->SetMaterial(3, MaterialGlowNonInteracted);
	}

	// Trigger
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetSphereRadius(80.0f, true);
	Trigger->SetupAttachment(StaticMeshComp);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	Trigger->SetCollisionProfileName(CPROFILE_OVERLAPALL);

	// Widget
	InteractionPromptComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptComponent"));
	InteractionPromptComponent->SetupAttachment(Trigger);
	InteractionPromptComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

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

void ACSSwitchBase::BeginInteraction()
{
	InteractionPromptComponent->SetVisibility(true);
}

void ACSSwitchBase::EndInteraction()
{
	InteractionPromptComponent->SetVisibility(false);
}

void ACSSwitchBase::Interact()
{
	UE_LOG(LogCS, Log, TEXT("[Netmode : %d] Interact"), GetWorld()->GetNetMode());
	bIsInteracted = !bIsInteracted;

	SetMaterial();
}

void ACSSwitchBase::SetMaterial()
{
	NetMulticastSetMaterial(bIsInteracted);
}

void ACSSwitchBase::NetMulticastSetMaterial_Implementation(bool bInIsInteracted)
{
	bIsInteracted = bInIsInteracted;
	//UE_LOG(LogCS, Log, TEXT("[NetMode : %d] NetMulticastSetMaterial_Implementation, %d"), GetWorld()->GetNetMode(), bIsInteracted);
	if (bIsInteracted)
	{
		StaticMeshComp->SetMaterial(1, MaterialSolidInteracted);
		StaticMeshComp->SetMaterial(3, MaterialGlowInteracted);
	}
	else
	{
		StaticMeshComp->SetMaterial(1, MaterialSolidNonInteracted);
		StaticMeshComp->SetMaterial(3, MaterialGlowNonInteracted);
	}
}



