// Copyright of V.S. Puranam and no one else

#include "Grid/KalkiGridVisualizer.h"
#include "Grid/KalkiGridManager.h"
#include "Utilities/KalkiMeshGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/KalkiLog.h"

AKalkiGridVisualizer::AKalkiGridVisualizer()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create base tile component
	TileInstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TileInstancedMeshComponent"));
	RootComponent = TileInstancedMeshComponent;

	// Configure for custom data (RGBA color per instance)
	TileInstancedMeshComponent->NumCustomDataFloats = 4;

	// Create hover overlay component
	HoverOverlayComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("HoverOverlayComponent"));
	HoverOverlayComponent->SetupAttachment(RootComponent);
	HoverOverlayComponent->NumCustomDataFloats = 4;

	// Border tier components will be created dynamically as needed
}

void AKalkiGridVisualizer::BeginPlay()
{
	Super::BeginPlay();

	CacheReferences();
	InitializeMeshComponents();

	KalkiLog::System(TEXT("GridVisualizer initialized"));
}

void AKalkiGridVisualizer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unbind from grid events
	if (GridManager)
	{
		GridManager->OnGridCreated.RemoveDynamic(this, &AKalkiGridVisualizer::OnGridCreated);
		GridManager->OnGridCleared.RemoveDynamic(this, &AKalkiGridVisualizer::OnGridCleared);
		GridManager->OnTileChanged.RemoveDynamic(this, &AKalkiGridVisualizer::OnTileChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void AKalkiGridVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update hover effect every frame (automatic hover detection)
	UpdateHoverEffect();
}

// ========================================
// INITIALIZATION
// ========================================

void AKalkiGridVisualizer::CacheReferences()
{
	// Get grid manager
	GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
	if (!GridManager)
	{
		KalkiLog::System(
			TEXT("GridVisualizer - GridManager not found!"),
			EKalkiLogSeverity::Warning,
			this
		);
		return;
	}

	// Bind to grid events
	GridManager->OnGridCreated.AddDynamic(this, &AKalkiGridVisualizer::OnGridCreated);
	GridManager->OnGridCleared.AddDynamic(this, &AKalkiGridVisualizer::OnGridCleared);
	GridManager->OnTileChanged.AddDynamic(this, &AKalkiGridVisualizer::OnTileChanged);

	KalkiLog::System(TEXT("GridVisualizer - Bound to GridManager events"));
}

void AKalkiGridVisualizer::InitializeMeshComponents()
{
	// Set base tile mesh
	if (TileMesh && TileInstancedMeshComponent)
	{
		TileInstancedMeshComponent->SetStaticMesh(TileMesh);
	}

	// Set base tile material
	if (TileMaterial && TileInstancedMeshComponent)
	{
		TileInstancedMeshComponent->SetMaterial(0, TileMaterial);
	}

	// Generate border frame mesh if not provided
	if (!BorderFrameMesh)
	{
		GenerateBorderFrameMesh();
	}

	// Set hover overlay mesh (uses tile mesh for now)
	if (TileMesh && HoverOverlayComponent)
	{
		HoverOverlayComponent->SetStaticMesh(TileMesh);
	}

	// Set hover material
	if (HoverMaterial && HoverOverlayComponent)
	{
		HoverOverlayComponent->SetMaterial(0, HoverMaterial);
	}
	else if (TileMaterial && HoverOverlayComponent)
	{
		// Fallback to tile material
		HoverOverlayComponent->SetMaterial(0, TileMaterial);
	}

	KalkiLog::System(TEXT("GridVisualizer - Mesh components initialized"));
}

void AKalkiGridVisualizer::GenerateBorderFrameMesh()
{
	if (!GridManager)
	{
		return;
	}

	// Get tile size from grid manager
	float TileSize = GridManager->GetTileSize();
	float BorderWidth = 3.0f; // Medium thickness

	// Generate procedural border frame
	BorderFrameMesh = UKalkiMeshGenerator::CreateBorderFrameMesh(TileSize, BorderWidth, 0.1f);

	if (BorderFrameMesh)
	{
		KalkiLog::System(TEXT("GridVisualizer - Generated border frame mesh procedurally"));
	}
	else
	{
		KalkiLog::System(
			TEXT("GridVisualizer - Failed to generate border frame mesh"),
			EKalkiLogSeverity::Error,
			this
		);
	}
}

// ========================================
// GRID CREATION/UPDATES
// ========================================

void AKalkiGridVisualizer::OnGridCreated()
{
	KalkiLog::System(TEXT("GridVisualizer - OnGridCreated event received"));
	CreateGridVisuals();
}

void AKalkiGridVisualizer::OnGridCleared()
{
	KalkiLog::System(TEXT("GridVisualizer - OnGridCleared event received"));
	ClearGridVisuals();
}

void AKalkiGridVisualizer::OnTileChanged(const FKalkiGridCoord& Coord)
{
	KalkiLog::Grid(
		FString::Printf(TEXT("GridVisualizer - Tile changed: %s"), *Coord.ToString()),
		EKalkiLogSeverity::Verbose
	);
	UpdateTileVisual(Coord);
}

void AKalkiGridVisualizer::CreateGridVisuals()
{
	if (!GridManager || !TileInstancedMeshComponent)
	{
		KalkiLog::System(
			TEXT("GridVisualizer - Cannot create visuals (no GridManager or TileComponent)"),
			EKalkiLogSeverity::Error,
			this
		);
		return;
	}

	// Clear existing instances
	TileInstancedMeshComponent->ClearInstances();

	// Get grid dimensions
	int32 GridSizeX = GridManager->GetGridSizeX();
	int32 GridSizeY = GridManager->GetGridSizeY();

	KalkiLog::System(
		FString::Printf(TEXT("GridVisualizer - Creating visuals for %dx%d grid"), GridSizeX, GridSizeY)
	);

	// Create instance for each tile
	int32 InstanceCount = 0;
	for (int32 Y = 0; Y < GridSizeY; ++Y)
	{
		for (int32 X = 0; X < GridSizeX; ++X)
		{
			FKalkiGridCoord Coord(X, Y);

			// Get tile data
			FKalkiGridTile Tile = GridManager->GetTile(Coord);

			// Calculate transform
			FVector Location = Tile.WorldPosition;
			FRotator Rotation = FRotator::ZeroRotator;
			FVector Scale = FVector(1.0f, 1.0f, 1.0f);
			FTransform Transform(Rotation, Location, Scale);

			// Add instance
			int32 InstanceIndex = TileInstancedMeshComponent->AddInstance(Transform);

			// Set color based on terrain cost
			FLinearColor TileColor = GetTerrainColorForTile(Coord);
			SetTileInstanceColor(InstanceIndex, TileColor);

			InstanceCount++;
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("GridVisualizer - Created %d tile instances"), InstanceCount)
	);
}

void AKalkiGridVisualizer::ClearGridVisuals()
{
	if (TileInstancedMeshComponent)
	{
		TileInstancedMeshComponent->ClearInstances();
	}

	// Clear all border tier components
	for (UInstancedStaticMeshComponent* BorderComp : BorderTierComponents)
	{
		if (BorderComp)
		{
			BorderComp->ClearInstances();
		}
	}

	// Clear hover
	if (HoverOverlayComponent)
	{
		HoverOverlayComponent->ClearInstances();
	}

	bHasSelection = false;
	bHasHover = false;
	ActiveMovementTiers.Empty();

	KalkiLog::System(TEXT("GridVisualizer - Cleared all visuals"));
}

void AKalkiGridVisualizer::UpdateTileVisual(const FKalkiGridCoord& Coord)
{
	if (!GridManager || !GridManager->IsValidCoord(Coord))
	{
		return;
	}

	// Calculate instance index (row-major order)
	int32 GridSizeX = GridManager->GetGridSizeX();
	int32 InstanceIndex = Coord.Y * GridSizeX + Coord.X;

	// Update color
	FLinearColor TileColor = GetTerrainColorForTile(Coord);
	SetTileInstanceColor(InstanceIndex, TileColor);
}

void AKalkiGridVisualizer::RefreshAllVisuals()
{
	if (!GridManager)
	{
		return;
	}

	int32 GridSizeX = GridManager->GetGridSizeX();
	int32 GridSizeY = GridManager->GetGridSizeY();

	for (int32 Y = 0; Y < GridSizeY; ++Y)
	{
		for (int32 X = 0; X < GridSizeX; ++X)
		{
			UpdateTileVisual(FKalkiGridCoord(X, Y));
		}
	}

	KalkiLog::System(TEXT("GridVisualizer - Refreshed all tile visuals"));
}

// ========================================
// TERRAIN COLORS
// ========================================

FLinearColor AKalkiGridVisualizer::GetTerrainColorForTile(const FKalkiGridCoord& Coord) const
{
	if (!GridManager || !GridManager->IsValidCoord(Coord))
	{
		return FLinearColor::White;
	}

	FKalkiGridTile Tile = GridManager->GetTile(Coord);

	// Determine color based on movement cost
	if (Tile.MovementCost <= 1.0f)
	{
		// Normal walkable
		return NormalTerrainColor;
	}
	else if (Tile.MovementCost <= 2.0f)
	{
		// Difficult terrain
		return DifficultTerrainColor;
	}
	else if (Tile.MovementCost <= 3.0f)
	{
		// Very difficult terrain
		return VeryDifficultTerrainColor;
	}
	else
	{
		// Impassable or extremely difficult
		return ImpassableTerrainColor;
	}
}

void AKalkiGridVisualizer::SetTileInstanceColor(int32 InstanceIndex, const FLinearColor& Color)
{
	if (!TileInstancedMeshComponent)
	{
		return;
	}

	// Set custom data (RGBA)
	TileInstancedMeshComponent->SetCustomDataValue(InstanceIndex, 0, Color.R, true);
	TileInstancedMeshComponent->SetCustomDataValue(InstanceIndex, 1, Color.G, true);
	TileInstancedMeshComponent->SetCustomDataValue(InstanceIndex, 2, Color.B, true);
	TileInstancedMeshComponent->SetCustomDataValue(InstanceIndex, 3, Color.A, true);
}

// ========================================
// TILE SELECTION (Used by GridInteractionComponent)
// ========================================

void AKalkiGridVisualizer::SelectTile(const FKalkiGridCoord& Coord)
{
	if (!GridManager || !GridManager->IsValidCoord(Coord))
	{
		KalkiLog::Grid(
			FString::Printf(TEXT("Cannot select invalid tile: %s"), *Coord.ToString()),
			EKalkiLogSeverity::Warning
		);
		return;
	}

	// Update selection state
	SelectedTile = Coord;
	bHasSelection = true;

	KalkiLog::Grid(
		FString::Printf(TEXT("Tile selected: %s"), *Coord.ToString())
	);
}

void AKalkiGridVisualizer::ClearSelection()
{
	if (!bHasSelection)
	{
		return;
	}

	bHasSelection = false;
	SelectedTile = FKalkiGridCoord::Invalid();

	KalkiLog::Grid(TEXT("Tile selection cleared"), EKalkiLogSeverity::Verbose);
}

// ========================================
// MOVEMENT RANGE - Multi-Tier System
// ========================================

void AKalkiGridVisualizer::ShowMovementRange(const FKalkiGridCoord& Origin, const TArray<FKalkiMovementTier>& Tiers)
{
	if (!GridManager || !GridManager->IsValidCoord(Origin))
	{
		KalkiLog::System(
			TEXT("ShowMovementRange - Invalid origin coordinate"),
			EKalkiLogSeverity::Warning,
			this
		);
		return;
	}

	if (Tiers.Num() == 0)
	{
		KalkiLog::System(
			TEXT("ShowMovementRange - No tiers provided"),
			EKalkiLogSeverity::Warning,
			this
		);
		return;
	}

	// Clear existing movement range
	HideMovementRange();

	// Store active tiers
	ActiveMovementTiers = Tiers;

	// Sort tiers by range (smallest to largest) for proper layering
	ActiveMovementTiers.Sort();

	// Ensure we have enough border components
	while (BorderTierComponents.Num() < ActiveMovementTiers.Num())
	{
		// Create new border component
		FString CompName = FString::Printf(TEXT("BorderTier%d"), BorderTierComponents.Num());
		UInstancedStaticMeshComponent* NewBorderComp = NewObject<UInstancedStaticMeshComponent>(this, *CompName);
		
		if (NewBorderComp)
		{
			NewBorderComp->SetupAttachment(RootComponent);
			NewBorderComp->RegisterComponent();
			NewBorderComp->SetStaticMesh(BorderFrameMesh);
			
			if (BorderMaterial)
			{
				NewBorderComp->SetMaterial(0, BorderMaterial);
			}
			
			NewBorderComp->NumCustomDataFloats = 4;
			BorderTierComponents.Add(NewBorderComp);
		}
	}

	// For each tier, create border instances
	for (int32 TierIndex = 0; TierIndex < ActiveMovementTiers.Num(); ++TierIndex)
	{
		const FKalkiMovementTier& Tier = ActiveMovementTiers[TierIndex];
		UInstancedStaticMeshComponent* BorderComp = BorderTierComponents[TierIndex];

		if (!BorderComp)
		{
			continue;
		}

		// Clear previous instances
		BorderComp->ClearInstances();

		// Get tiles in range for this tier
		TArray<FKalkiGridCoord> TilesInRange = GridManager->GetTilesInRange(
			Origin,
			Tier.Range,
			true // Only walkable
		);

		KalkiLog::System(
			FString::Printf(TEXT("ShowMovementRange - Tier '%s': %d tiles in range %d"),
				*Tier.TierName,
				TilesInRange.Num(),
				Tier.Range),
			EKalkiLogSeverity::Verbose
		);

		// Create border instance for each tile
		for (const FKalkiGridCoord& TileCoord : TilesInRange)
		{
			FKalkiGridTile Tile = GridManager->GetTile(TileCoord);

			// Calculate transform with Z-offset
			FVector Location = Tile.WorldPosition;
			Location.Z += Tier.ZOffset; // Layer borders at different heights

			FRotator Rotation = FRotator::ZeroRotator;
			FVector Scale = FVector(1.0f, 1.0f, 1.0f);
			FTransform Transform(Rotation, Location, Scale);

			// Add border instance
			int32 InstanceIndex = BorderComp->AddInstance(Transform);

			// Set border color via custom data
			BorderComp->SetCustomDataValue(InstanceIndex, 0, Tier.BorderColor.R, true);
			BorderComp->SetCustomDataValue(InstanceIndex, 1, Tier.BorderColor.G, true);
			BorderComp->SetCustomDataValue(InstanceIndex, 2, Tier.BorderColor.B, true);
			BorderComp->SetCustomDataValue(InstanceIndex, 3, Tier.BorderColor.A, true);
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("ShowMovementRange - Displayed %d movement tiers from %s"),
			ActiveMovementTiers.Num(),
			*Origin.ToString())
	);
}

void AKalkiGridVisualizer::HideMovementRange()
{
	// Clear all border tier components
	for (UInstancedStaticMeshComponent* BorderComp : BorderTierComponents)
	{
		if (BorderComp)
		{
			BorderComp->ClearInstances();
		}
	}

	ActiveMovementTiers.Empty();

	KalkiLog::System(TEXT("HideMovementRange - Cleared all movement borders"), EKalkiLogSeverity::Verbose);
}

// ========================================
// HOVER EFFECT (Automatic)
// ========================================

void AKalkiGridVisualizer::UpdateHoverEffect()
{
	if (!GridManager || !HoverOverlayComponent)
	{
		return;
	}

	// Get tile under cursor
	bool bSuccess = false;
	FKalkiGridCoord CursorTile = GetTileUnderCursor(bSuccess);

	if (bSuccess && GridManager->IsValidCoord(CursorTile))
	{
		// Check if hover changed
		if (!bHasHover || HoveredTile != CursorTile)
		{
			// Clear old hover
			ClearHoverEffect();

			// Set new hover
			HoveredTile = CursorTile;
			bHasHover = true;

			// Get tile data
			FKalkiGridTile Tile = GridManager->GetTile(HoveredTile);

			// Calculate transform (Z+10 for hover layer)
			FVector Location = Tile.WorldPosition;
			Location.Z += 10.0f; // Above everything else

			FRotator Rotation = FRotator::ZeroRotator;
			FVector Scale = FVector(1.0f, 1.0f, 1.0f);
			FTransform Transform(Rotation, Location, Scale);

			// Add hover instance
			int32 InstanceIndex = HoverOverlayComponent->AddInstance(Transform);

			// Set hover color (white outline)
			HoverOverlayComponent->SetCustomDataValue(InstanceIndex, 0, HoverOutlineColor.R, true);
			HoverOverlayComponent->SetCustomDataValue(InstanceIndex, 1, HoverOutlineColor.G, true);
			HoverOverlayComponent->SetCustomDataValue(InstanceIndex, 2, HoverOutlineColor.B, true);
			HoverOverlayComponent->SetCustomDataValue(InstanceIndex, 3, HoverOutlineColor.A, true);
		}
	}
	else
	{
		// No valid tile under cursor - clear hover
		if (bHasHover)
		{
			ClearHoverEffect();
		}
	}
}

void AKalkiGridVisualizer::ClearHoverEffect()
{
	if (HoverOverlayComponent)
	{
		HoverOverlayComponent->ClearInstances();
	}

	bHasHover = false;
	HoveredTile = FKalkiGridCoord::Invalid();
}

FKalkiGridCoord AKalkiGridVisualizer::GetTileUnderCursor(bool& bSuccess)
{
	FVector HitLocation;
	FKalkiGridCoord Coord;

	bSuccess = RaycastToGrid(HitLocation, Coord);
	return Coord;
}

bool AKalkiGridVisualizer::RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord)
{
	if (!GridManager)
	{
		return false;
	}

	// Get player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return false;
	}

	// Get mouse ray
	FVector WorldLocation, WorldDirection;
	if (!PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		return false;
	}

	// Perform raycast
	FHitResult HitResult;
	FVector TraceEnd = WorldLocation + (WorldDirection * 50000.0f); // Max distance
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WorldLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		OutHitLocation = HitResult.Location;

		// Convert world position to grid coordinate
		OutCoord = GridManager->WorldPositionToCoord(OutHitLocation);

		// Validate coordinate
		if (GridManager->IsValidCoord(OutCoord))
		{
			return true;
		}
	}

	return false;
}

// ========================================
// GRID VISIBILITY
// ========================================

void AKalkiGridVisualizer::ShowGrid()
{
	SetShowGrid(true);
}

void AKalkiGridVisualizer::HideGrid()
{
	SetShowGrid(false);
}

void AKalkiGridVisualizer::SetShowGrid(bool bShow)
{
	if (bGridVisible == bShow)
	{
		return; // No change
	}

	bGridVisible = bShow;

	// Set visibility on all components
	if (TileInstancedMeshComponent)
	{
		TileInstancedMeshComponent->SetVisibility(bGridVisible);
	}

	for (UInstancedStaticMeshComponent* BorderComp : BorderTierComponents)
	{
		if (BorderComp)
		{
			BorderComp->SetVisibility(bGridVisible);
		}
	}

	if (HoverOverlayComponent)
	{
		HoverOverlayComponent->SetVisibility(bGridVisible);
	}

	KalkiLog::System(
		FString::Printf(TEXT("GridVisualizer - Grid visibility set to %s"),
			bGridVisible ? TEXT("visible") : TEXT("hidden"))
	);
}