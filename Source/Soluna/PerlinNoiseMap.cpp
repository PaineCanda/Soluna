// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseMap.h"

// Sets default values
APerlinNoiseMap::APerlinNoiseMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APerlinNoiseMap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	generatedPerlinNoiseTexture = GenerateTexture(GenerateNoiseMap());
}

// Called when the game starts or when spawned
void APerlinNoiseMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerlinNoiseMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<float> APerlinNoiseMap::GenerateNoiseMap()
{
	TArray<float> noiseMap;
	noiseMap.SetNum(mapWidth * mapHeight);

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

void APerlinNoiseMap::GenerateMap()
{
	TArray<float> noiseMap = GenerateNoiseMap();

}

UTexture2D* APerlinNoiseMap::GenerateTexture(const TArray<float>& noiseMap)
{
	UTexture2D* texture = UTexture2D::CreateTransient(mapWidth, mapHeight, PF_B8G8R8A8);
	TArray<FColor> colourMap;
	colourMap.SetNum(mapWidth * mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++) {
			int index = y * mapWidth + x;
			colourMap[index] = FColor(noiseMap[index] * 255, noiseMap[index] * 255, noiseMap[index] * 255, 255);
		}
	}
	if (texture == nullptr)
	{
		return nullptr;
	}
	FTexture2DMipMap& mip = texture->GetPlatformData()->Mips[0];
	void* TexturedData = mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TexturedData, colourMap.GetData(), colourMap.Num() * sizeof(FColor));
	mip.BulkData.Unlock();
	texture->UpdateResource();
	return texture;
}