// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridManager.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "KalkiGridManager.generated.h"

class AKalkiGridTile;

/**
 * Manages the tactical grid for the battlefield
 * Tracks tiles, provides queries, and handles coordinate conversion
 * Does NOT spawn tiles - that's the map generator's job
 */
UCLASS()
class KALKI_API UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Subsystem lifecycle
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Tile registration (called by tiles themselves)
    void RegisterTile(AKalkiGridTile* Tile);
    void UnregisterTile(AKalkiGridTile* Tile);

    // Grid queries
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    AKalkiGridTile* GetTileAt(const FIntPoint& GridCoords) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    AKalkiGridTile* GetTileAtWorldLocation(const FVector& WorldLocation) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsValidGridPosition(const FIntPoint& GridCoords) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsWalkable(const FIntPoint& GridCoords) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool CanBeOccupied(const FIntPoint& GridCoords) const;

    // Coordinate conversion
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    FIntPoint WorldToGrid(const FVector& WorldLocation) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    FVector GridToWorld(const FIntPoint& GridCoords) const;

    // Grid properties
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    FVector GetGridOrigin() const { return GridOrigin; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetGridOrigin(const FVector& Origin) { GridOrigin = Origin; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    float GetTileSize() const { return TileSize; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetTileSize(float Size) { TileSize = FMath::Max(1.0f, Size); }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    int32 GetTileCount() const { return GridTiles.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    TArray<AKalkiGridTile*> GetAllTiles() const;

    // Debug visualization
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetGridVisualizationEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsGridVisualizationEnabled() const { return bShowGridVisualization; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ToggleGridVisualization();

    // Debug helper - spawns a simple flat grid for testing
    // This is temporary - real levels use proper map generation
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Debug")
    void SpawnDebugGrid(FVector Origin, int32 SizeX, int32 SizeY, TSubclassOf<AKalkiGridTile> TileClass);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Debug")
    void ClearAllTiles();

protected:
    // Tick for debug visualization
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;

    // Draw debug lines for grid
    void DrawDebugGrid();

    // Grid data - map from grid position to tile
    UPROPERTY()
    TMap<FIntPoint, TObjectPtr<AKalkiGridTile>> GridTiles;

    // Grid configuration
    UPROPERTY()
    FVector GridOrigin = FVector::ZeroVector;

    UPROPERTY()
    float TileSize = 100.0f; // 5 feet in Unreal units

    // Visualization
    UPROPERTY()
    bool bShowGridVisualization = false;

    UPROPERTY()
    FTimerHandle VisualizationTimerHandle;
};