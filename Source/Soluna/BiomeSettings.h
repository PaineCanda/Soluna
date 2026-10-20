// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NormalizeMode.h"
#include "CoreMinimal.h"
#include "BiomeSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FBiomeSettings
{

	GENERATED_BODY()

	int ChunkSize = 513;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int Seed = 22; // Same seed allows for the same map to be generated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int Octaves = 1; // Levels of detail for the noise

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Persistence = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
	float Lacunarity = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float NoiseScale = 4000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Offset;  // Allows scrolling through the map

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENormalizeMode NormalizeMode = ENormalizeMode::Global;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAdditionalFeatures = true;


};


