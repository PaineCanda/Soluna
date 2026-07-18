// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinNoiseMap.generated.h"

UCLASS()
class SOLUNA_API APerlinNoiseMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinNoiseMap();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNoise")
	int mapWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNoise")
	int mapHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNoise")
	float noiseScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* generatedPerlinNoiseTexture;


	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	TArray<float> GenerateNoiseMap();

	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	void GenerateMap();

	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	UTexture2D* GenerateTexture(const TArray<float>& noiseMap);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
