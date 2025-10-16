// Copyright of V.S. Puranam and no one else

// Public/Level/KalkiLevelManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KalkiLevelManager.generated.h"

class UKalkiGridManager;
class AKalkiGridVisualizer;

/**
 * Level Manager
 * Placed in each level to configure level-specific settings
 * Initializes the grid, spawns characters, sets up encounters
 * 
 * MULTIPLAYER NOTES:
 * - Replicates to all clients
 * - Grid configuration syncs deterministically
 * - Server creates grid, clients build identical copy
 * - Always relevant to all players
 */
UCLASS()
class KALKI_API AKalkiLevelManager : public AActor
{
    GENERATED_BODY()

public:
    AKalkiLevelManager();

    // Replication
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void ShowGridVisualizer();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void HideGridVisualizer();
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Grid configuration (Replicated for deterministic grid)
    UPROPERTY(EditAnywhere, Replicated, Category = "Grid")
    int32 GridSizeX = 20;

    UPROPERTY(EditAnywhere, Replicated, Category = "Grid")
    int32 GridSizeY = 20;

    UPROPERTY(EditAnywhere, Replicated, Category = "Grid")
    float TileSize = 100.0f;

    UPROPERTY(EditAnywhere, Replicated, Category = "Grid")
    FVector GridOrigin = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Replicated, Category = "Grid")
    bool bAutoCreateGrid = true;

    // Elevation configuration (Replicated)
    UPROPERTY(EditAnywhere, Replicated, Category = "Grid|Elevation")
    bool bUseElevation = true;

    UPROPERTY(EditAnywhere, Replicated, Category = "Grid|Elevation", meta = (EditCondition = "bUseElevation"))
    float MaxClimbHeight = 200.0f;

    UPROPERTY(EditAnywhere, Replicated, Category = "Grid|Elevation", meta = (EditCondition = "bUseElevation"))
    float MaxDropHeight = 300.0f;

    // Grid Visualization
    UPROPERTY(EditAnywhere, Category = "Grid|Visualization", meta = (
        Tooltip = "Grid visualizer class to spawn. Leave empty for no visualization."))
    TSubclassOf<AKalkiGridVisualizer> GridVisualizerClass;

    UPROPERTY(EditAnywhere, Category = "Grid|Visualization")
    bool bAutoSpawnVisualizer = true;

    UPROPERTY(EditAnywhere, Category = "Grid|Visualization")
    bool bShowVisualizerInCombat = true;

    UPROPERTY(EditAnywhere, Category = "Grid|Visualization")
    bool bShowVisualizerInStrategy = false;

    // Runtime references
    UPROPERTY()
    TObjectPtr<UKalkiGridManager> GridManager;

    UPROPERTY()
    TObjectPtr<AKalkiGridVisualizer> GridVisualizer;

    // Initialize the level (Server only)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void InitializeLevel();

    // Create the grid (Server only)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void CreateGrid();

    // Grid event handlers
    UFUNCTION()
    void OnGridCreated();

    // Visualizer management
    void SpawnGridVisualizer();
    void DestroyGridVisualizer();

    // Elevation helpers (Server only)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void SetTileElevation(int32 X, int32 Y, float Elevation);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void CreatePlatform(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float Elevation);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Level")
    void CreateRamp(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float StartElevation, float EndElevation);
};