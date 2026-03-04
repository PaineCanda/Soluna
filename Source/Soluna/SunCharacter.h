// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SolunaBaseCharacter.h"
#include "SunAttributeSet.h"
#include "SunCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SOLUNA_API ASunCharacter : public ASolunaBaseCharacter
{
	GENERATED_BODY()

	ASunCharacter();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	USunAttributeSet* SunAttributeSet;
	
};
 