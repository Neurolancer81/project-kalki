// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridManager.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridManager.generated.h"

// Grid lifecycle events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCreated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileChanged, const FKalkiGridCoord&, Coord);

/**
 * Grid Manager Subsystem
 * Manages the tactical grid, pathfinding, and tile queries
 * 
 * MULTIPLAYER NOTES:
 * - Grid is deterministic (same on all machines)
 * - Only server has authority to modify grid
 * - Clients can query but not modify
 * - Grid built from LevelManager settings (replicated)
 */
UCLASS()
class KALKI_API UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Subsystem interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Grid creation (Server only)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void CreateGrid(int32 SizeX, int32 SizeY, float TileSize = 100.0f, const FVector& Origin = FVector::ZeroVector);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ClearGrid();

    // Grid queries (Anyone can query)
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool IsValidCoord(const FKalkiGridCoord& Coord) const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    FKalkiGridTile GetTile(const FKalkiGridCoord& Coord) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool SetTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile);

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool IsTileWalkable(const FKalkiGridCoord& Coord) const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool IsTileOccupied(const FKalkiGridCoord& Coord) const;

    // Coordinate conversion (Pure, no authority needed)
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    FVector CoordToWorldPosition(const FKalkiGridCoord& Coord) const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    FKalkiGridCoord WorldPositionToCoord(const FVector& WorldPos) const;

    /** Set tile occupant (pass nullptr to clear) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool SetTileOccupant(const FKalkiGridCoord& Coord, AActor* Occupant);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool ClearTileOccupant(const FKalkiGridCoord& Coord);

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    AActor* GetTileOccupant(const FKalkiGridCoord& Coord) const;

    // Elevation (Server only for modification)
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    float GetElevation(const FKalkiGridCoord& Coord) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool SetElevation(const FKalkiGridCoord& Coord, float Elevation);

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    float GetElevationDifference(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool IsClimbable(const FKalkiGridCoord& From, const FKalkiGridCoord& To, float MaxClimbHeight = 200.0f) const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool HasLineOfSight(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const;

    // Neighbors (Pure queries)
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    TArray<FKalkiGridCoord> GetNeighbors(const FKalkiGridCoord& Coord, bool bDiagonalAllowed = true) const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    TArray<FKalkiGridCoord> GetWalkableNeighbors(const FKalkiGridCoord& Coord, bool bDiagonalAllowed = true) const;

    // Pathfinding (Pure queries)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    FKalkiGridPath FindPath(const FKalkiGridCoord& Start, const FKalkiGridCoord& End, bool bDiagonalAllowed = true);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    TArray<FKalkiGridCoord> GetTilesInRange(const FKalkiGridCoord& Center, int32 Range, bool bRequireWalkable = false) const;

    // Grid info (Pure queries)
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    int32 GetGridSizeX() const { return GridSizeX; }

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    int32 GetGridSizeY() const { return GridSizeY; }

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    float GetTileSize() const { return TileSize; }

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    FVector GetGridOrigin() const { return GridOrigin; }

    // Authority check
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool HasGridAuthority() const;

    /** Event fired when grid is created and ready */
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid|Events")
    FOnGridCreated OnGridCreated;

    /** Event fired when grid is cleared */
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid|Events")
    FOnGridCleared OnGridCleared;

    /** Event fired when a tile is modified (elevation, walkability, etc.) */
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid|Events")
    FOnTileChanged OnTileChanged;

    // Get all tiles within a certain range (Euclidean distance)
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    TSet<FKalkiGridCoord> GetTilesInRangeEuclidean(const FKalkiGridCoord& Origin, float Range) const;

protected:
    // Grid storage
    UPROPERTY()
    TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;

    // Grid parameters
    UPROPERTY()
    int32 GridSizeX = 0;

    UPROPERTY()
    int32 GridSizeY = 0;

    UPROPERTY()
    float TileSize = 100.0f;

    UPROPERTY()
    FVector GridOrigin = FVector::ZeroVector;

    // Helper functions
    FKalkiGridTile* GetTileMutable(const FKalkiGridCoord& Coord);
    float GetMovementCost(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const;
};