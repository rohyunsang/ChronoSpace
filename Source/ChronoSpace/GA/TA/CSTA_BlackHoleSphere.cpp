// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_BlackHoleSphere.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CSCharacterPlayer.h"
#include "Actor/CSWhiteHall.h"
#include "Physics/CSCollision.h"
#include "Kismet/GameplayStatics.h"
#include "ChronoSpace.h"

ACSTA_BlackHoleSphere::ACSTA_BlackHoleSphere()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true; 

	// GravitySphereTrigger
	GravitySphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("GravitySphereTrigger"));
	RootComponent = GravitySphereTrigger;
	GravitySphereTrigger->SetSphereRadius(GravityInfluenceRange, true);
	GravitySphereTrigger->SetRelativeLocation(FVector(600.0f, 0.0f, 200.0f));
	GravitySphereTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	GravitySphereTrigger->SetIsReplicated(true);

	GravitySphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_BlackHoleSphere::OnTriggerBeginOverlap);
	GravitySphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_BlackHoleSphere::OnTriggerEndOverlap);

	// EventHorizonSphereTrigger
	EventHorizonSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("EventHorizonSphereTrigger"));
	EventHorizonSphereTrigger->SetupAttachment(GravitySphereTrigger);
	EventHorizonSphereTrigger->SetSphereRadius(EventHorizonRadius, true);
	EventHorizonSphereTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	EventHorizonSphereTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	EventHorizonSphereTrigger->SetIsReplicated(true);
	EventHorizonSphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_BlackHoleSphere::OnEventHorizonBeginOverlap); 

	// Static Mesh
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComp->SetupAttachment(GravitySphereTrigger);
	StaticMeshComp->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/MaterialSphere.MaterialSphere'"));
	if (StaticMeshRef.Object)
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MI_BlackHole.MI_BlackHole'"));
	if (MaterialRef.Object)
	{
		StaticMeshComp->SetMaterial(0, MaterialRef.Object);
	}
}

void ACSTA_BlackHoleSphere::BeginPlay()
{
	Super::BeginPlay();

	if (!bShowDebug) return;

	if (GravitySphereTrigger)
	{
		FVector SphereLocation = GravitySphereTrigger->GetComponentLocation(); 
		float SphereRadius = GravitySphereTrigger->GetScaledSphereRadius();   

		DrawDebugSphere(
			GetWorld(),
			SphereLocation,
			SphereRadius,
			12,          // 세그먼트 수 (구의 매끄러움)
			FColor::Green,
			false,       // 지속 표시
			10,           // 지속 시간
			0,           // 디버그 선 우선순위
			2.0f         // 선 두께
		);
	}
}

void ACSTA_BlackHoleSphere::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector BlackHoleLocation = GravitySphereTrigger->GetComponentLocation();
	
	for (auto Char = CharactersInSphereTrigger.CreateIterator(); Char; ++Char)
	{
		if (IsValid(Char.Value()))
		{
			FVector Power(230000.0f, 230000.0f, 230000.0f);
			FVector TargetLocation = Char.Value()->GetActorLocation();
			FVector Distance = BlackHoleLocation - TargetLocation;
			FVector Direction = Distance.GetSafeNormal();
			
			Char.Value()->GetCharacterMovement()->AddForce(Power * Direction);
		}
	}
}

void ACSTA_BlackHoleSphere::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_BlackHoleSphere::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		OnComplete.Broadcast();
	}
}

void ACSTA_BlackHoleSphere::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

	if (DetectedCharacter)
	{
		CharactersInSphereTrigger.Emplace(DetectedCharacter->GetFName(), DetectedCharacter);
	}
}

void ACSTA_BlackHoleSphere::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

	if (DetectedCharacter)
	{
		CharactersInSphereTrigger.Remove(DetectedCharacter->GetFName());
	}
}

void ACSTA_BlackHoleSphere::OnEventHorizonBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ACharacter* OverlapedCharacter = Cast<ACharacter>(OtherActor);
	ACSCharacterPlayer* OverlapedCharacterPlayer = Cast<ACSCharacterPlayer>(OtherActor);


	ACSCharacterPlayer* Player = Cast<ACSCharacterPlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );

	if ( ACSWhiteHall* WhiteHall = Player->GetWhiteHall() )
	{
		FVector NewLocation = WhiteHall->GetActorLocation();
		
		OtherActor->SetActorLocation(NewLocation);
		if ( (GravitySphereTrigger->GetComponentLocation() - NewLocation).Size() > GravityInfluenceRange )
		{
			CharactersInSphereTrigger.Remove(OverlapedCharacter->GetFName());
		}
	}
	else if (OverlapedCharacter)
	{
		if (OverlapedCharacterPlayer == nullptr)
		{
			OtherActor->Destroy();
		}
	} 
}

