// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridVisualizer.generated.h"

// Forward declarations
class UKalkiGridManager;
class UInstancedStaticMeshComponent;

/**
 * Grid Visualizer
 * Renders the tactical grid with multi-layer border system
 * 
 * FEATURES:
 * - Base tiles show terrain type (green/yellow/orange/gray based on MovementCost)
 * - Border layers show movement ranges with multiple tiers (cyan/yellow/orange)
 * - Hover effect highlights single tile (brighten + outline)
 * - Selection system for clicked tiles
 */
UCLASS()
class KALKI_API AKalkiGridVisualizer : public AActor
{
    GENERATED_BODY()

public:
    AKalkiGridVisualizer();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

    // ========================================
    // COMPONENTS
    // ========================================

    /** Base tile mesh component (shows terrain colors) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid|Components")
    TObjectPtr<UInstancedStaticMeshComponent> TileInstancedMeshComponent;

    /** Border tier components (dynamically created, one per movement tier) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid|Components")
    TArray<TObjectPtr<UInstancedStaticMeshComponent>> BorderTierComponents;

    /** Hover highlight component (single tile brightening + outline) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid|Components")
    TObjectPtr<UInstancedStaticMeshComponent> HoverOverlayComponent;

    // ========================================
    // SETTINGS
    // ========================================

    /** Show debug visualizations? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization")
    bool bShowDebug = false;

    /** Is grid visible? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization")
    bool bGridVisible = true;

    // ========================================
    // MESHES & MATERIALS
    // ========================================

    /** Base tile mesh (plane/quad) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Assets")
    TObjectPtr<UStaticMesh> TileMesh;

    /** Base tile material */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Assets")
    TObjectPtr<UMaterialInterface> TileMaterial;

    /** Border frame mesh (hollow square, generated if null) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Assets")
    TObjectPtr<UStaticMesh> BorderFrameMesh;

    /** Border material (emissive for glow) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Assets")
    TObjectPtr<UMaterialInterface> BorderMaterial;

    /** Hover overlay material */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Assets")
    TObjectPtr<UMaterialInterface> HoverMaterial;

    // ========================================
    // TERRAIN COLORS (Low Alpha for Visibility)
    // ========================================

    /** Normal walkable terrain color (light green, low alpha) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Terrain Colors")
    FLinearColor NormalTerrainColor = FLinearColor(0.3f, 0.8f, 0.3f, 0.3f);

    /** Difficult terrain color (yellow, low alpha) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Terrain Colors")
    FLinearColor DifficultTerrainColor = FLinearColor(0.8f, 0.8f, 0.3f, 0.3f);

    /** Very difficult terrain color (orange, low alpha) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Terrain Colors")
    FLinearColor VeryDifficultTerrainColor = FLinearColor(0.9f, 0.5f, 0.2f, 0.3f);

    /** Impassable terrain color (dark gray) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Terrain Colors")
    FLinearColor ImpassableTerrainColor = FLinearColor(0.2f, 0.2f, 0.2f, 0.5f);

    // ========================================
    // HOVER SETTINGS
    // ========================================

    /** Hover brightness multiplier (1.5 = 50% brighter) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Hover")
    float HoverBrightness = 1.5f;

    /** Hover outline color (white) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Visualization|Hover")
    FLinearColor HoverOutlineColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    

    // ========================================
    // STATE
    // ========================================

    /** Currently selected tile */
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Visualization")
    FKalkiGridCoord SelectedTile;

    /** Is a tile selected? */
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Visualization")
    bool bHasSelection = false;

    /** Currently hovered tile */
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Visualization")
    FKalkiGridCoord HoveredTile;

    /** Is a tile hovered? */
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Visualization")
    bool bHasHover = false;

    /** Current movement tiers being displayed */
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Visualization")
    TArray<FKalkiMovementTier> ActiveMovementTiers;

    // ========================================
    // TILE SELECTION (Used by GridInteractionComponent)
    // ========================================

