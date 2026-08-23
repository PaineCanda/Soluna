// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureGenerator.h"


UTexture2D* FTextureGenerator::TextureFromColourMap(const TArray<FColor>& ColourMap, int32 Width, int32 Height)
{

	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

	if (Texture == nullptr)
	{
		return nullptr;	
	}

	FTexture2DMipMap& mip = Texture->GetPlatformData()->Mips[0];
	void* TexturedData = mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TexturedData, ColourMap.GetData(), ColourMap.Num() * sizeof(FColor));
	mip.BulkData.Unlock();
	Texture->Filter = TF_Nearest; // Allows for the nearest value to be used when sampling the texture. No making a blend of colours from the surrounding pixels.
	Texture->AddressX = TA_Clamp; // Repeat the ending pixles instead of wrapping around to the other side of the texture. This is important for height maps as we don't want the edges to wrap around and create a seam.
	Texture->AddressY = TA_Clamp;
	Texture->UpdateResource();
	return Texture;
}

UTexture2D* FTextureGenerator::TextureFromHeightMap(const TArray<float>& HeightMap, int32 Width, int32 Height)
{
	TArray<FColor> ColourMap = ColouredMapGenerator.GenerateBlackAndWhiteMap(Width, Height, HeightMap);
	return TextureFromColourMap(ColourMap, Width, Height);
}
