// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CSLabyrinthKeyWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CHRONOSPACE_API UCSLabyrinthKeyWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UCSLabyrinthKeyWorldSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FORCEINLINE void SetLabyrinthKeyCount(int8 InCount) { LabyrinthKeyCount = InCount; }
	FORCEINLINE const int8 GetLabyrinthKeyCount() const { return LabyrinthKeyCount; }
private:
	int8 LabyrinthKeyCount;
};
