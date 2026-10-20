// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteTerrain.h"

// Sets default values
AInfiniteTerrain::AInfiniteTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInfiniteTerrain::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		APawn* PlayerPawn = PC->GetPawn();
		if (PlayerPawn) {
			Viewer = PlayerPawn;
		}
	}
	Start();
	UpdateVisibleChunks();
}

// Called every frame
void AInfiniteTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ViewerPosition = FVector2D(Viewer->GetActorLocation().X, Viewer->GetActorLocation().Y);
	FVector2D NewChunkCoord = FVector2D(FMath::RoundToInt32(ViewerPosition.X / WorldChunkSize), FMath::RoundToInt32(ViewerPosition.Y / WorldChunkSize));
	if (!VectorPostionsQueue.IsEmpty()) { // If it's not empty, spawn chunk
		SpawnChunk();
	}

	if (NewChunkCoord != CurrentChunkCoord) {
		UpdateVisibleChunks();
	}
}

void AInfiniteTerrain::Start()
{
	ATerrainChunk* DefaultChunk = TerrainChunkClass->GetDefaultObject<ATerrainChunk>();
	WorldChunkSize = (DefaultChunk->ChunkSettings.ChunkSize - 3) * DefaultChunk->ChunkSettings.MeshSpacing;
	ChunkSize = (DefaultChunk->ChunkSettings.ChunkSize - 3);
	ChunksVisibleInViewDistance = FMath::RoundToInt32((float) DistanceSettings.MaxViewDistance / (float)WorldChunkSize);
}


void AInfiniteTerrain::UpdateVisibleChunks() 
{
	int CurrentChunkCoordX = FMath::RoundToInt32(ViewerPosition.X / WorldChunkSize); // Gets the chunk coordinate that will be visible to the player at this point in time.
	int CurrentChunkCoordY = FMath::RoundToInt32(ViewerPosition.Y / WorldChunkSize);
	CurrentChunkCoord = FVector2D(CurrentChunkCoordX, CurrentChunkCoordY);
	FVector ViewerPosition3D = Viewer->GetActorLocation();
	
	for (int i = TerrainChunksLastVisible.Num() - 1; i >= 0; i--) {
		ATerrainChunk* TerrainChunk = TerrainChunksLastVisible[i];
		if (FMath::Abs(TerrainChunk->ChunkCoord.X - CurrentChunkCoord.X) > ChunksVisibleInViewDistance || FMath::Abs(TerrainChunk->ChunkCoord.Y - CurrentChunkCoord.Y) > ChunksVisibleInViewDistance) 
		{
			TerrainChunk->SetVisibilityForTerrainChunk(false);
			TerrainChunksLastVisible.RemoveAt(i);
		}
	}

	VectorPostionsQueue.Empty();

	for (int YOffset = -ChunksVisibleInViewDistance; YOffset <= ChunksVisibleInViewDistance; YOffset++) {
		for (int XOffset = -ChunksVisibleInViewDistance; XOffset <= ChunksVisibleInViewDistance; XOffset++) { // Creates a grid from the extreme values. A mix and match between all of the values from -1 to 1 on a grid.
			FVector2D ChunkAroundPlayer = FVector2D(CurrentChunkCoordX + XOffset, CurrentChunkCoordY + YOffset);
			if (TerrainChunksDictionary.Contains(ChunkAroundPlayer)) {
				TerrainChunksDictionary[ChunkAroundPlayer]->UpdateTerrainChunk(ViewerPosition3D, DistanceSettings); // Updates the chunk
			}
			else {
				VectorPostionsQueue.Enqueue(ChunkAroundPlayer); // The Vector Coord at which the Chunk should spawn at
			}
		}
	}
}

void AInfiniteTerrain::SpawnChunk()
{

	FVector2D Position;
	if (VectorPostionsQueue.Dequeue(Position)) { // Gets the Vector Coord from the list and stores it in Postion
		if (CoordsForChunksAlreadySpawned.Contains(Position))
		{
			return;
		}
		else {
			ATerrainChunk* Chunk = GetWorld()->SpawnActor<ATerrainChunk>(TerrainChunkClass, FTransform(FVector(Position.X * WorldChunkSize, Position.Y * WorldChunkSize, 0))); // Makes Transform from Vector with coordinates of the chunk to place the Chunk of class TerrainChunkClass in the Transform position
			FVector2D Offset = FVector2D(Position.X * ChunkSize, Position.Y * ChunkSize);
			Chunk->SetDefaultsForInfiniteTerrain(Position, WorldChunkSize, Offset, ENormalizeMode::Global);
			Chunk->GenerateChunk(Viewer->GetActorLocation(), DistanceSettings);
			TerrainChunksDictionary.Add(Position, Chunk);
			TerrainChunksLastVisible.Add(Chunk); // Adds this specific chunk to the dictionary
			CoordsForChunksAlreadySpawned.Add(Position);
		}
	}
}