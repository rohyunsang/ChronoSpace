// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/CSTA_ReverseGravityBox.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ChronoSpace.h"

ACSTA_ReverseGravityBox::ACSTA_ReverseGravityBox()
{
    bReplicates = true;
    bShowDebug = false;

    // Trigger
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerBeginOverlap);
    BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACSTA_ReverseGravityBox::OnTriggerEndOverlap);

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialRef(TEXT("/Script/Engine.Material'/Game/Material/MAT_AntyGravity.MAT_AntyGravity'"));
    
    SetSteticMeshMaterial(MaterialRef.Object, MeshScale.X);
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

void ACSTA_ReverseGravityBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);

    if (DetectedCharacter)
    {
        UCharacterMovementComponent* MovementComp = DetectedCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            UE_LOG(LogCS, Log, TEXT("OnTriggerBeginOverlap called: %f"), MovementComp->GravityScale);
            if (MovementComp->GravityScale < 0)
            {
                return;
            }
            MovementComp->AddImpulse(FVector(0.0f, 0.0f, 0.1f));
            MovementComp->GravityScale *= -1.0f;
            
            ActorsInBoxTrigger.Emplace(OtherActor->GetFName(), OtherActor);
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
            UE_LOG(LogCS, Log, TEXT("OnTriggerEndOverlap called: %f"), MovementComp->GravityScale);
            ActorsInBoxTrigger.Remove(OtherActor->GetFName());
        }
    }
}
