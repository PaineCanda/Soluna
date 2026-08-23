// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainChunk.h"
#include "GameFramework/Actor.h"
#include "TerrainDistanceSettings.h"
#include "InfiniteTerrain.generated.h"



UCLASS()
class SOLUNA_API AInfiniteTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInfiniteTerrain();

	UPROPERTY(BlueprintReadWrite)
	FTerrainDistanceSettings DistanceSettings;
	
	UPROPERTY(BlueprintReadWrite)
	int ChunkSize;

	UPROPERTY(BlueprintReadWrite)
	int WorldChunkSize;

	UPROPERTY(BlueprintReadWrite)
	int ChunksVisibleInViewDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	AActor* Viewer;

	UPROPERTY(BlueprintReadWrite)
	FVector2D ViewerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D CurrentChunkCoord = FVector2D(0, 0);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainChunk")
	TSubclassOf<ATerrainChunk> TerrainChunkClass; // Can set what should use as "TerrainChunk" template. Works with blueprints as well

	TQueue<FVector2D> VectorPostionsQueue;
	TArray<FVector2D> CoordsForChunksAlreadySpawned;


	UPROPERTY(BlueprintReadWrite)
	TMap<FVector2D, ATerrainChunk*> TerrainChunksDictionary;




	UPROPERTY(BlueprintReadWrite)
	TArray<ATerrainChunk*> TerrainChunksLastVisible;

	UFUNCTION(BlueprintCallable)
	void Start();

	void SpawnChunk();

	void UpdateVisibleChunks();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
