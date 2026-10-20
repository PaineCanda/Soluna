// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

FMeshData FMeshGenerator::GenerateTerrainMeshData(int32 Width, int32 Height, const TArray<float>& HeightMap, const TArray<FColor>& ColourMap, float HeightMultiplier, UCurveFloat* MeshHeightCurve, int LevelOfDetail, int MeshSpacing)
{

	int MeshSimplificationIncrement = 1 << LevelOfDetail; // Shift bits to the left. Basically does 2 to the power of LevelofDetail. If LOD is 0, it will be 2^0 = 1, if LOD is 1, it will be 2^1 = 2 and so forth...
	int BorderedSize = Width;
	int MeshSize = BorderedSize - 2;
	int MeshSizeUnsimplified = BorderedSize - 2;


	int VerticesPerLine = ((MeshSize - 1) / MeshSimplificationIncrement) + 1; // How Many vertices there actually are in one line
	
	FMeshData MeshData(VerticesPerLine); // Create MeshData

	if (MeshHeightCurve == nullptr) {
		return MeshData; // Stops Unreal from crashing
	}

	
	int MeshVertex = 0;
	int BorderVertexIndex = -1;
	TArray<int> VertexIndexMap;
	VertexIndexMap.SetNum(BorderedSize * BorderedSize);


	float TopLeft = (MeshSizeUnsimplified - 1) / 2.f; // To center the mesh alongside the x-axis and y-axis.

	for (int y = 0; y < BorderedSize; y += MeshSimplificationIncrement) {
		for (int x = 0; x < BorderedSize; x += MeshSimplificationIncrement) {
			int Index = y * BorderedSize + x;
			bool bIsBorderedVertex = x == 0 || y == 0 || x == BorderedSize - 1 || y == BorderedSize - 1;
			
			if (bIsBorderedVertex) {
				VertexIndexMap[Index] = BorderVertexIndex;
				BorderVertexIndex--;
			}
			else {
				VertexIndexMap[Index] = MeshVertex;
				MeshVertex++;
			}

		}
	}



	for (int y = 0; y < BorderedSize; y += MeshSimplificationIncrement) {
		for (int x = 0; x < BorderedSize; x += MeshSimplificationIncrement) {

			int32 Index = y * BorderedSize + x;
			int VertexIndex = VertexIndexMap[Index];

			FVector2D Percent = FVector2D((x - MeshSimplificationIncrement)/ float(MeshSize - 1), (y - MeshSimplificationIncrement) / float(MeshSize - 1)); // Getting the values in a range of 0 to 1, which is what we need for UVs. Minusing the MeshSimplificationIncrement to have 0 for the UVs in our MeshSpacing.

			FVector VectorPosition = FVector((Percent.X * (MeshSizeUnsimplified - 1) - TopLeft) * MeshSpacing, (Percent.Y * (MeshSizeUnsimplified - 1) - TopLeft) * MeshSpacing, MeshHeightCurve->GetFloatValue(HeightMap[Index]) * HeightMultiplier); // Centering the mesh through the origin and multipliying the Z value. Smoothing out noise through a custom curve. There are only 6 spaces between 7 vertices therefore we need MeshSizeUnsimplified - 1 to correctly space mesh in world position. Using Percent to make sure that meshes with lower lod still cover the entire range of values.


			MeshData.AddVertex(VectorPosition, Percent, VertexIndex, ColourMap[Index]); // Gets the color from the colour map only for the needed vertices

			if (x < BorderedSize - 1 && y < BorderedSize - 1)
			{
				int A = VertexIndexMap[Index];
				int B = VertexIndexMap[Index + MeshSimplificationIncrement];
				int C = VertexIndexMap[Index + BorderedSize * MeshSimplificationIncrement];
				int D = VertexIndexMap[Index + BorderedSize * MeshSimplificationIncrement + MeshSimplificationIncrement];
				MeshData.AddTriangle(A, C, D); // Getting the vertices added in an anticlockwise manner
				MeshData.AddTriangle(D, B, A);
			}
		}
	}
	MeshData.Normals = MeshData.CalculateNormalForVertices();
	return MeshData;
}



