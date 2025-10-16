// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridVisualizer.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/KalkiGridTypes.h"
#include "UI/Grid/KalkiGridInfoViewModel.h"
#include "UI/Grid/KalkiGridInfoWidget.h"
#include "KalkiGridVisualizer.generated.h"

class UKalkiGridManager;
class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UUserWidget;

/**
 * Grid Visualizer
 * Renders the tactical grid using instanced meshes
 * Shows walkability, elevation, and movement ranges visually
 * Handles hover detection and tile selection
 * 
 * PERFORMANCE:
 * - Uses GPU instancing (1-2 draw calls for entire grid)
 * - Color-coded via material parameters
 * - Local only (no replication needed)
 */
UCLASS()
class KALKI_API AKalkiGridVisualizer : public AActor
{
    GENERATED_BODY()

public:
    AKalkiGridVisualizer();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // ========================================
    // COMPONENTS
    // ========================================

    // Instanced mesh component (renders all tiles efficiently)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    TObjectPtr<UInstancedStaticMeshComponent> TileInstances;

    // ========================================
    // MESH & MATERIAL
    // ========================================

    // Base tile mesh (your 1x1 mesh)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid|Mesh")
    TObjectPtr<UStaticMesh> TileMesh;

    // Material for tiles (color-coded)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid|Material")
    TObjectPtr<UMaterialInterface> TileMaterial;

    // Scale factor for tiles (0.9 = 10% gap, 0.85 = 15% gap)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Mesh", meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float TileScale = 0.9f;

    // Dynamic material instance (for runtime color changes)
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynamicTileMaterial;

    // ========================================
    // GRID REFERENCE
    // ========================================

    // Grid manager reference
    UPROPERTY()
    TObjectPtr<UKalkiGridManager> GridManager;

    // ========================================
    // VISUALIZATION SETTINGS
    // ========================================

    UPROPERTY(EditAnywhere, Category = "Grid|Settings")
    bool bShowGrid = true;

    UPROPERTY(EditAnywhere, Category = "Grid|Settings")
    bool bShowElevation = true;

    UPROPERTY(EditAnywhere, Category = "Grid|Settings")
    bool bShowWalkability = true;

    UPROPERTY(EditAnywhere, Category = "Grid|Settings")
    float TileZOffset = 1.0f; // Offset above ground to prevent z-fighting

    // ========================================
    // COLORS
    // ========================================

