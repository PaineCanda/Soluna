// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainType.generated.h"

/**
 * 
 */

 // Fill out your copyright notice in the Description page of Project Settings.

UENUM()
enum class ETerrainType : uint8 // Will allow to spawn different things based on terrain type
{
	ShallowWater UMETA(DisplayName = "Shallow Water"),
	DeepWater UMETA(DisplayName = "Deep Water"),
	Sand UMETA(DisplayName = "Sand"),
	Grass UMETA(DisplayName = "Grass"),
	Grass_2 UMETA(DisplayName = "Grass 2"), // different type of grass
	Rock UMETA(DisplayName = "Rock"),
	Rock_2 UMETA(DisplayName = "Rock 2"), // different type of rock
	Snow UMETA(DisplayName = "Snow")
};

USTRUCT(BlueprintType)
struct FTerrainTypeData // Data that each terrain defines
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETerrainType TerrainType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Colour;

};

