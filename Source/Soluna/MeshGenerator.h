// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class SOLUNA_API FMeshData
{
public:

	FMeshData(int32 Width, int32 Height); // Constructor

	TArray<FVector> Vertices;
	TArray<FVector2D> UVs;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FColor> VerticesColours;
	int32 TriangleIndex = 0;

	void AddTriangle(int32 first_vertex, int32 second_vertex, int32 third_vertex);

	TArray<FVector> CalculateNormalForVertices();

};

class SOLUNA_API FMeshGenerator
{
public:

	FMeshData GenerateTerrainMeshData(int32 Width, int32 Height, const TArray<float>& HeightMap, const TArray<FColor>& ColourMap, float HeightMultiplier, UCurveFloat* MeshHeightCurve, int LevelOfDetail, int MeshSpacing);

};