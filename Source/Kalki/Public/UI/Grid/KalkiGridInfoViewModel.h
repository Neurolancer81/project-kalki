// Copyright of V.S. Puranam and no one else

// Public/UI/Common/KalkiGridInfoViewModel.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelBase.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridInfoViewModel.generated.h"

class UKalkiGridManager;
class AKalkiCharacter;

/**
 * Grid Info ViewModel
 * Holds data for displaying grid tile information
 * Used by hover tooltips, selection panels, and action previews
 * 
 * DATA FLOW:
 * - GridVisualizer updates ViewModel when tile hovered/selected
 * - Character/Ability systems can add additional data
 * - Widget binds to properties and updates automatically
 */
UCLASS(BlueprintType)
class KALKI_API UKalkiGridInfoViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // ========================================
    // TILE BASIC INFO
    // ========================================

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info")
    FKalkiGridCoord TileCoordinate = FKalkiGridCoord(-1, -1);

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info")
    float Elevation = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info")
    bool bWalkable = true;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info")
    bool bOccupied = false;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info")
    FString OccupantName = TEXT("");

    // ========================================
    // MOVEMENT INFO (For Phase 4)
    // ========================================

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Movement")
    float MovementCost = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Movement")
    int32 DistanceFromOrigin = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Movement")
    bool bInMovementRange = false;

    // ========================================
    // COMBAT INFO (For Phase 6)
    // ========================================

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Combat")
    bool bInAttackRange = false;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Combat")
    float CoverBonus = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Combat")
    bool bHasLineOfSight = true;

    // ========================================
    // DISPLAY FLAGS
    // ========================================

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Display")
    bool bShowMovementInfo = false;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Info|Display")
    bool bShowCombatInfo = false;

    // ========================================
    // UPDATE METHODS
    // ========================================

    // Update from tile data (basic info)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Info")
    void UpdateFromTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile);

    // Update movement info (for character moving)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Info")
    void UpdateMovementInfo(float Cost, int32 Distance, bool bInRange);

    // Update combat info (for abilities)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Info")
    void UpdateCombatInfo(bool bInRange, float Cover, bool bLOS);

    // Clear all data (hide widget)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Info")
    void Clear();

    // ========================================
    // FORMATTED TEXT (For Display)
    // ========================================

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FText GetCoordinateText() const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FText GetElevationText() const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FText GetWalkableText() const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FText GetOccupiedText() const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FText GetMovementCostText() const;

    // ========================================
    // COLOR HELPERS (For Dynamic Styling)
    // ========================================

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FLinearColor GetWalkableColor() const;

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    FLinearColor GetRangeColor() const;
};