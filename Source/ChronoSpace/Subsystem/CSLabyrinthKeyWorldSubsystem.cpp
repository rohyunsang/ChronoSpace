// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/CSLabyrinthKeyWorldSubsystem.h"

UCSLabyrinthKeyWorldSubsystem::UCSLabyrinthKeyWorldSubsystem()
{
	LabyrinthKeyCount = 0;
}

void UCSLabyrinthKeyWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LabyrinthKeyCount = 0;
}

void UCSLabyrinthKeyWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
