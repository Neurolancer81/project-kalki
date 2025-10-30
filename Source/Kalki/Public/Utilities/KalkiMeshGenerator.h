// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KalkiMeshGenerator.generated.h"

/**
 * Kalki Mesh Generator
 * Utility class for procedurally generating meshes at runtime
 * 
 * USAGE:
 * UStaticMesh* BorderMesh = UKalkiMeshGenerator::CreateBorderFrameMesh(100.0f, 3.0f);
 */
UCLASS()
class KALKI_API UKalkiMeshGenerator : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Create a hollow square border frame mesh
	 * 
	 * @param OuterSize - Outer dimensions of frame (tile size, e.g., 100.0f)
	 * @param BorderWidth - Thickness of border (e.g., 3.0f for medium)
	 * @param Height - Z-height of mesh (very thin, e.g., 0.1f)
	 * @return Generated static mesh (or nullptr on failure)
	 * 
	 * EXAMPLE:
	 * // Create 100x100 tile with 3-unit thick border
	 * UStaticMesh* Frame = UKalkiMeshGenerator::CreateBorderFrameMesh(100.0f, 3.0f, 0.1f);
	 * 
	 * VISUAL:
	 * +------------------------+
	 * |  (BorderWidth thick)   |
	 * |   +----------------+   |
	 * |   |   (empty)      |   |  ← Center is empty (shows tile underneath)
	 * |   |                |   |
	 * |   +----------------+   |
	 * +------------------------+
	 */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Mesh")
	static UStaticMesh* CreateBorderFrameMesh(
		float OuterSize = 100.0f,
		float BorderWidth = 3.0f,
		float Height = 0.1f
	);

private:
	/**
	 * Helper: Create mesh section from vertices and triangles
	 * 
	 * @param Mesh - Target mesh
	 * @param Vertices - Vertex positions
	 * @param Triangles - Triangle indices (3 per triangle)
	 * @param Normals - Vertex normals
	 * @param UVs - Texture coordinates
	 */
	static void CreateMeshSection(
		UStaticMesh* Mesh,
		const TArray<FVector>& Vertices,
		const TArray<int32>& Triangles,
		const TArray<FVector>& Normals,
		const TArray<FVector2D>& UVs
	);

	/**
	 * Helper: Generate vertices for border frame
	 * 
	 * @param OuterSize - Outer dimensions
	 * @param BorderWidth - Border thickness
	 * @param Height - Z-height
	 * @param OutVertices - Output vertex array
	 * @param OutTriangles - Output triangle array
	 * @param OutNormals - Output normal array
	 * @param OutUVs - Output UV array
	 */
	static void GenerateBorderFrameGeometry(
		float OuterSize,
		float BorderWidth,
		float Height,
		TArray<FVector>& OutVertices,
		TArray<int32>& OutTriangles,
		TArray<FVector>& OutNormals,
		TArray<FVector2D>& OutUVs
	);
};