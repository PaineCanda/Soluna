// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainChunk.h"

// Sets default values
ATerrainChunk::ATerrainChunk()
{
	MeshLOD0 = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Generated MeshLOD0"));
	MeshLOD0NoCollision = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Generated MeshLOD0NoCollision"));
	MeshLOD1 = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Generated MeshLOD1"));
	MeshLOD2 = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Generated MeshLOD2"));
	MeshLOD3 = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Generated MeshLOD3"));

	RootComponent = MeshLOD0; // Make the original full resolution LOD the root component

	MeshLOD0NoCollision->SetupAttachment(RootComponent);
	MeshLOD1->SetupAttachment(RootComponent);
	MeshLOD2->SetupAttachment(RootComponent);
	MeshLOD3->SetupAttachment(RootComponent); // Attach them to the root component
}

// Called when the game starts or when spawned
void ATerrainChunk::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrainChunk::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called every frame
void ATerrainChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerrainChunk::UpdateTerrainChunk(FVector ViewerPosition3D, FTerrainDistanceSettings DistanceSettings)
{

	if ((NoiseMap.Num() == 0) || (ColourMap.Num() == 0)) {
		return;
	}

	FVector ClosestPoint = Bounds.GetClosestPointTo(ViewerPosition3D);
    float ClosestDistanceX = ClosestPoint.X - ViewerPosition3D.X;
	float ClosestDistanceY = ClosestPoint.Y - ViewerPosition3D.Y;
	float ViewerDistanceFromNearestChunk = FMath::Sqrt(ClosestDistanceX * ClosestDistanceX + ClosestDistanceY * ClosestDistanceY); // Uses Phytagora's to find hypotenuse between player and closest point.
	bIsVisible = ViewerDistanceFromNearestChunk <= DistanceSettings.MaxViewDistance;
	SetVisibilityForTerrainChunk(bIsVisible);

	int NewLevelOfDetail = 3;
	bool bNewCollisionSetting = true;

	if (ViewerDistanceFromNearestChunk > DistanceSettings.ThirdLevelOfDetailDistance) {
			NewLevelOfDetail = 3;
			bNewCollisionSetting = false;
	}
	else if (ViewerDistanceFromNearestChunk > DistanceSettings.SecondLevelOfDetailDistance) {

			NewLevelOfDetail = 2;
			bNewCollisionSetting = false;
	}
	else if (ViewerDistanceFromNearestChunk > DistanceSettings.FirstLevelOfDetailDistance) {
		
			NewLevelOfDetail = 1;
			bNewCollisionSetting = false;
	}
	else if (ViewerDistanceFromNearestChunk > DistanceSettings.FullDetailNoCollisionDistance) {
		
			NewLevelOfDetail = 0;
			bNewCollisionSetting = false;
	}
	else {	
			NewLevelOfDetail = 0;
			bNewCollisionSetting = true;
	}

	MeshLOD0->SetMeshSectionVisible(0, false);
	MeshLOD0NoCollision->SetMeshSectionVisible(0, false);
	MeshLOD1->SetMeshSectionVisible(0, false);
	MeshLOD3->SetMeshSectionVisible(0, false);
	MeshLOD2->SetMeshSectionVisible(0, false); // Set visibility to false for all Meshes with different LODS
	
	MeshLOD0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshLOD0NoCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshLOD1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshLOD2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshLOD3->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Set collision to false for all Meshes with different LODS to ensure only the one that needs to have collision has it


	int MeshIndex = GetMeshIndex(NewLevelOfDetail, bNewCollisionSetting);
	UProceduralMeshComponent* NewMesh = GetMeshFromMeshIndex(MeshIndex); // Gets Mesh pointer

	if (!NewMesh) {
		return;
	}


	if (!(bLODGenerated[MeshIndex]) && !(bLODIsGenerating[MeshIndex])) // Generate this Mesh if it hasn't been generated or generating at the moment
	{
		bLODIsGenerating[MeshIndex] = true; // Set Is Generating to true
		CreateMesh(NewMesh, NewLevelOfDetail, bNewCollisionSetting, MeshIndex);
		if (BiomeSettings.bAdditionalFeatures) {
			BiomeSpecificBehavior();
		}
	}
	else if (bLODGenerated[MeshIndex]) { // If it is generated, then...
		NewMesh->SetMeshSectionVisible(0, true);

		if (bNewCollisionSetting) {  // Enable collision accordingly
			NewMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}

}

void ATerrainChunk::SetVisibilityForTerrainChunk(bool IsVisible)
{
	this->SetActorHiddenInGame(!IsVisible);
}

bool ATerrainChunk::IsVisible()
{
	return !IsHidden();
}

void ATerrainChunk::SetDefaultsForInfiniteTerrain(FVector2D Coordinates, int32 Size, FVector2D Offset, ENormalizeMode GenerationMode) {
	ChunkCoord = Coordinates;
	FVector2D Position = Coordinates * Size; // Transforms back into position vectors.
	FVector PositionIn3DSpace = FVector(Position.X, Position.Y, 0); // Makes it in 3D space
	Bounds = FBox::BuildAABB(PositionIn3DSpace, (FVector::One() * Size) / 2); // Builds a Box with centre PositionIn3DSpace and extent of half of the chunk size in all directions.
	ChunkSettings.Offset.X = Offset.X;
	ChunkSettings.Offset.Y = Offset.Y;
	ChunkSettings.NormalizeMode = GenerationMode;
}

UProceduralMeshComponent* ATerrainChunk::GetMeshFromMeshIndex(int MeshIndex)
{
	switch (MeshIndex) {
	case 0 : return MeshLOD0;
	case 1 : return MeshLOD0NoCollision;
	case 2 : return MeshLOD1;
	case 3 : return MeshLOD2;
	case 4 : return MeshLOD3;
	default: return nullptr;
	}
	
}

int ATerrainChunk::GetMeshIndex(int LOD, bool CollisionSetting)
{
	if (LOD == 0) {
		return CollisionSetting ?  0 : 1; // Makes sure we are at the correct index in our LODGenerated;
	}

	return LOD + 1;
}

void ATerrainChunk::BiomeSpecificBehavior()
{
	for (int y = 0; y < ChunkSettings.ChunkSize; y++) {
		for (int x = 0; x < ChunkSettings.ChunkSize; x++) {
			int Index = y * ChunkSettings.ChunkSize + x;
			EBiomes Biome;

			for (int i = 0; i < Biomes.Num(); i++)
			{
				if (BiomeMap[Index] <= Biomes[i].HeightThreshold)
				{
					Biome = Biomes[i].Biome;
					break;
				}
			}

			if (Biome == EBiomes::SunnyPlains) {
				// Code that relates to SunnyPlains Biome


			}

			if (Biome == EBiomes::MoonForests) {
				// Code that relates to MoonForests Biome
			}

			if (Biome == EBiomes::Mountains) {
				// Code that relates to Mountains Biome
			}
		}
	}
}


void ATerrainChunk::DisplayMap(UTexture2D* Texture)
{
	if (!DynamicMaterial) 
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		MeshLOD0->SetMaterial(0, DynamicMaterial);
		MeshLOD0NoCollision->SetMaterial(0, DynamicMaterial);
		MeshLOD1->SetMaterial(0, DynamicMaterial);
		MeshLOD2->SetMaterial(0, DynamicMaterial);
		MeshLOD3->SetMaterial(0, DynamicMaterial);
	}

	DynamicMaterial->SetTextureParameterValue(FName("GeneratedTexture"), Texture);
}


