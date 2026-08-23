// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalizeMode.h"
#include "TerrainSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTerrainSettings 
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ChunkSize = 513; // Needs to be a power of 2 + 1 in order to work correctly with our level of detail

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "7"))
	int LevelOfDetail; // Will skip vertices as this value increases which will lead to a lower level of detail.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int Seed = 22; // Same seed allows for the same map to be generated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int Octaves = 4; // Levels of detail for the noise

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Persistence = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
	float Lacunarity = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float NoiseScale = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Offset;  // Allows scrolling through the map

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float HeightMultiplier = 750;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* MeshHeightCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MeshSpacing = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENormalizeMode NormalizeMode = ENormalizeMode::Global;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldHaveCollision = true;

};