FMeshData::FMeshData(int32 VerticesPerLine)
{
	int32 VertexCount = VerticesPerLine * VerticesPerLine;

	BorderVertices.SetNum(VerticesPerLine * 4 + 4);
	BorderTriangles.Reserve(VerticesPerLine * 24); // Reserve this many, but if there's more allocate dynamically.
	Vertices.SetNum(VerticesPerLine * VerticesPerLine);
	Triangles.SetNum((VerticesPerLine - 1) * (VerticesPerLine - 1) * 6); // Each quad consists of 2 triangles, each triangle has 3 vertices. For a 3 by 3 grid, there are a total of 4 quads, 2*2 * 6 (vertices from both triangels from the quads)


	UVs.Init(FVector2D::ZeroVector, VertexCount); // Sets the array with size VertexCount and filling every element with a ZeroVector
	Normals.Init(FVector::ZeroVector, VertexCount); // Sets the array with size VertexCount and filling every element with a ZeroVector
	VerticesColours.Init(FColor::White, VertexCount); // Fills the array with WhiteColor with VertexCount elements.
}

void FMeshData::AddTriangle(int32 first_vertex, int32 second_vertex, int32 third_vertex)
{
	if (first_vertex < 0 || second_vertex < 0 || third_vertex < 0) {
		BorderTriangles.Add(first_vertex); // Add vertices of the triangles to the array in order
		BorderTriangles.Add(second_vertex); // Add vertices of the triangles to the array in order
		BorderTriangles.Add(third_vertex); // Add vertices of the triangles to the array in order
		BorderTriangleIndex += 3;
		
	}
	else {
		Triangles[TriangleIndex] = first_vertex; // Add vertices of the triangles to the array in order
		Triangles[TriangleIndex + 1] = second_vertex;
		Triangles[TriangleIndex + 2] = third_vertex;
		TriangleIndex += 3;
	}
}

void FMeshData::AddVertex(FVector VectorPosition, FVector2D UV, int VertexIndex, FColor VertexColour)
{
	if (VertexIndex < 0) {
		BorderVertices[-VertexIndex - 1] = VectorPosition;
	}
	else {
		Vertices[VertexIndex] = VectorPosition;
		UVs[VertexIndex] = UV;
		VerticesColours[VertexIndex] = VertexColour;
	}
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

	for (int i = 0; i < BorderTriangles.Num() ; i += 3)
	{
		int32 IndexA = BorderTriangles[i]; // Gives the indexes for our vertices.
		int32 IndexB = BorderTriangles[i + 1];
		int32 IndexC = BorderTriangles[i + 2];

		FVector A = (IndexA < 0) ? BorderVertices[-IndexA - 1] : Vertices[IndexA]; // Gives the value of the Vertex at IndexA, which is initially calculated in the GenerateTerrainMesh function
		FVector B = (IndexB < 0) ? BorderVertices[-IndexB - 1] : Vertices[IndexB];
		FVector C = (IndexC < 0) ? BorderVertices[-IndexC - 1] : Vertices[IndexC];

		FVector EdgeAB = B - A; // Calculates the direction from A to B
		FVector EdgeAC = C - A; // Calculates the direction from A to C

		FVector Normal = FVector::CrossProduct(EdgeAC, EdgeAB); // Normal for the vertices in Triangle ABC. Their order matter otherwise the normals can be flipped 180 which can result into an unlit mesh.
		
		if (IndexA >= 0) {
			Normals[IndexA] += Normal; // Adds the normal of the current triangle to this position that uses the vertex with IndexA as one of its vertices.
		}
		if (IndexB >= 0) {
			Normals[IndexB] += Normal; // Adds the normal of the current triangle to this position that uses the vertex with IndexA as one of its vertices.
		}
		if (IndexC >= 0) {
			Normals[IndexC] += Normal; // Adds the normal of the current triangle to this position that uses the vertex with IndexA as one of its vertices.
		}
	}

	for (int i = 0; i < Normals.Num(); i++)
	{
		Normals[i].Normalize(); // Mean of normals for that vertex
	}

	return Normals;
}
