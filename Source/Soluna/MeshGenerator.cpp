// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

FMeshData FMeshGenerator::GenerateTerrainMeshData(int32 Width, int32 Height, const TArray<float>& HeightMap, const TArray<FColor>& ColourMap, float HeightMultiplier, UCurveFloat* MeshHeightCurve, int LevelOfDetail, int MeshSpacing)
{



	int MeshSimplificationIncrement = 1 << LevelOfDetail; // Shift bits to the left. Basically does 2 to the power of LevelofDetail.
	int VerticesPerLine = ((Width - 1) / MeshSimplificationIncrement) + 1;
	
	FMeshData MeshData(VerticesPerLine, VerticesPerLine);

	if (MeshHeightCurve == nullptr) {
		return MeshData; // Stops Unreal from crashing
	}

	int32 VertexIndex = 0;
	float TopLeftX = (Width - 1) / 2.f; // To center the mesh alongside the x-axis
	float TopLeftY = (Height - 1) / 2.f; // To center the mesh alongside the y-axis

	
	VertexIndex = 0;
	for (int y = 0; y < Height; y += MeshSimplificationIncrement) {
		for (int x = 0; x < Width; x += MeshSimplificationIncrement) {

			int32 index = y * Width + x;
			MeshData.Vertices[VertexIndex] = FVector((x - TopLeftX) * MeshSpacing, (y - TopLeftY) * MeshSpacing, MeshHeightCurve->GetFloatValue(HeightMap[index]) * HeightMultiplier); // Centering the mesh through the origin and multipliying the Z value. Smoothing out noise through a custom curve
			MeshData.UVs[VertexIndex] = FVector2D(x / float(Width - 1), y / float(Height - 1)); // Getting the values in a range of 0 to 1, which is what we need for UVs

			MeshData.VerticesColours.Add(ColourMap[index]); // Gets the color from the colour map only for the needed vertices

			if (x < Width - 1 && y < Height - 1)
			{
				MeshData.AddTriangle(VertexIndex, VertexIndex + VerticesPerLine, VertexIndex + VerticesPerLine + 1); // Getting the vertices added in an anticlockwise manner
				MeshData.AddTriangle(VertexIndex + VerticesPerLine + 1, VertexIndex + 1, VertexIndex);
			}

			VertexIndex++;
		}
	}
	MeshData.Normals = MeshData.CalculateNormalForVertices();
	return MeshData;
}



FMeshData::FMeshData(int32 Width, int32 Height)
{
	Vertices.SetNum(Width * Height);
	Triangles.SetNum((Width - 1) * (Height - 1) * 6); // Each quad consists of 2 triangles, each triangle has 3 vertices. For a 3 by 3 grid, there are a total of 4 quads, 2*2.
	UVs.SetNum(Width * Height);
	Normals.SetNum(Width * Height);
}

void FMeshData::AddTriangle(int32 first_vertex, int32 second_vertex, int32 third_vertex)
{
	
	Triangles[TriangleIndex] = first_vertex; // Add vertices of the triangles to the array in order
	Triangles[TriangleIndex + 1] = second_vertex;
	Triangles[TriangleIndex + 2] = third_vertex;
	TriangleIndex += 3;
}

TArray<FVector> FMeshData::CalculateNormalForVertices()
{

	for (int i = 0; i < Triangles.Num() ; i += 3)
	{
		int32 IndexA = Triangles[i]; // Gives the indexes for our vertices.
		int32 IndexB = Triangles[i + 1];
		int32 IndexC = Triangles[i + 2];

		FVector A = Vertices[IndexA]; // Gives the value of the Vertex at IndexA, which is initially calculated in the GenerateTerrainMesh function
		FVector B = Vertices[IndexB];
		FVector C = Vertices[IndexC];

		FVector EdgeAB = B - A; // Calculates the direction from A to B
		FVector EdgeAC = C - A; // Calculates the direction from A to C

		FVector Normal = FVector::CrossProduct(EdgeAC, EdgeAB); // Normal for the vertices in Triangle ABC. Their order matter otherwise the normals can be flipped 180 which can result into an unlit mesh.

		Normals[IndexA] += Normal; // Adds the normal of the current triangle to this position that uses the vertex with IndexA as one of its vertices.
		Normals[IndexB] += Normal; // Adds the normal of the current triangle to this position that uses the vertex with IndexA as one of its vertices.
		Normals[IndexC] += Normal; // Adds the normal of the current triangle to this position that uses the vertex with IndexA as one of its vertices.
	}

	for (int i = 0; i < Normals.Num(); i++)
	{
		Normals[i].Normalize(); // Mean of normals for that vertex
	}

	return Normals;
}
