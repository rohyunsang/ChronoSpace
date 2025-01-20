// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CSAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class CHRONOSPACE_API UCSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UCSAttributeSet();

	ATTRIBUTE_ACCESSORS(UCSAttributeSet, Energy);
	ATTRIBUTE_ACCESSORS(UCSAttributeSet, MaxEnergy);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Energy", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Energy;

	UPROPERTY(BlueprintReadOnly, Category = "Energy", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxEnergy;
	
};
