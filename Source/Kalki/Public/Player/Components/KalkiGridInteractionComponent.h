// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridInteractionComponent.generated.h"

// Forward declarations
class UKalkiGridManager;
class AKalkiGridVisualizer;

/**
 * Grid Interaction Component
 * Handles mouse interaction with the grid (clicking, raycasting, selection)
 * Attached to PlayerController
 */
UCLASS(ClassGroup=(Kalki), meta=(BlueprintSpawnableComponent))
class KALKI_API UKalkiGridInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKalkiGridInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ========================================
	// SETTINGS
	// ========================================

	/** Enable click detection? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")
	bool bEnableClickDetection = true;

	/** Show debug visualizations? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")
	bool bShowDebug = false;

	/** Raycast channel for grid detection */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")
	TEnumAsByte<ECollisionChannel> RaycastChannel = ECC_Visibility;

	/** Maximum raycast distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")
	float MaxRaycastDistance = 50000.0f;

	// ========================================
	// STATE
	// ========================================

	/** Currently selected tile */
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Interaction")
	FKalkiGridCoord SelectedTile;

	/** Is a tile selected? */
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Interaction")
	bool bHasSelection = false;

	// ========================================
	// PUBLIC FUNCTIONS
	// ========================================

	/** Handle left mouse click */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")
	void HandleLeftClick();

	/** Handle right mouse click */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")
	void HandleRightClick();

	/** Raycast from camera to world, return hit tile */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")
	bool RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord);

	/** Get tile under mouse cursor */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")
	FKalkiGridCoord GetTileUnderCursor(bool& bSuccess);

	/** Select a tile */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")
	void SelectTile(const FKalkiGridCoord& Coord);

	/** Clear tile selection */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")
	void ClearSelection();

	/** Get selected tile (returns invalid coord if none selected) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Interaction")
	FKalkiGridCoord GetSelectedTile() const { return SelectedTile; }

	/** Is a tile currently selected? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Interaction")
	bool HasSelection() const { return bHasSelection; }

private:
	// ========================================
	// CACHED REFERENCES
	// ========================================

	UPROPERTY()
	TObjectPtr<UKalkiGridManager> GridManager;

	UPROPERTY()
	TObjectPtr<AKalkiGridVisualizer> GridVisualizer;

	UPROPERTY()
	TObjectPtr<APlayerController> OwningController;

	// ========================================
	// INTERNAL FUNCTIONS
	// ========================================

	/** Cache subsystem references */
	void CacheReferences();

	/** Draw debug visuals */
	void DrawDebugVisuals(const FVector& Start, const FVector& End, bool bHit, const FVector& HitLocation);
};