// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSCustomGravityDirComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOSPACE_API UCSCustomGravityDirComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSCustomGravityDirComponent();
	
	static FVector OrgGravityDirection;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector GetDirection();

	UPROPERTY()
	TObjectPtr<class ACSGravityCore> CurrentGravityCore;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGravityDirection)
	FVector CurrentGravityDirection;

	UFUNCTION()
	void OnRep_CurrentGravityDirection();

	UPROPERTY()
	TObjectPtr<class ACharacter> OwnerCharacter;

protected:
	UFUNCTION()
	void OnActorBeginOverlapCallback(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorEndOverlapCallback(AActor* OverlappedActor, AActor* OtherActor);

protected:
	FTimerHandle GravityCheckTimerHandle;
	
	void CheckGravity();
};
