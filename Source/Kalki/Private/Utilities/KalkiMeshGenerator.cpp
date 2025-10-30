// Copyright of V.S. Puranam and no one else

#include "Utilities//KalkiMeshGenerator.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshAttributes.h"
#include "MeshDescriptionBuilder.h"
#include "Logging/KalkiLog.h"

UStaticMesh* UKalkiMeshGenerator::CreateBorderFrameMesh(float OuterSize, float BorderWidth, float Height)
{
	// Validate parameters
	if (OuterSize <= 0.0f || BorderWidth <= 0.0f || BorderWidth >= OuterSize / 2.0f)
	{
		KalkiLog::System(
			TEXT("CreateBorderFrameMesh - Invalid parameters"),
			EKalkiLogSeverity::Error
		);
		return nullptr;
	}

	// Create new static mesh
	UStaticMesh* BorderMesh = NewObject<UStaticMesh>();
	if (!BorderMesh)
	{
		KalkiLog::System(
			TEXT("CreateBorderFrameMesh - Failed to create mesh object"),
			EKalkiLogSeverity::Error
		);
		return nullptr;
	}

	// Generate geometry
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	GenerateBorderFrameGeometry(OuterSize, BorderWidth, Height, Vertices, Triangles, Normals, UVs);

	// Create mesh section
	CreateMeshSection(BorderMesh, Vertices, Triangles, Normals, UVs);

	KalkiLog::System(
		FString::Printf(TEXT("Created border frame mesh: %.1fx%.1f, border width: %.1f"),
			OuterSize, OuterSize, BorderWidth)
	);

	return BorderMesh;
}

void UKalkiMeshGenerator::GenerateBorderFrameGeometry(
	float OuterSize,
	float BorderWidth,
	float Height,
	TArray<FVector>& OutVertices,
	TArray<int32>& OutTriangles,
	TArray<FVector>& OutNormals,
	TArray<FVector2D>& OutUVs
)
{
	// Clear output arrays
	OutVertices.Empty();
	OutTriangles.Empty();
	OutNormals.Empty();
	OutUVs.Empty();

	// Calculate dimensions
	float HalfOuter = OuterSize * 0.5f;
	float HalfInner = HalfOuter - BorderWidth;
	float HalfHeight = Height * 0.5f;

	// Center the mesh at origin
	// Outer square: from -HalfOuter to +HalfOuter
	// Inner square: from -HalfInner to +HalfInner

	/**
	 * Frame layout (top view):
	 * 
	 *  0 ---- 1
	 *  |      |
	 *  |  4--5|
	 *  |  |  ||  ← Inner hole
	 *  |  7--6|
	 *  |      |
	 *  3 ---- 2
	 * 
	 * We create 4 quads (one per side of frame)
	 */

	// Outer vertices (8 vertices - 4 corners, top and bottom)
	FVector OuterCorners[8];
	OuterCorners[0] = FVector(-HalfOuter, -HalfOuter, HalfHeight);  // Top-left, top face
	OuterCorners[1] = FVector(HalfOuter, -HalfOuter, HalfHeight);   // Top-right, top face
	OuterCorners[2] = FVector(HalfOuter, HalfOuter, HalfHeight);    // Bottom-right, top face
	OuterCorners[3] = FVector(-HalfOuter, HalfOuter, HalfHeight);   // Bottom-left, top face
	OuterCorners[4] = FVector(-HalfOuter, -HalfOuter, -HalfHeight); // Top-left, bottom face
	OuterCorners[5] = FVector(HalfOuter, -HalfOuter, -HalfHeight);  // Top-right, bottom face
	OuterCorners[6] = FVector(HalfOuter, HalfOuter, -HalfHeight);   // Bottom-right, bottom face
	OuterCorners[7] = FVector(-HalfOuter, HalfOuter, -HalfHeight);  // Bottom-left, bottom face

	// Inner vertices (8 vertices)
	FVector InnerCorners[8];
	InnerCorners[0] = FVector(-HalfInner, -HalfInner, HalfHeight);
	InnerCorners[1] = FVector(HalfInner, -HalfInner, HalfHeight);
	InnerCorners[2] = FVector(HalfInner, HalfInner, HalfHeight);
	InnerCorners[3] = FVector(-HalfInner, HalfInner, HalfHeight);
	InnerCorners[4] = FVector(-HalfInner, -HalfInner, -HalfHeight);
	InnerCorners[5] = FVector(HalfInner, -HalfInner, -HalfHeight);
	InnerCorners[6] = FVector(HalfInner, HalfInner, -HalfHeight);
	InnerCorners[7] = FVector(-HalfInner, HalfInner, -HalfHeight);

	// ========================================
	// TOP FACE (4 quads forming frame)
	// ========================================

	// Normal for top face
	FVector NormalUp = FVector(0, 0, 1);

	// Top border (between outer top and inner top)
	int32 BaseIdx = OutVertices.Num();
	OutVertices.Add(OuterCorners[0]); // 0
	OutVertices.Add(OuterCorners[1]); // 1
	OutVertices.Add(InnerCorners[1]); // 2
	OutVertices.Add(InnerCorners[0]); // 3
	
	OutNormals.Add(NormalUp);
	OutNormals.Add(NormalUp);
	OutNormals.Add(NormalUp);
	OutNormals.Add(NormalUp);

	OutUVs.Add(FVector2D(0, 0));
	OutUVs.Add(FVector2D(1, 0));
	OutUVs.Add(FVector2D(1, 1));
	OutUVs.Add(FVector2D(0, 1));

	// Triangles (2 triangles per quad)
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// Right border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(OuterCorners[1]);
	OutVertices.Add(OuterCorners[2]);
	OutVertices.Add(InnerCorners[2]);
	OutVertices.Add(InnerCorners[1]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalUp);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// Bottom border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(OuterCorners[2]);
	OutVertices.Add(OuterCorners[3]);
	OutVertices.Add(InnerCorners[3]);
	OutVertices.Add(InnerCorners[2]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalUp);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// Left border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(OuterCorners[3]);
	OutVertices.Add(OuterCorners[0]);
	OutVertices.Add(InnerCorners[0]);
	OutVertices.Add(InnerCorners[3]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalUp);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// ========================================
	// BOTTOM FACE (mirror of top, reversed winding)
	// ========================================

	FVector NormalDown = FVector(0, 0, -1);

	// Bottom - Top border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(InnerCorners[4]);
	OutVertices.Add(InnerCorners[5]);
	OutVertices.Add(OuterCorners[5]);
	OutVertices.Add(OuterCorners[4]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalDown);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// Bottom - Right border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(InnerCorners[5]);
	OutVertices.Add(InnerCorners[6]);
	OutVertices.Add(OuterCorners[6]);
	OutVertices.Add(OuterCorners[5]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalDown);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// Bottom - Bottom border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(InnerCorners[6]);
	OutVertices.Add(InnerCorners[7]);
	OutVertices.Add(OuterCorners[7]);
	OutVertices.Add(OuterCorners[6]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalDown);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);

	// Bottom - Left border
	BaseIdx = OutVertices.Num();
	OutVertices.Add(InnerCorners[7]);
	OutVertices.Add(InnerCorners[4]);
	OutVertices.Add(OuterCorners[4]);
	OutVertices.Add(OuterCorners[7]);

	for (int32 i = 0; i < 4; ++i)
	{
		OutNormals.Add(NormalDown);
		OutUVs.Add(FVector2D(i % 2, i / 2));
	}

	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 1);
	OutTriangles.Add(BaseIdx + 2);
	
	OutTriangles.Add(BaseIdx + 0);
	OutTriangles.Add(BaseIdx + 2);
	OutTriangles.Add(BaseIdx + 3);
}

