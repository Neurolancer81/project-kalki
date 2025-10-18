// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridOccupancyComponent.generated.h"

// Forward declarations
class UKalkiGridManager;

/**
 * Grid Occupancy Component
 * Handles an actor's presence on the tactical grid
 * Add this to any actor that should occupy grid tiles
 * 
 * FEATURES:
 * - Grid position tracking (replicated)
 * - Automatic world position sync
 * - Tile occupancy management
 * - Multi-tile support (large creatures)
 * - Optional (can be disabled)
 * 
 * USAGE:
 * Component = Actor->FindComponentByClass<UKalkiGridOccupancyComponent>();
 * Component->PlaceOnGrid(FKalkiGridCoord(5, 7));
 */
UCLASS(ClassGroup=(Kalki), meta=(BlueprintSpawnableComponent))
class KALKI_API UKalkiGridOccupancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKalkiGridOccupancyComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// ========================================
	// GRID POSITION
	// ========================================

	/** Get current grid position */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
	FKalkiGridCoord GetGridPosition() const { return GridPosition; }

	/** Set grid position (also updates world position and occupancy) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid", meta = (Keywords = "move teleport"))
	void SetGridPosition(const FKalkiGridCoord& NewPosition);

	/** Place actor on grid at specific coordinate with validation */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
	bool PlaceOnGrid(const FKalkiGridCoord& Coord);

	/** Remove actor from grid (clears occupancy) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
	void RemoveFromGrid();

	/** Is actor currently on grid? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
	bool IsOnGrid() const { return GridPosition.IsValid(); }

	// ========================================
	// OCCUPANCY VALIDATION
	// ========================================

	/** Check if this occupant can occupy a specific tile */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
	bool CanOccupyTile(const FKalkiGridCoord& Coord) const;

	// ========================================
	// SETTINGS
	// ========================================

	/** Enable/disable grid occupancy (for exploration mode, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
	bool bCanOccupyTiles = true;

	/** Update world position when grid position changes? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
	bool bUpdateWorldPosition = true;

	/** Size in tiles (1 = normal, 2 = large, 4 = huge, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid", meta = (ClampMin = "1", ClampMax = "4"))
	int32 OccupancySize = 1;

	/** Z-offset from tile surface (for floating creatures, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
	float ZOffset = 0.0f;

protected:
	// ========================================
	// STATE
	// ========================================

	/** Current grid position (replicated) */
	UPROPERTY(ReplicatedUsing = OnRep_GridPosition, BlueprintReadOnly, Category = "Kalki|Grid")
	FKalkiGridCoord GridPosition;

	/** Grid manager reference (cached) */
	UPROPERTY()
	TObjectPtr<UKalkiGridManager> GridManager;

	// ========================================
	// INTERNAL FUNCTIONS
	// ========================================

	/** Replication callback for grid position */
	UFUNCTION()
	void OnRep_GridPosition();

	/** Update world position from grid position */
	void UpdateWorldPositionFromGrid();

	/** Cache grid manager reference */
	void CacheGridManager();

	/** Network replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};