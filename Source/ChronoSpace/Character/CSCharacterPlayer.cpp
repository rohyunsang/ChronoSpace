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
#include "Character/CSCharacterPatrol.h"
#include "ActorComponent/CSPlayerInteractionComponent.h"


ACSCharacterPlayer::ACSCharacterPlayer()
{
	bReplicates = true;

	bIsInteracted = false;

	PrimaryActorTick.bCanEverTick = true;


	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		MappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReverseGravityRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ReverseGravity.IA_ReverseGravity'"));
	if (nullptr != InputActionReverseGravityRef.Object)
	{
		ReverseGravityAction = InputActionReverseGravityRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionBlackHoleRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_BlackHole.IA_BlackHole'"));
	if (nullptr != InputActionBlackHoleRef.Object)
	{
		BlackHoleAction = InputActionBlackHoleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWhiteHoleRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_WhiteHole.IA_WhiteHole'"));
	if (nullptr != InputActionWhiteHoleRef.Object)
	{
		WhiteHoleAction = InputActionWhiteHoleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWeakenGravity10PActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_WeakenGravity10P.IA_WeakenGravity10P'"));
	if (nullptr != InputActionBlackHoleRef.Object)
	{
		WeakenGravity10PAction = InputActionWeakenGravity10PActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWeakenGravity50PActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_WeakenGravity50P.IA_WeakenGravity50P'"));
	if (nullptr != InputActionWhiteHoleRef.Object)
	{
		WeakenGravity50PAction = InputActionWeakenGravity50PActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChronoControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ChronoControl.IA_ChronoControl'"));
	if (nullptr != InputActionChronoControlRef.Object)
	{
		ChronoControlAction = InputActionChronoControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAbilityPreviewRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_AbilityPreview.IA_AbilityPreview'"));
	if (nullptr != InputActionAbilityPreviewRef.Object)
	{
		AbilityPreviewAction = InputActionAbilityPreviewRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTimeRewindRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_TimeRewind.IA_TimeRewind'"));
	if (nullptr != InputActionTimeRewindRef.Object)
	{
		TimeRewindAction = InputActionTimeRewindRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Interact.IA_Interact'"));
	if (nullptr != InputActionInteractRef.Object)
	{
		InteractAction = InputActionInteractRef.Object;
	}


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

	// ASC
	ASC = nullptr;

	// Trigger
	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->InitCapsuleSize(50.f, 100.0f);
	Trigger->SetCollisionProfileName( CPROFILE_OVERLAPALL );
	Trigger->SetupAttachment(GetCapsuleComponent());
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACSCharacterPlayer::OnTriggerBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACSCharacterPlayer::OnTriggerEndOverlap);

	InteractionComponent = CreateDefaultSubobject<UCSPlayerInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetTrigger(Trigger);
}


void ACSCharacterPlayer::RecordTransform()
{
	FCSF_CharacterFrameData FrameData(GetActorLocation(), GetActorRotation(), GetWorld()->GetTimeSeconds());

	if (TransformHistory.Num() >= MaxHistorySize + 1) // 1 is offset
	{
		TransformHistory.RemoveAt(0); 
	}
	TransformHistory.Add(FrameData);
}

void ACSCharacterPlayer::ServerInteract_Implementation()
{
	InteractionComponent->ExecInteraction();
}

void ACSCharacterPlayer::Interact()
{
	if (bIsInteracted) return;

	if ( HasAuthority() )
	{
		InteractionComponent->ExecInteraction();
	}
	else
	{
		ServerInteract();
	}
	bIsInteracted = true;
}

void ACSCharacterPlayer::EndInteract()
{
	bIsInteracted = false;
}


UAbilitySystemComponent* ACSCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACSCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//UE_LOG(LogCS, Log, TEXT("[NetMode: %d] PossessedBy"), GetWorld()->GetNetMode());
	SetASC();
	SetGASAbilities();
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

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::Interact);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::EndInteract);

	SetupGASInputComponent();
}

void ACSCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// UE_LOG(LogCS, Log, TEXT("*** [NetMode : %d] OnRep_PlayerState, %s, %s"), GetWorld()->GetNetMode(), *GetName(), *GetPlayerState()->GetName());

	SetASC();
	EnergyBar->ActivateGAS();
	// UE_LOG(LogCS, Log, TEXT("*** [NetMode : %d] OnRep_PlayerState, %s, %s"), GetWorld()->GetNetMode(), *GetName(), *GetPlayerState()->GetName());
}

void ACSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogCS, Log, TEXT("[NetMode: %d] BeginPlay"), GetWorld()->GetNetMode());

	if ( HasAuthority() )
	{
		EnergyBar->ActivateGAS();
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

void ACSCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeSinceLastRecord += DeltaSeconds;
	if (TimeSinceLastRecord >= RecordInterval)
	{
		RecordTransform();
		TimeSinceLastRecord = 0.0f;
	}

	if ( GetCharacterMovement()->Velocity.IsNearlyZero() )
	{
		return;
	}

	FVector ForwardDirection = GetMesh()->GetComponentRotation().Vector();
	FVector Power = FVector(10000.0f, 10000.0f, 0.0f);
	Power.X *= -ForwardDirection.Y;
	Power.Y *= ForwardDirection.X;

	for (auto Char = CharsInPushing.CreateIterator(); Char; ++Char)
	{
		if ( IsValid(Char.Value()) )
		{
			Char.Value()->GetCharacterMovement()->AddImpulse(Power); 
		}
	} 
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

void ACSCharacterPlayer::SetASC()
{
	if (ASC) return;

	ACSPlayerState* CSPS = GetPlayerState<ACSPlayerState>();
	if (CSPS)
	{
		ASC = CSPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(CSPS, this);
		ASC->ReplicationMode = EGameplayEffectReplicationMode::Mixed;
		//UE_LOG(LogCS, Log, TEXT("*** [NetMode : %d] SetASC, %s, %s"), GetWorld()->GetNetMode(), *GetName(), *GetPlayerState()->GetName());
	}
	else
	{
		UE_LOG(LogCS, Log, TEXT("[NetMode %d] SetASC - ASC Not Found"), GetWorld()->GetNetMode());
	}
}

void ACSCharacterPlayer::SetGASAbilities()
{
	if ( ASC )
	{
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}
	}
}

void ACSCharacterPlayer::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Cast<ACSCharacterPlayer>(OtherActor) && 
		nullptr == Cast<ACSCharacterPatrol>(OtherActor))
	{
		ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);

		if ( OverlappedCharacter )
		{
			CharsInPushing.Emplace(OverlappedCharacter->GetFName(), OverlappedCharacter);
		}
	}	
}

