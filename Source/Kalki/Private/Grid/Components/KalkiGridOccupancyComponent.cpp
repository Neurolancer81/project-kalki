// Copyright of V.S. Puranam and no one else

#include "Grid/Components/KalkiGridOccupancyComponent.h"
#include "Grid/KalkiGridManager.h"
#include "Grid/Components/KalkiGridOccupant.h"
#include "Logging/KalkiLog.h"
#include "Net/UnrealNetwork.h"

UKalkiGridOccupancyComponent::UKalkiGridOccupancyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// Start off-grid
	GridPosition = FKalkiGridCoord::Invalid();
}

void UKalkiGridOccupancyComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheGridManager();

	KalkiLog::Grid(
		FString::Printf(TEXT("GridOccupancyComponent initialized on: %s"), 
			*GetOwner()->GetName()),
		EKalkiLogSeverity::Verbose
	);
}

void UKalkiGridOccupancyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clean up - remove from grid
	if (IsOnGrid())
	{
		RemoveFromGrid();
	}

	Super::EndPlay(EndPlayReason);
}

// ========================================
// GRID POSITION
// ========================================

void UKalkiGridOccupancyComponent::SetGridPosition(const FKalkiGridCoord& NewPosition)
{
	if (!GridManager)
	{
		KalkiLog::Grid(
			TEXT("SetGridPosition failed - no GridManager"),
			EKalkiLogSeverity::Warning,
			GetOwner()
		);
		return;
	}

	// Validate new position
	if (!GridManager->IsValidCoord(NewPosition))
	{
		KalkiLog::Grid(
			FString::Printf(TEXT("SetGridPosition failed - invalid coord: %s"), 
				*NewPosition.ToString()),
			EKalkiLogSeverity::Warning,
			GetOwner()
		);
		return;
	}

	// Clear old position occupancy
	if (GridPosition.IsValid() && bCanOccupyTiles)
	{
		GridManager->SetTileOccupant(GridPosition, nullptr);
	}

	// Set new position
	GridPosition = NewPosition;

	// Update occupancy at new position
	if (bCanOccupyTiles)
	{
		GridManager->SetTileOccupant(GridPosition, GetOwner());
	}

	// Update world position
	if (bUpdateWorldPosition)
	{
		UpdateWorldPositionFromGrid();
	}

	KalkiLog::Grid(
		FString::Printf(TEXT("%s moved to grid position: %s"),
			*GetOwner()->GetName(),
			*GridPosition.ToString()),
		EKalkiLogSeverity::Verbose,
		GetOwner()
	);
}

bool UKalkiGridOccupancyComponent::PlaceOnGrid(const FKalkiGridCoord& Coord)
{
	if (!GridManager)
	{
		CacheGridManager();
		if (!GridManager)
		{
			KalkiLog::Grid(
				TEXT("PlaceOnGrid failed - no GridManager"),
				EKalkiLogSeverity::Error,
				GetOwner()
			);
			return false;
		}
	}

	// Validate coordinate
	if (!GridManager->IsValidCoord(Coord))
	{
		KalkiLog::Grid(
			FString::Printf(TEXT("PlaceOnGrid failed - invalid coord: %s"), 
				*Coord.ToString()),
			EKalkiLogSeverity::Warning,
			GetOwner()
		);
		return false;
	}

	// Check if can occupy (custom validation)
	if (!CanOccupyTile(Coord))
	{
		KalkiLog::Grid(
			FString::Printf(TEXT("PlaceOnGrid failed - cannot occupy tile: %s"), 
				*Coord.ToString()),
			EKalkiLogSeverity::Warning,
			GetOwner()
		);
		return false;
	}

	// Check if tile is walkable
	FKalkiGridTile Tile = GridManager->GetTile(Coord);
	if (!Tile.bWalkable)
	{
		KalkiLog::Grid(
			FString::Printf(TEXT("PlaceOnGrid failed - tile not walkable: %s"), 
				*Coord.ToString()),
			EKalkiLogSeverity::Warning,
			GetOwner()
		);
		return false;
	}

	// Check if tile is occupied (unless it's us)
	if (Tile.IsOccupied() && Tile.Occupant != GetOwner())
	{
		KalkiLog::Grid(
			FString::Printf(TEXT("PlaceOnGrid failed - tile occupied by %s: %s"), 
				*Tile.Occupant->GetName(),
				*Coord.ToString()),
			EKalkiLogSeverity::Warning,
			GetOwner()
		);
		return false;
	}

	// Place on grid
	SetGridPosition(Coord);

	KalkiLog::Grid(
		FString::Printf(TEXT("%s placed on grid at: %s"),
			*GetOwner()->GetName(),
			*Coord.ToString())
	);

	return true;
}

