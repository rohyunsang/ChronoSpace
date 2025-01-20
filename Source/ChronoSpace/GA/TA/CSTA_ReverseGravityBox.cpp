// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "Physics/CSCollision.h"
#include "ChronoSpace.h"

ACSTA_ReverseGravityBox::ACSTA_ReverseGravityBox()
{
    // Trigger
	ReverseGravityTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ReverseGravityTrigger"));
    RootComponent = ReverseGravityTrigger;
	ReverseGravityTrigger->SetBoxExtent(FVector(200.0f, 200.0f, 1000.0f));
	ReverseGravityTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ReverseGravityTrigger->SetCollisionProfileName(CPROFILE_CSTRIGGER);
	ReverseGravityTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerBeginOverlap);
    ReverseGravityTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerEndOverlap);

    // Static Mesh
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    StaticMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
    StaticMeshComp->SetupAttachment(ReverseGravityTrigger);
    StaticMeshComp->SetRelativeLocation(FVector(-0.0f, -0.0f, -0.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/StaticMesh/SM_Cube.SM_Cube'"));
    if (nullptr != StaticMeshRef.Object)
    {
        StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
    }

    FVector BoxExtent = ReverseGravityTrigger->GetScaledBoxExtent();
    FVector MeshScale = BoxExtent / 50.0f; // SM_Cube 기본 사이즈가 100x100x100
    StaticMeshComp->SetRelativeScale3D(MeshScale); 

    static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MAT_AntyGravity_Inst.MAT_AntyGravity_Inst'"));
    if (nullptr != MaterialRef.Object)
    {
        StaticMeshComp->SetMaterial(0, MaterialRef.Object);
    }
}

void ACSTA_ReverseGravityBox::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACSTA_ReverseGravityBox::ConfirmTargetingAndContinue()
{
	//UE_LOG(LogCS, Log, TEXT("ConfirmTargetingAndContinue"));
	if (SourceActor)
	{
		OnComplete.Broadcast();
	}
}

void ACSTA_ReverseGravityBox::BeginPlay()
{
    Super::BeginPlay();

    if (ReverseGravityTrigger)
    {
        FVector BoxLocation = ReverseGravityTrigger->GetComponentLocation();
        FVector BoxExtent = ReverseGravityTrigger->GetScaledBoxExtent(); // 스케일이 적용된 박스 크기
        FQuat   BoxRotation = ReverseGravityTrigger->GetComponentRotation().Quaternion();

        DrawDebugBox(
            GetWorld(),
            BoxLocation,
            BoxExtent,
            BoxRotation,
            FColor::Green,
            false,  // 지속 표시
            DurtionTime, // -1은 시간 제한 없음
            0,     // 디버그 선 우선순위
            2.0f   // 선 두께
        );
    }
}

void ACSTA_ReverseGravityBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto Act = ActorsThatIsReveredGravity.CreateIterator(); Act; ++Act)
    {
        if (!IsValid(Act.Value())) continue;
        
        ACharacter* RemainedCharacter = Cast<ACharacter>(Act.Value());
        if (RemainedCharacter)
        {
            UCharacterMovementComponent* MovementComp = RemainedCharacter->GetCharacterMovement();
            if (!MovementComp) continue;
            if (MovementComp->GravityScale > 0) continue;
                
            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;
        }
        
        ActorsThatIsReveredGravity.FindAndRemoveChecked(Act.Value()->GetFName());
    }
    Super::EndPlay(EndPlayReason);
}


void ACSTA_ReverseGravityBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

    if (DetectedCharacter)
    {
        UCharacterMovementComponent* MovementComp = DetectedCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            if (MovementComp->GravityScale < 0)
            {
                return;
            }
            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;
            
            ActorsThatIsReveredGravity.Emplace(OtherActor->GetFName(), OtherActor);
        }
    }
}

void ACSTA_ReverseGravityBox::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    //UE_LOG(LogCS, Log, TEXT("OnTriggerEndOverlap called: %s"), *OtherActor->GetName());
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
    if (DetectedCharacter)
    {
        UCharacterMovementComponent* MovementComp = DetectedCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            if (MovementComp->GravityScale > 0)
            {
                return;
            }

            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;

            ActorsThatIsReveredGravity.FindAndRemoveChecked(OtherActor->GetFName());
        }
    }
}
