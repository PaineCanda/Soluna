// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainDistanceSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTerrainDistanceSettings 
{
	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int FullDetailNoCollisionDistance = 500;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int FirstLevelOfDetailDistance = 3000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int SecondLevelOfDetailDistance = 4000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ThirdLevelOfDetailDistance = 5000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxViewDistance = 8000;

};