void ATerrainChunk::GenerateChunk(FVector ViewerPosition3D, FTerrainDistanceSettings DistanceSettings)
{
	FTerrainSettings TerrainSettings = ChunkSettings;
	FBiomeSettings LocalBiomeSettings = BiomeSettings;
	LocalBiomeSettings.ChunkSize = TerrainSettings.ChunkSize;
	LocalBiomeSettings.Seed = TerrainSettings.Seed;
	LocalBiomeSettings.Offset = TerrainSettings.Offset;
	LocalBiomeSettings.NormalizeMode = TerrainSettings.NormalizeMode;
	TArray<FTerrainTypeData> ChunkRegions = Regions;
	TArray<FBiomesData> LocalBiomes = Biomes;
	EDrawMode LocalDrawMode = DrawMode;

	Async(EAsyncExecution::ThreadPool, [this, TerrainSettings = MoveTemp(TerrainSettings), LocalBiomeSettings = MoveTemp(LocalBiomeSettings), ChunkRegions = MoveTemp(ChunkRegions), LocalBiomes = MoveTemp(LocalBiomes), LocalDrawMode = MoveTemp(LocalDrawMode), ViewerPosition3D, DistanceSettings]()
		{
			NoiseMap = MapGenerator.GenerateNoiseMap(TerrainSettings.ChunkSize, TerrainSettings.ChunkSize, TerrainSettings.Seed, TerrainSettings.Octaves, TerrainSettings.Persistence, TerrainSettings.Lacunarity, TerrainSettings.NoiseScale, TerrainSettings.Offset, TerrainSettings.NormalizeMode);


			BiomeMap = MapGenerator.GenerateNoiseMap(LocalBiomeSettings.ChunkSize, LocalBiomeSettings.ChunkSize, LocalBiomeSettings.Seed, LocalBiomeSettings.Octaves, LocalBiomeSettings.Persistence, LocalBiomeSettings.Lacunarity, LocalBiomeSettings.NoiseScale, LocalBiomeSettings.Offset, LocalBiomeSettings.NormalizeMode);


			ColourMap = MapGenerator.GenerateColouredMap(TerrainSettings.ChunkSize, TerrainSettings.ChunkSize, NoiseMap, BiomeMap, ChunkRegions, LocalBiomes);

			AsyncTask(ENamedThreads::GameThread, [TerrainSettings, this, ViewerPosition3D, DistanceSettings]() {
				GeneratedTexture = TextureGenerator.TextureFromColourMap(ColourMap, TerrainSettings.ChunkSize, TerrainSettings.ChunkSize);
				DisplayMap(GeneratedTexture);
				UpdateTerrainChunk(ViewerPosition3D, DistanceSettings);
				});
		});

}

