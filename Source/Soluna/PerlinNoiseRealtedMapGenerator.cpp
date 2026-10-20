// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseRealtedMapGenerator.h"
#include "Math/NumericLimits.h"


TArray<float> FPerlinNoiseRelatedMapGenerator::GenerateNoiseMap(int MapWidth, int MapHeight, int Seed, int Octaves, float Persistence, float Lacunarity, float NoiseScale, FVector2D Offset, ENormalizeMode NormalizeMode)
{

	TArray<float> NoiseMap;
	NoiseMap.SetNum(MapWidth * MapHeight);

	float MaxPossibleNoiseHeigt = 0;
	float Amplitude = 1.0f;
	float Frequency = 1.0f;

	FRandomStream prng(Seed); // Create a pseudo random number generator with the given seed
	TArray<FVector2D> OctaveOffsets;
	OctaveOffsets.SetNum(Octaves);

	for (int i = 0; i < Octaves; i++) {
		float OffsetX = prng.FRandRange(-100000, 100000) + Offset.X; // Generate a random offset for each octave to avoid repetition in the noise pattern
		float OffsetY = prng.FRandRange(-100000, 100000) + Offset.Y; // Generate a random offset for each octave to avoid repetition in the noise pattern
		OctaveOffsets[i] = FVector2D(OffsetX, OffsetY);
		MaxPossibleNoiseHeigt += Amplitude;
		Amplitude *= Persistence;
	}


	NoiseScale <= 0 ? NoiseScale = 0.0001f : NoiseScale;

	float MaxNoiseHeight = TNumericLimits<float>::Lowest(); // Ensures we use highest float later on
	float MinNoiseHeight = TNumericLimits<float>::Max(); // Ensures we use lowest float later on

	float HalfWidth = MapWidth / 2.0f;  //Allows to scale into the middle of the map
	float HalfHeight = MapHeight / 2.0f;

	for (int y = 0; y < MapHeight; y++) 
		{
			for (int x = 0; x < MapWidth; x++)
			{
				Amplitude = 1.0f;
				Frequency = 1.0f;
				float NoiseHeight = 0.0f;


				for (int i = 0; i < Octaves; i++) {
					float SampleX = ((x - HalfWidth) + OctaveOffsets[i].X) / NoiseScale * Frequency;
					float SampleY = ((y - HalfHeight) + OctaveOffsets[i].Y) / NoiseScale * Frequency;
					float PerlinValue = FMath::PerlinNoise2D(FVector2D(SampleX, SampleY));

					NoiseHeight += PerlinValue * Amplitude;

					Amplitude *= Persistence;
					Frequency *= Lacunarity;

				}

				if (NoiseHeight > MaxNoiseHeight) {
					MaxNoiseHeight = NoiseHeight;
				}
				else if (NoiseHeight < MinNoiseHeight) {
					MinNoiseHeight = NoiseHeight;
				}

				int index = y * MapWidth + x;
				NoiseMap[index] = NoiseHeight;
			}
		}

	ParallelFor(MapHeight, [&](int32 y)
		{
			for (int x = 0; x < MapWidth; x++)
			{
				int index = y * MapWidth + x;
				if (NormalizeMode == ENormalizeMode::Local)
				{
					NoiseMap[index] = (NoiseMap[index] - MinNoiseHeight) / (MaxNoiseHeight - MinNoiseHeight); // Normalize to [0, 1]
				}
				else if (NormalizeMode == ENormalizeMode::Global)
				{
					float NormalizedHeight = FMath::Clamp((NoiseMap[index] + 1) / (2.f * MaxPossibleNoiseHeigt / 1.9f), 0, 1); // Normalize to [0 ; 1] and make every chunk have the same NormalizedHeight as there is a fixed variable MaxPossibleNoiseHeight that is shared across all chunks
					NoiseMap[index] = NormalizedHeight;
				}
			}
		});
	return NoiseMap;
}

TArray<FColor> FPerlinNoiseRelatedMapGenerator::GenerateBlackAndWhiteMap(int32 MapWidth, int32 MapHeight, const TArray<float>& HeightMap)
{
	TArray<FColor> BlackAndWhiteMap;
	BlackAndWhiteMap.SetNum(MapWidth * MapHeight);
	ParallelFor(MapHeight, [&](int32 y)
		{
			for (int x = 0; x < MapWidth; x++) {
				int index = y * MapWidth + x;
				BlackAndWhiteMap[index] = FColor(HeightMap[index] * 255, HeightMap[index] * 255, HeightMap[index] * 255, 255);
			}
		});

	return BlackAndWhiteMap;
}

TArray<FColor> FPerlinNoiseRelatedMapGenerator::GenerateColouredMap(int32 MapWidth, int32 MapHeight, const TArray<float>& NoiseMap, const TArray<float>& BiomeMap, const TArray<FTerrainTypeData>& Regions, const TArray<FBiomesData>& Biomes)
{
	TArray<FColor> ColourMap;
	ColourMap.SetNum(MapWidth * MapHeight);
	

	ParallelFor(MapHeight, [&](int32 y)
		{
			for (int x = 0; x < MapWidth; x++)
			{
				int index = y * MapWidth + x;

				FLinearColor RegionsColor;

				FLinearColor BiomeColor;


				float RegionsValue = NoiseMap[index];

				float BiomeValue = BiomeMap[index];

				for (int i = 0; i < Regions.Num(); i++)
				{
					if (RegionsValue <= Regions[i].HeightThreshold)
					{
						RegionsColor = Regions[i].Colour;
						break; // Exit the loop once the correct region is found
					}
				}

				if (RegionsValue <= 0.4) {
					ColourMap[index] = RegionsColor.ToFColor(true);
				}

				else {
					for (int i = 0; i < Biomes.Num(); i++)
					{
						if (BiomeValue <= Biomes[i].HeightThreshold)
						{
							BiomeColor = Biomes[i].Colour;
							break; // Exit the loop once the correct biome is found
						}
					}

					FLinearColor FinalColor = FMath::Lerp(RegionsColor, BiomeColor, 0.3);
					ColourMap[index] = FinalColor.ToFColor(true);
				}
			}
		});

	return ColourMap;
}

