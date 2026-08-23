// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerlinNoiseRealtedMapGenerator.h"
#include "Engine/Texture2D.h"

/**
 * 
 */
class SOLUNA_API FTextureGenerator
{
public:

	FPerlinNoiseRelatedMapGenerator ColouredMapGenerator;

	UTexture2D* TextureFromColourMap(const TArray<FColor>& ColourMap, int32 Width, int32 Height);

	UTexture2D* TextureFromHeightMap(const TArray<float>& HeightMap, int32 Width, int32 Height);
};
