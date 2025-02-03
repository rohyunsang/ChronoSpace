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

ACSCharacterPlayer::ACSCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 활성화 

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

}

void ACSCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastRecord += DeltaTime;
	if (TimeSinceLastRecord >= RecordInterval)
	{
		RecordTransform();
		TimeSinceLastRecord = 0.0f;
	}
}

void ACSCharacterPlayer::RecordTransform()
{
	FCSF_CharacterFrameData FrameData(GetActorLocation(), GetActorRotation(), GetWorld()->GetTimeSeconds());

	if (TransformHistory.Num() >= MaxHistorySize + 1) // 1 is offset
	{
		TransformHistory.RemoveAt(0); // 오래된 데이터 제거
	}
	TransformHistory.Add(FrameData);
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

void ACSCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController()); 
	if (PlayerController) 
	{
		DisableInput(PlayerController); 
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
		UE_LOG(LogTemp, Log, TEXT("Invalid InputComponent"));
	}
}

void ACSCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		if (Spec->InputPressed) return;

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