    /**
     * Select a tile (visual feedback only, doesn't move anything)
     * Used when player clicks on a tile
     */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void SelectTile(const FKalkiGridCoord& Coord);

    /** Clear tile selection */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void ClearSelection();

    /** Get currently selected tile */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Visualization")
    FKalkiGridCoord GetSelectedTile() const { return SelectedTile; }

    /** Is a tile currently selected? */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Visualization")
    bool HasSelection() const { return bHasSelection; }

    // ========================================
    // GRID VISIBILITY (Used by GridInteractionComponent)
    // ========================================

    /** Show the grid */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void ShowGrid();

    /** Hide the grid */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void HideGrid();

    /** Set grid visibility */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void SetShowGrid(bool bShow);

    /** Is grid currently visible? */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Visualization")
    bool IsGridVisible() const { return bGridVisible; }

    // ========================================
    // LEGACY COMPATIBILITY (Aliases)
    // ========================================

    /** Deselect tile (alias for ClearSelection) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void DeselectTile() { ClearSelection(); }

    // ========================================
    // MOVEMENT RANGE (Multi-Tier System)
    // ========================================

    /**
     * Show movement range with multiple tiers (Normal/Dash/Triple)
     * 
     * @param Origin - Starting position (character location)
     * @param Tiers - Array of movement tiers (range + color + Z-offset)
     * 
     * EXAMPLE:
     * TArray<FKalkiMovementTier> Tiers;
     * Tiers.Add(FKalkiMovementTier(6, Cyan, "Normal", 5.0f));
     * Tiers.Add(FKalkiMovementTier(12, Yellow, "Dash", 6.0f));
     * ShowMovementRange(CharPos, Tiers);
     * 
     * This will show cyan borders for tiles 0-6 and yellow borders for tiles 7-12.
     * Borders are nested (tiles 0-6 have BOTH cyan and yellow).
     */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void ShowMovementRange(const FKalkiGridCoord& Origin, const TArray<FKalkiMovementTier>& Tiers);

    /** Hide movement range (clear all border layers) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void HideMovementRange();

    /** Is movement range currently shown? */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Visualization")
    bool IsShowingMovementRange() const { return ActiveMovementTiers.Num() > 0; }

    // ========================================
    // GRID CREATION/UPDATES
    // ========================================

    /** Create visual instances for all tiles */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void CreateGridVisuals();

    /** Clear all visual instances */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void ClearGridVisuals();

    /** Update single tile visual (color based on terrain cost) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void UpdateTileVisual(const FKalkiGridCoord& Coord);

    /** Refresh all tile visuals */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Visualization")
    void RefreshAllVisuals();

private:
    // ========================================
    // CACHED REFERENCES
    // ========================================

    UPROPERTY()
    TObjectPtr<UKalkiGridManager> GridManager;

    // ========================================
    // INTERNAL FUNCTIONS
    // ========================================

    /** Cache subsystem references */
    void CacheReferences();

    /** Initialize mesh components */
    void InitializeMeshComponents();

    /** Generate border frame mesh if not provided */
    void GenerateBorderFrameMesh();

    /** Called when grid is created */
    UFUNCTION()
    void OnGridCreated();

    /** Called when grid is cleared */
    UFUNCTION()
    void OnGridCleared();

    /** Called when tile changes */
    UFUNCTION()
    void OnTileChanged(const FKalkiGridCoord& Coord);

    /** Get terrain color for tile based on movement cost */
    FLinearColor GetTerrainColorForTile(const FKalkiGridCoord& Coord) const;

    /** Set instance color (for base tiles) */
    void SetTileInstanceColor(int32 InstanceIndex, const FLinearColor& Color);

    /** Update hover effect (called every frame in Tick) */
    void UpdateHoverEffect();

    /** Clear hover effect */
    void ClearHoverEffect();

    /** Get tile under cursor (for hover detection) */
    FKalkiGridCoord GetTileUnderCursor(bool& bSuccess);

    /** Raycast to grid from mouse position */
    bool RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord);
};