void ATerrainChunk::CreateMesh(UProceduralMeshComponent* Mesh, int LOD, bool ShouldHaveColision, int MeshIndex)
{
	FTerrainSettings Settings = ChunkSettings;
	TArray<float> LocalNoiseMap = NoiseMap;
	TArray<FColor> LocalColourMap = ColourMap;

	Async(EAsyncExecution::ThreadPool, [MeshIndex, Mesh, LOD, LocalNoiseMap, LocalColourMap, Settings = MoveTemp(Settings), this, ShouldHaveColision]() {
		FMeshData MeshData = MeshGenerator.GenerateTerrainMeshData(Settings.ChunkSize, Settings.ChunkSize, LocalNoiseMap, LocalColourMap, Settings.HeightMultiplier, Settings.MeshHeightCurve, LOD, Settings.MeshSpacing);
		AsyncTask(ENamedThreads::GameThread, [MeshIndex, Mesh, LOD, MeshData, ShouldHaveColision, this]() {
			Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UVs, MeshData.VerticesColours, {}, ShouldHaveColision); // Change the last parameter to true when you need colision

			if (ShouldHaveColision) {
				Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Ensure Collision is set to true if need be.
			}
			else {
				Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			bLODGenerated[MeshIndex] = true;
			bLODIsGenerating[MeshIndex] = false;
			});
		});
}