void UKalkiGridOccupancyComponent::RemoveFromGrid()
{
	if (!GridPosition.IsValid())
	{
		return;
	}

	if (GridManager && bCanOccupyTiles)
	{
		GridManager->SetTileOccupant(GridPosition, nullptr);
	}

	KalkiLog::Grid(
		FString::Printf(TEXT("%s removed from grid (was at: %s)"),
			*GetOwner()->GetName(),
			*GridPosition.ToString()),
		EKalkiLogSeverity::Verbose,
		GetOwner()
	);

	GridPosition = FKalkiGridCoord::Invalid();
}

// ========================================
// OCCUPANCY VALIDATION
// ========================================

bool UKalkiGridOccupancyComponent::CanOccupyTile(const FKalkiGridCoord& Coord) const
{
	if (!bCanOccupyTiles)
	{
		return false;
	}

	if (!GridManager || !GridManager->IsValidCoord(Coord))
	{
		return false;
	}

	// Get tile
	FKalkiGridTile Tile = GridManager->GetTile(Coord);

	// Must be walkable
	if (!Tile.bWalkable)
	{
		return false;
	}

	// Must not be occupied (unless it's us)
	if (Tile.IsOccupied() && Tile.Occupant != GetOwner())
	{
		return false;
	}

	// TODO: Add custom validation here
	// - Flying creatures can occupy any tile
	// - Incorporeal creatures ignore occupancy
	// - Large creatures need multiple tiles
	// etc.

	return true;
}

// ========================================
// INTERNAL FUNCTIONS
// ========================================

void UKalkiGridOccupancyComponent::OnRep_GridPosition()
{
	// Client: Update world position when grid position replicates
	if (bUpdateWorldPosition)
	{
		UpdateWorldPositionFromGrid();
	}

	KalkiLog::Grid(
		FString::Printf(TEXT("%s grid position replicated: %s"),
			*GetOwner()->GetName(),
			*GridPosition.ToString()),
		EKalkiLogSeverity::Verbose,
		GetOwner()
	);
}

void UKalkiGridOccupancyComponent::UpdateWorldPositionFromGrid()
{
	if (!GridManager || !GridPosition.IsValid() || !GetOwner())
	{
		return;
	}

	// Get world position for grid coordinate
	FVector NewWorldPos = GridManager->CoordToWorldPosition(GridPosition);

	// Apply Z offset (for floating creatures, etc.)
	NewWorldPos.Z += ZOffset;

	// Set actor location (snapped to tile center)
	GetOwner()->SetActorLocation(NewWorldPos);
}

void UKalkiGridOccupancyComponent::CacheGridManager()
{
	if (GetWorld())
	{
		GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();

		if (!GridManager)
		{
			KalkiLog::Grid(
				TEXT("Failed to get GridManager subsystem"),
				EKalkiLogSeverity::Warning,
				GetOwner()
			);
		}
	}
}

void UKalkiGridOccupancyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate grid position to all clients
	DOREPLIFETIME(UKalkiGridOccupancyComponent, GridPosition);
}