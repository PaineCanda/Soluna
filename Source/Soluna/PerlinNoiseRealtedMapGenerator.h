// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalizeMode.h"
#include "TerrainType.h"

/**
 * 
 */
class SOLUNA_API FPerlinNoiseRelatedMapGenerator
{
public:
	
	TArray<float> GenerateNoiseMap(int MapWidth, int MapHeight, int Seed, int Octaves, float Persistence, float Lacunarity, float NoiseScale, FVector2D Offset, ENormalizeMode NormalizeMode);

	TArray<FColor> GenerateBlackAndWhiteMap(int32 MapWidth, int32 MapHeight, const TArray<float>& HeightMap);

	TArray<FColor> GenerateColouredMap(int32 MapWidth, int32 MapHeight, const TArray<float>& NoiseMap, const TArray<FTerrainTypeData>& Regions);


};
