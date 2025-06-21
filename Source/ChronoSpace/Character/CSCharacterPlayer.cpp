// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/CSPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/CSGASWidgetComponent.h"
#include "Character/CSF_CharacterFrameData.h"
#include "UI/CSGASUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Actor/CSWhiteHall.h"
#include "Physics/CSCollision.h"
#include "Actor/CSGravityCore.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "ChronoSpace.h"
#include "ActorComponent/CSPlayerInteractionComponent.h"
#include "ActorComponent/CSPushingCharacterComponent.h"
#include "ActorComponent/CSCharacterScaleComponent.h"
#include "ActorComponent/CSGASManagerComponent.h"
#include "ActorComponent/CSTransformRecordComponent.h"
#include "Player/CSPlayerController.h"
#include "DataAsset/CSCharacterPlayerData.h"


ACSCharacterPlayer::ACSCharacterPlayer()
{
	bReplicates = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	/*
	// UI 
	EnergyBar = CreateDefaultSubobject<UCSGASWidgetComponent>(TEXT("Widget"));
	EnergyBar->SetupAttachment(GetMesh());
	EnergyBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	
	//static ConstructorHelpers::FClassFinder<UUserWidget> EnergyBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> EnergyBarWidgetRef(TEXT("/Game/Blueprint/UI/BP_EnergyBar.BP_EnergyBar_C"));
	if (EnergyBarWidgetRef.Class)
	{
		EnergyBar->SetWidgetClass(EnergyBarWidgetRef.Class);
		EnergyBar->SetWidgetSpace(EWidgetSpace::Screen);
		EnergyBar->SetDrawSize(FVector2D(200.0f, 20.f));
		EnergyBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (EnergyBar)
		{
			UE_LOG(LogTemp, Log, TEXT("EnergyBar is valid and configured."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EnergyBar is not valid!"));
		}
	}
	*/

	// ASC
	ASC = nullptr;

	// Trigger
	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->SetCollisionProfileName( CPROFILE_OVERLAPALL );
	Trigger->SetupAttachment(GetCapsuleComponent());

	// 캐릭터에 컴포넌트 추가
	PushingCharacterComponent = CreateDefaultSubobject<UCSPushingCharacterComponent>(TEXT("PushingCharacterComponent"));
	PushingCharacterComponent->SetTrigger(Trigger);

	InteractionComponent = CreateDefaultSubobject<UCSPlayerInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetTrigger(Trigger);

	ScaleComponent = CreateDefaultSubobject<UCSCharacterScaleComponent>(TEXT("ScaleComponent"));
	
	GASManagerComponent = CreateDefaultSubobject<UCSGASManagerComponent>(TEXT("GASManagerComponent"));

	TransformRecordComponent = CreateDefaultSubobject<UCSTransformRecordComponent>(TEXT("TransformRecordComponent"));
}

UAbilitySystemComponent* ACSCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACSCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACSPlayerState* CSPS = GetPlayerState<ACSPlayerState>();
	
	GASManagerComponent->SetASC(CSPS->GetAbilitySystemComponent(), CSPS);
	GASManagerComponent->SetGASAbilities();
}

void ACSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//UE_LOG(LogCS, Log, TEXT("[NetMode %d] SetupPlayerInputComponent"), GetWorld()->GetNetMode());
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::ShoulderLook);

	GASManagerComponent->SetupGASInputComponent(Cast<UEnhancedInputComponent>(PlayerInputComponent));
	InteractionComponent->SetInteractionInputComponent(Cast<UEnhancedInputComponent>(PlayerInputComponent));
}

void ACSCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ACSPlayerState* CSPS = GetPlayerState<ACSPlayerState>();
	GASManagerComponent->SetASC(ASC, CSPS);
	// EnergyBar->ActivateGAS();
}

void ACSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogCS, Log, TEXT("[NetMode: %d] BeginPlay"), GetWorld()->GetNetMode());

	if ( HasAuthority() )
	{
		// EnergyBar->ActivateGAS();
	}
	
	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController()); 
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))			
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	AttachWindUpKeyToSocket();
}

void ACSCharacterPlayer::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	SetData();
}

void ACSCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController()); 
	if (PlayerController) 
	{
		DisableInput(PlayerController); 
	}
}

void ACSCharacterPlayer::SetData()
{
	if (Data == nullptr) return;

	CameraBoom->TargetArmLength = Data->TargetArmLength;
	CameraBoom->SetRelativeLocation(Data->CameraOffset);

	GetCharacterMovement()->RotationRate = Data->RotationRate;
	GetCharacterMovement()->JumpZVelocity = Data->JumpZVelocity;
	GetCharacterMovement()->AirControl = Data->AirControl;
	GetCharacterMovement()->MaxWalkSpeed = Data->MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = Data->MinAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = Data->BrakingDecelerationWalking;

	GetCapsuleComponent()->SetCapsuleSize(Data->CapsuleRadius, Data->CapsuleHeight); 

	GetMesh()->SetSkeletalMesh(Data->Mesh);
	GetMesh()->SetAnimInstanceClass(Data->AnimInstance);
	GetMesh()->SetRelativeLocation(Data->MeshLocation);
	GetMesh()->SetRelativeRotation(Data->MeshRotation); 

	Trigger->SetCapsuleSize(Data->TriggerRadius, Data->TriggerHeight); 
}

void ACSCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	//UE_LOG(LogCS, Log, TEXT("[NetMode %d] ShoulderMove"), GetWorld()->GetNetMode());
	AddMovementInput( FollowCamera->GetForwardVector(), MovementVector.X);
	AddMovementInput( FollowCamera->GetRightVector(), MovementVector.Y);
}

void ACSCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACSCharacterPlayer::ClearWhiteHall()
{
	if ( IsValid(WhiteHall) )
	{
		WhiteHall->Destroy();
	}
	WhiteHall = nullptr;
}


void ACSCharacterPlayer::RequestUIRefresh()
{
	if (ACSPlayerController* PC = Cast<ACSPlayerController>(GetController()))
	{
		PC->RefreshGameUI();
	}
}