// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainSettings.h"
#include "TextureGenerator.h"
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "TerrainType.h"
#include "PerlinNoiseRealtedMapGenerator.h"
#include "TerrainDistanceSettings.h"
#include "TerrainChunk.generated.h"


UENUM()
enum class EDrawMode : uint8 // How we can visualise the map
{
	NoiseMap UMETA(DisplayName = "Noise Map"),
	ColourMap UMETA(DisplayName = "Colour Map"),
	Mesh UMETA(DisplayName = "Mesh")
};


UCLASS()
class SOLUNA_API ATerrainChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainChunk();
	
	public:
		FBox Bounds;
		bool bIsVisible;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainGeneration")
		FTerrainSettings ChunkSettings;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D ChunkCoord;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterialInterface* BaseMaterial; // Base material to be used for the dynamic material instance

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
		UMaterialInstanceDynamic* DynamicMaterial; // Assigns the generated texture to the material instance

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		TArray<FTerrainTypeData> Regions; // Different type of terrain to be generated based on height

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		EDrawMode DrawMode; // How we want to visualise the map

		UPROPERTY(VisibleAnywhere, Category = "Texture")
		UTexture2D* GeneratedTexture; // The texture generated from the noise map

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UProceduralMeshComponent* MeshLOD0;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UProceduralMeshComponent* MeshLOD0NoCollision;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UProceduralMeshComponent* MeshLOD1;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UProceduralMeshComponent* MeshLOD2;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UProceduralMeshComponent* MeshLOD3;



		bool bLODGenerated[5] = { false, false, false, false, false };

		bool bLODIsGenerating[5] = { false, false, false, false, false };

		TArray<float> NoiseMap;

		TArray<FColor> ColourMap;

		// Instances of other classes

		FPerlinNoiseRelatedMapGenerator MapGenerator;

		FTextureGenerator TextureGenerator; // Instance of the texture generator class to generate textures from noise and colour maps

		FMeshGenerator MeshGenerator; // Creates the mesh


		// Functions


		UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
		void GenerateChunk(FVector ViewerPosition3D, FTerrainDistanceSettings DistanceSettings);

		UFUNCTION(BlueprintCallable, Category = "Display")
		void DisplayMap(UTexture2D* Texture);

		UFUNCTION(BlueprintCallable, Category = "Mesh")
		void CreateMesh(UProceduralMeshComponent* Mesh, int LOD, bool ShouldHaveColision, int MeshIndex);

		void UpdateTerrainChunk(FVector ViewerPosition, FTerrainDistanceSettings DistanceSettings);
		void SetVisibilityForTerrainChunk(bool IsVisible);
		bool IsVisible();
		void SetDefaultsForInfiniteTerrain(FVector2D Coordinates, int32 Size, FVector2D Offset, ENormalizeMode GenerationMode);
		UProceduralMeshComponent* GetMeshFromMeshIndex(int MeshIndex);
		int GetMeshIndex(int LOD, bool CollisionSetting);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
