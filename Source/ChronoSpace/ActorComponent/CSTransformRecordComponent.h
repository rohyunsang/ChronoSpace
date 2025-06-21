// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/CSF_CharacterFrameData.h"
#include "CSTransformRecordComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOSPACE_API UCSTransformRecordComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSTransformRecordComponent(); 

public: 
	const TArray<FCSF_CharacterFrameData>& GetTransformHistory() { return TransformHistory; }
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	void RecordTransform();

	// Character Frame Datas
	UPROPERTY()
	TArray<FCSF_CharacterFrameData> TransformHistory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	float RecordInterval = 0.03f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform History")
	int32 MaxHistorySize = 99;

protected:
	FTimerHandle RecordTimer;
};
