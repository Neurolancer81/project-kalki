// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridLibrary.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KalkiGridLibrary.generated.h"

/**
 * Static helper functions for grid operations
 * Distance calculations, neighbor queries, and other grid utilities
 */
UCLASS()
class KALKI_API UKalkiGridLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Movement cost constants
    static constexpr float ORTHOGONAL_MOVE_COST = 1.0f;
    static constexpr float DIAGONAL_MOVE_COST = 1.41421356f; // √2

    // Distance calculations
    
    /** Manhattan distance (sum of X and Y differences) - no diagonals */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static int32 GetManhattanDistance(const FIntPoint& A, const FIntPoint& B);

    /** Euclidean distance (straight line) - returns float */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static float GetEuclideanDistance(const FIntPoint& A, const FIntPoint& B);

    /** Chebyshev distance (max of X or Y difference) - allows diagonals */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static int32 GetChebyshevDistance(const FIntPoint& A, const FIntPoint& B);

    // Neighbor queries
    
    /** Get 4 orthogonal neighbors (N, S, E, W) */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static TArray<FIntPoint> GetOrthogonalNeighbors(const FIntPoint& Position);

    /** Get all 8 neighbors (orthogonal + diagonals) */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static TArray<FIntPoint> GetAllNeighbors(const FIntPoint& Position);

    // Movement cost
    
    /** Calculate movement cost between two adjacent tiles (1.0 or √2) */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static float GetMovementCost(const FIntPoint& From, const FIntPoint& To);

    /** Check if two positions are adjacent (orthogonal or diagonal) */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static bool AreAdjacent(const FIntPoint& A, const FIntPoint& B);

    // Direction helpers
    
    /** Get normalized direction vector from A to B */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    static FIntPoint GetDirectionVector(const FIntPoint& From, const FIntPoint& To);
};