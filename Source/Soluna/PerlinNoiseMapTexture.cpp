// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseMapTexture.h"

PerlinNoiseMapTexture::PerlinNoiseMapTexture()
{
}

PerlinNoiseMapTexture::~PerlinNoiseMapTexture()
{
}

TArray<float> PerlinNoiseMapTexture::GenerateNoiseMap()
{
	TArray<float> noiseMap;

	noiseScale <= 0 ? noiseScale = 0.0001f : noiseScale; 

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			float sampleX = x / noiseScale;
			float sampleY = y / noiseScale;
			float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX, sampleY));
			float value = (perlinValue + 1.0f) / 2.0f; // Normalize to [0, 1]
			int index = y * mapWidth + x;
			noiseMap[index] = value;
		}
	}
	return noiseMap;
}

void PerlinNoiseMapTexture::GenerateMap()
{
	TArray<float> noiseMap = GenerateNoiseMap();

}

void PerlinNoiseMapTexture::MapDisplay( const TArray<float>& noiseMap)
{
	UTexture2D* texture = UTexture2D::CreateTransient(mapWidth, mapHeight);
	TArray<FLinearColor> colourMap;
	colourMap.SetNum(mapWidth * mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++) {
			int index = y * mapWidth + x;
			colourMap[index] = FMath::Lerp(FLinearColor::Black, FLinearColor::White, noiseMap[index]);
		}	
	}
	FTexture2DMipMap& mip = texture->GetPlatformData()->Mips[0];
	void* TexturedData = mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TexturedData, colourMap.GetData(), colourMap.Num() * sizeof(FLinearColor));
	mip.BulkData.Unlock();
	texture->UpdateResource();
}