    // Base tile colors
    UPROPERTY(EditAnywhere, Category = "Grid|Colors")
    FLinearColor WalkableColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.3f); // Green

    UPROPERTY(EditAnywhere, Category = "Grid|Colors")
    FLinearColor UnwalkableColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.3f); // Red

    UPROPERTY(EditAnywhere, Category = "Grid|Colors")
    FLinearColor OccupiedColor = FLinearColor(1.0f, 0.5f, 0.0f, 0.3f); // Orange

    UPROPERTY(EditAnywhere, Category = "Grid|Colors")
    FLinearColor BaseElevationColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray

    UPROPERTY(EditAnywhere, Category = "Grid|Colors")
    FLinearColor HighElevationColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // White

    UPROPERTY(EditAnywhere, Category = "Grid|Colors")
    float MaxElevationForColor = 500.0f; // Elevation at which color is fully white

    // Hover and selection colors
    UPROPERTY(EditAnywhere, Category = "Grid|Colors|Hover")
    FLinearColor HoverColor = FLinearColor(1.0f, 1.0f, 0.0f, 0.5f); // Yellow

    UPROPERTY(EditAnywhere, Category = "Grid|Colors|Selection")
    FLinearColor SelectedColor = FLinearColor(0.0f, 1.0f, 1.0f, 0.5f); // Cyan

    UPROPERTY(EditAnywhere, Category = "Grid|Colors|Movement Range")
    FLinearColor MovementRangeColor = FLinearColor(0.0f, 0.5f, 1.0f, 0.4f); // Blue

    // ========================================
    // HOVER DETECTION
    // ========================================

    UPROPERTY(EditAnywhere, Category = "Grid|Hover")
    bool bEnableHover = true;

    UPROPERTY(EditAnywhere, Category = "Grid|Hover")
    float HoverRaycastDistance = 10000.0f;

    // Hover state
    FKalkiGridCoord HoveredTile = FKalkiGridCoord(-1, -1);
    FKalkiGridCoord PreviousHoveredTile = FKalkiGridCoord(-1, -1);

    // ========================================
    // SELECTION
    // ========================================

    // Selected tile (for movement preview)
    FKalkiGridCoord SelectedTile = FKalkiGridCoord(-1, -1);
    bool bHasTileSelected = false;

    // ========================================
    // MOVEMENT RANGE
    // ========================================

    // Movement range preview
    TSet<FKalkiGridCoord> MovementRangeTiles;

    UPROPERTY(EditAnywhere, Category = "Grid|Movement Range")
    int32 PreviewMovementRange = 5; // Default range for testing

    // ========================================
    // INFO WIDGET
    // ========================================

    UPROPERTY(EditAnywhere, Category = "Grid|UI")
    TSubclassOf<UKalkiGridInfoWidget> GridInfoWidgetClass;

    UPROPERTY()
    TObjectPtr<UKalkiGridInfoWidget> GridInfoWidget;

    UPROPERTY()
    TObjectPtr<UKalkiGridInfoViewModel> GridInfoViewModel;

    // ========================================
    // GRID STATE
    // ========================================

    bool bGridInitialized = false;

    // ========================================
    // INITIALIZATION
    // ========================================

    // Initialize the grid visualization
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void InitializeGrid();

    // ========================================
    // TILE UPDATES
    // ========================================

    // Update all tile visuals
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void RefreshGrid();

    // Update a single tile's visual
    void UpdateTileVisual(const FKalkiGridCoord& Coord);

    // Get color for a tile based on its state
    FLinearColor GetTileColor(const FKalkiGridTile& Tile) const;

    // ========================================
    // HOVER SYSTEM
    // ========================================

    // Update hover state (called every tick)
    void UpdateHover(float DeltaTime);

    // Get tile under mouse cursor
    bool GetTileUnderCursor(FKalkiGridCoord& OutCoord) const;

    // ========================================
    // HIGHLIGHT SYSTEM
    // ========================================

    // Set highlight color on a tile
    void SetTileHighlight(const FKalkiGridCoord& Coord, const FLinearColor& Color);

    // Clear highlight from a tile (restore original color)
    void ClearTileHighlight(const FKalkiGridCoord& Coord);

    // Clear all highlights
    void ClearAllHighlights();

    // ========================================
    // EVENT HANDLERS
    // ========================================

    // Event handler for tile changes
    UFUNCTION()
    void OnTileChanged(const FKalkiGridCoord& Coord);

    // ========================================
    // VISIBILITY
    // ========================================

    // Show/hide the grid
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetGridVisible(bool bVisible);

public:
    // ========================================
    // PUBLIC API
    // ========================================

    // External controls (for combat mode, etc.)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ShowGrid() { SetGridVisible(true); }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void HideGrid() { SetGridVisible(false); }

    // Selection
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SelectTile(const FKalkiGridCoord& Coord);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void DeselectTile();

    // Movement range preview
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ShowMovementRange(const FKalkiGridCoord& Origin, int32 Range);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void HideMovementRange();

    // Getters
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    FKalkiGridCoord GetHoveredTile() const { return HoveredTile; }

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool IsHoveringTile() const { return HoveredTile.X >= 0 && HoveredTile.Y >= 0; }

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    FKalkiGridCoord GetSelectedTile() const { return SelectedTile; }

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool HasTileSelected() const { return bHasTileSelected; }

protected:
    // Update widget position to follow mouse
    void UpdateWidgetPosition();
    
    // Offset from cursor (so tooltip doesn't cover cursor)
    UPROPERTY(EditAnywhere, Category = "Grid|UI")
    FVector2D TooltipOffset = FVector2D(15.0f, 15.0f);
};