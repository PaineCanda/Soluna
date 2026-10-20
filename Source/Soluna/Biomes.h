// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Biomes.generated.h"

/**
 * 
 */
 
UENUM()
enum class EBiomes : uint8
{
	SunnyPlains UMETA(DisplayName = "Sunny Plains"),
	MoonForests UMETA(DisplayName = "Moon Forests"),
	Mountains UMETA(DisplayName = "Mountains")
	
};

USTRUCT(BlueprintType)
struct FBiomesData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBiomes Biome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Colour;

};
