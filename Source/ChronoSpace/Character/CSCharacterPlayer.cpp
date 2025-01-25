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
#include "UI/CSGASUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Actor/CSWhiteHall.h"
#include "Physics/CSCollision.h"
#include "ChronoSpace.h"

ACSCharacterPlayer::ACSCharacterPlayer()
{
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
			UE_LOG(LogTemp, Warning, TEXT("EnergyBar is valid and configured."));
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
	Trigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	Trigger->SetupAttachment(GetCapsuleComponent());
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACSCharacterPlayer::OnTriggerBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACSCharacterPlayer::OnTriggerEndOverlap);
}

UAbilitySystemComponent* ACSCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACSCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACSPlayerState* CSPS = GetPlayerState<ACSPlayerState>();
	if (CSPS)
	{
		ASC = CSPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(CSPS, this);

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

		/*APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));*/
	}
}

void ACSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::ShoulderLook);
}

void ACSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled()) return;

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController()); 
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) 
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	SetupGASInputComponent();
}

void ACSCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

void ACSCharacterPlayer::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr != Cast<ACSCharacterPlayer>(OtherActor)) return;

	ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);

	if (OverlappedCharacter)
	{
		CharsInPushing.Emplace(OverlappedCharacter->GetFName(), OverlappedCharacter);

		UE_LOG(LogCS, Log, TEXT("OnTriggerBeginOverlap : %s"), *OtherActor->GetName());
	}
}

void ACSCharacterPlayer::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (nullptr != Cast<ACSCharacterPlayer>(OtherActor)) return;

	ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);

	if (OverlappedCharacter)
	{
		CharsInPushing.Remove(OverlappedCharacter->GetFName());
	}
}

void ACSCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ACSCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACSCharacterPlayer::SetupGASInputComponent()
{
	UE_LOG(LogTemp, Log, TEXT("SetupGASInputComponent Start"));
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(ReverseGravityAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::ReverseGravity);
		EnhancedInputComponent->BindAction(ReverseGravityAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::ReverseGravity);
		EnhancedInputComponent->BindAction(BlackHoleAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::BlackHole);
		EnhancedInputComponent->BindAction(BlackHoleAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::BlackHole);
		EnhancedInputComponent->BindAction(WhiteHoleAction, ETriggerEvent::Triggered, this, &ACSCharacterPlayer::GASInputPressed, (int32)EAbilityIndex::WhiteHole);
		EnhancedInputComponent->BindAction(WhiteHoleAction, ETriggerEvent::Completed, this, &ACSCharacterPlayer::GASInputReleased, (int32)EAbilityIndex::WhiteHole);

		UE_LOG(LogTemp, Log, TEXT("SetupGASInputComponent Succeed"));
	}
	else if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid ASC"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid InputComponent"));
	}
}

void ACSCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		if (Spec->InputPressed) return;
		UE_LOG(LogCS, Log, TEXT("GASInputPressed"));
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

void ACSCharacterPlayer::ClearWhiteHall()
{
	if ( IsValid(WhiteHall) )
	{
		WhiteHall->Destroy();
	}
	WhiteHall = nullptr;
}