void UKalkiMeshGenerator::CreateMeshSection(
	UStaticMesh* Mesh,
	const TArray<FVector>& Vertices,
	const TArray<int32>& Triangles,
	const TArray<FVector>& Normals,
	const TArray<FVector2D>& UVs
)
{
	if (!Mesh || Vertices.Num() == 0 || Triangles.Num() == 0)
	{
		return;
	}

	// Create mesh description
	FMeshDescription MeshDescription;
	FStaticMeshAttributes Attributes(MeshDescription);
	Attributes.Register();

	// Create builder
	FMeshDescriptionBuilder Builder;
	Builder.SetMeshDescription(&MeshDescription);

	// Reserve space for vertices
	Builder.ReserveNewVertices(Vertices.Num());

	// Add vertices
	TArray<FVertexID> VertexIDs;
	VertexIDs.Reserve(Vertices.Num());
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		FVertexID VertexID = Builder.AppendVertex(Vertices[i]);
		VertexIDs.Add(VertexID);
	}

	// Add triangles
	TArray<FVertexInstanceID> VertexInstanceIDs;
	VertexInstanceIDs.SetNum(3);

	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		// Create vertex instances for this triangle
		for (int32 j = 0; j < 3; ++j)
		{
			int32 VertexIndex = Triangles[i + j];
			FVertexInstanceID InstanceID = Builder.AppendInstance(VertexIDs[VertexIndex]);
			VertexInstanceIDs[j] = InstanceID;

			// Set normal
			if (Normals.IsValidIndex(VertexIndex))
			{
				Builder.SetInstanceNormal(InstanceID, Normals[VertexIndex]);
			}

			// Set UV
			if (UVs.IsValidIndex(VertexIndex))
			{
				Builder.SetInstanceUV(InstanceID, UVs[VertexIndex], 0);
			}
		}

		// Append triangle
		Builder.AppendTriangle(
			VertexInstanceIDs[0],
			VertexInstanceIDs[1],
			VertexInstanceIDs[2],
			0 // Polygon group (material slot)
		);
	}

	// Build mesh
	UStaticMesh::FBuildMeshDescriptionsParams Params;
	Params.bMarkPackageDirty = false;
	Params.bBuildSimpleCollision = false;

	TArray<const FMeshDescription*> MeshDescriptions;
	MeshDescriptions.Add(&MeshDescription);

	Mesh->BuildFromMeshDescriptions(MeshDescriptions, Params);
}