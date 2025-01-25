// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/CSGA_WhiteHall.h"
#include "Actor/CSWhiteHall.h"
#include "Character/CSCharacterPlayer.h"
#include "ChronoSpace.h"

UCSGA_WhiteHall::UCSGA_WhiteHall()
{
}

void UCSGA_WhiteHall::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogCS, Display, TEXT("UCSGA_WhiteHall::ActivateAbility"));
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	ACSCharacterPlayer* Player = Cast<ACSCharacterPlayer>(ActorInfo->AvatarActor);
	if ( Player->GetWhiteHall() )
	{
		Player->ClearWhiteHall(); 
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled); 
		return;
	}

	const FVector SpawnLocation = Player->GetActorLocation();

	ACSWhiteHall* WhiteHall = GetWorld()->SpawnActor<ACSWhiteHall>(ACSWhiteHall::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	Player->SetWhiteHall(WhiteHall);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