void ACSCharacterPlayer::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void ACSCharacterPlayer::SetupGASInputComponent()
{
	//UE_LOG(LogCS, Log, TEXT("[NetMode: %d] SetupGASInputComponent Start"), GetWorld()->GetNetMode());
	if (IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(ReverseGravityAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::ReverseGravity);
		EnhancedInputComponent->BindAction(ReverseGravityAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::ReverseGravity);

		EnhancedInputComponent->BindAction(BlackHoleAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::BlackHole);
		EnhancedInputComponent->BindAction(BlackHoleAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::BlackHole);
		EnhancedInputComponent->BindAction(WhiteHoleAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::WhiteHole);
		EnhancedInputComponent->BindAction(WhiteHoleAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::WhiteHole);
		EnhancedInputComponent->BindAction(WeakenGravity10PAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::WeakenGravity10P);
		EnhancedInputComponent->BindAction(WeakenGravity10PAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::WeakenGravity10P);
		EnhancedInputComponent->BindAction(WeakenGravity50PAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::WeakenGravity50P);
		EnhancedInputComponent->BindAction(WeakenGravity50PAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::WeakenGravity50P);
		//UE_LOG(LogCS, Log, TEXT("SetupGASInputComponent Succeed"));

		EnhancedInputComponent->BindAction(ChronoControlAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::ChronoControl);
		EnhancedInputComponent->BindAction(ChronoControlAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::ChronoControl);

		EnhancedInputComponent->BindAction(AbilityPreviewAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::AbilityPreviewBox);
		EnhancedInputComponent->BindAction(AbilityPreviewAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::AbilityPreviewBox);

		EnhancedInputComponent->BindAction(TimeRewindAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::TimeRewind);
		EnhancedInputComponent->BindAction(TimeRewindAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::TimeRewind);

		if (ASC)
		{
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.Movement")));
		}

		UE_LOG(LogTemp, Log, TEXT("SetupGASInputComponent Succeed"));
	}
	else if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid ASC"));
	}
	else
	{
		UE_LOG(LogCS, Log, TEXT("Invalid InputComponent"));
	}
}

void ACSCharacterPlayer::GASInputPressed(int32 InputId)
{
	
	if ( HasAuthority() )
	{
		//UE_LOG(LogCS, Log, TEXT("[NetMode : %d], GASInputPressed"), GetWorld()->GetNetMode());
		HandleGASInputPressed(InputId);
	}
	else
	{
		//UE_LOG(LogCS, Log, TEXT("[NetMode : %d], GASInputPressed"), GetWorld()->GetNetMode());
		ServerGASInputPressed(InputId);
	}
}

void ACSCharacterPlayer::ServerGASInputPressed_Implementation(int32 InputId)
{
	//UE_LOG(LogCS, Log, TEXT("[NetMode : %d], ServerGASInputPressed_Implementation"), GetWorld()->GetNetMode());
	if ( HasAuthority() )
	{
		HandleGASInputPressed(InputId);
	}
}

void ACSCharacterPlayer::HandleGASInputPressed(int32 InputId)
{
	if ( !ASC )
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		if (Spec->InputPressed) return;
		//UE_LOG(LogCS, Log, TEXT("[NetMode : %d], HandleGASInputPressed"), GetWorld()->GetNetMode());
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void ACSCharacterPlayer::GASInputReleased(int32 InputId)
{
	if (HasAuthority())
	{
		HandleGASInputReleased(InputId);
	}
	else
	{
		ServerGASInputReleased(InputId);
	}
}

void ACSCharacterPlayer::HandleGASInputReleased(int32 InputId)
{
	if ( !ASC )
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void ACSCharacterPlayer::ServerGASInputReleased_Implementation(int32 InputId)
{
	HandleGASInputReleased(InputId);
}

void ACSCharacterPlayer::ClearWhiteHall()
{
	if ( IsValid(WhiteHall) )
	{
		WhiteHall->Destroy();
	}
	WhiteHall = nullptr;
}


