// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridOccupant.generated.h"

// Forward declarations
class UKalkiGridOccupancyComponent;

/**
 * UInterface boilerplate (required by Unreal)
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UKalkiGridOccupant : public UInterface
{
	GENERATED_BODY()
};

/**
 * Grid Occupant Interface
 * Implement this on any actor that can occupy grid tiles
 * 
 * IMPLEMENTERS:
 * - Characters (AKalkiCharacter)
 * - Traps
 * - Objects
 * - Summons
 * - Environmental hazards
 * 
 * USAGE:
 * if (IKalkiGridOccupant* Occupant = Cast<IKalkiGridOccupant>(Actor))
 * {
 *     FKalkiGridCoord Pos = Occupant->GetGridPosition();
 * }
 */
class KALKI_API IKalkiGridOccupant
{
	GENERATED_BODY()

public:
	// ========================================
	// INTERFACE METHODS
	// ========================================

	/**
	 * Get current grid position
	 * @return Current coordinate on grid (Invalid if not on grid)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kalki|Grid")
	FKalkiGridCoord GetGridPosition() const;

	/**
	 * Check if this occupant can occupy a specific tile
	 * Allows custom validation (flying creatures, incorporeal, etc.)
	 * @param Coord The coordinate to check
	 * @return True if it can occupy, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kalki|Grid")
	bool CanOccupyTile(const FKalkiGridCoord& Coord) const;

	/**
	 * Get size in tiles (1 = normal, 2 = large, 4 = huge, etc.)
	 * For future large creature support
	 * @return Number of tiles occupied
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kalki|Grid")
	int32 GetOccupancySize() const;

	/**
	 * Get the grid occupancy component (if exists)
	 * @return Component or nullptr
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kalki|Grid")
	UKalkiGridOccupancyComponent* GetGridOccupancyComponent() const;

	// ========================================
	// HELPER (NON-VIRTUAL)
	// ========================================

	/**
	 * Check if occupant is currently on grid
	 * @return True if on valid grid position
	 */
	bool IsOnGrid() const
	{
		return Execute_GetGridPosition(Cast<UObject>(this)).IsValid();
	}
};