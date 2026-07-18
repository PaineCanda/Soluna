// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SOLUNA_API PerlinNoiseMapTexture
{

public:
	PerlinNoiseMapTexture();
	~PerlinNoiseMapTexture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	int mapWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	int mapHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	float noiseScale;


	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	TArray<float> GenerateNoiseMap();

	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	void GenerateMap();

	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	void MapDisplay(const TArray<float>& noiseMap);

};
