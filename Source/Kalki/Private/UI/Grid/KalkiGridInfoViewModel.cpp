// Copyright of V.S. Puranam and no one else

// Private/UI/Grid/KalkiGridInfoViewModel.cpp

#include "UI/Grid/KalkiGridInfoViewModel.h"
#include "Grid/KalkiGridManager.h"
#include "Logging/KalkiLog.h"

void UKalkiGridInfoViewModel::UpdateFromTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile)
{
    // Update coordinate
    TileCoordinate = Coord;

    // Update basic tile info
    Elevation = Tile.Elevation;
    bWalkable = Tile.bWalkable;
    bOccupied = Tile.IsOccupied();

    // Update occupant name if present
    if (bOccupied && Tile.Occupant.Get())
    {
        OccupantName = Tile.Occupant->GetName();
    }
    else
    {
        OccupantName = TEXT("");
    }

    // Reset movement/combat info (will be set separately if needed)
    MovementCost = 0.0f;
    DistanceFromOrigin = 0;
    bInMovementRange = false;
    bInAttackRange = false;

    KalkiLog::Grid(
        FString::Printf(TEXT("GridInfoViewModel updated: Tile (%d, %d), Elevation=%.1f, Walkable=%s"),
            Coord.X, Coord.Y, Elevation, bWalkable ? TEXT("Yes") : TEXT("No"))
    );
}

void UKalkiGridInfoViewModel::UpdateMovementInfo(float Cost, int32 Distance, bool bInRange)
{
    MovementCost = Cost;
    DistanceFromOrigin = Distance;
    bInMovementRange = bInRange;
    bShowMovementInfo = true;

    KalkiLog::Grid(
        FString::Printf(TEXT("GridInfoViewModel movement updated: Cost=%.1f, Distance=%d, InRange=%s"),
            Cost, Distance, bInRange ? TEXT("Yes") : TEXT("No"))
    );
}

void UKalkiGridInfoViewModel::UpdateCombatInfo(bool bInRange, float Cover, bool bLOS)
{
    bInAttackRange = bInRange;
    CoverBonus = Cover;
    bHasLineOfSight = bLOS;
    bShowCombatInfo = true;

    KalkiLog::Grid(
        FString::Printf(TEXT("GridInfoViewModel combat updated: InRange=%s, Cover=%.1f, LOS=%s"),
            bInRange ? TEXT("Yes") : TEXT("No"), Cover, bLOS ? TEXT("Yes") : TEXT("No"))
    );
}

void UKalkiGridInfoViewModel::Clear()
{
    TileCoordinate = FKalkiGridCoord(-1, -1);
    Elevation = 0.0f;
    bWalkable = true;
    bOccupied = false;
    OccupantName = TEXT("");
    MovementCost = 0.0f;
    DistanceFromOrigin = 0;
    bInMovementRange = false;
    bInAttackRange = false;
    CoverBonus = 0.0f;
    bHasLineOfSight = true;
    bShowMovementInfo = false;
    bShowCombatInfo = false;

    KalkiLog::Grid(TEXT("GridInfoViewModel cleared"));
}

FText UKalkiGridInfoViewModel::GetCoordinateText() const
{
    if (TileCoordinate.X < 0 || TileCoordinate.Y < 0)
    {
        return INVTEXT("No Tile");
    }

    return FText::Format(
        INVTEXT("Tile: ({0}, {1})"),
        TileCoordinate.X,
        TileCoordinate.Y
    );
}

FText UKalkiGridInfoViewModel::GetElevationText() const
{
    return FText::Format(
        INVTEXT("Elevation: {0}"),
        FText::AsNumber(Elevation)
    );
}

FText UKalkiGridInfoViewModel::GetWalkableText() const
{
    if (bOccupied)
    {
        return FText::Format(
            INVTEXT("Occupied: {0}"),
            FText::FromString(OccupantName)
        );
    }
    else if (bWalkable)
    {
        return INVTEXT("Walkable: Yes");
    }
    else
    {
        return INVTEXT("Walkable: No");
    }
}

FText UKalkiGridInfoViewModel::GetOccupiedText() const
{
    if (bOccupied)
    {
        return FText::Format(
            INVTEXT("Occupied by: {0}"),
            FText::FromString(OccupantName)
        );
    }
    else
    {
        return INVTEXT("Unoccupied");
    }
}

FText UKalkiGridInfoViewModel::GetMovementCostText() const
{
    if (!bShowMovementInfo)
    {
        return FText::GetEmpty();
    }

    if (!bInMovementRange)
    {
        return INVTEXT("Out of Range");
    }

    return FText::Format(
        INVTEXT("Cost: {0} ({1} tiles)"),
        FText::AsNumber(MovementCost, &FNumberFormattingOptions::DefaultNoGrouping()),
        DistanceFromOrigin
    );
}

FLinearColor UKalkiGridInfoViewModel::GetWalkableColor() const
{
    if (bOccupied)
    {
        return FLinearColor(1.0f, 0.5f, 0.0f); // Orange
    }
    else if (bWalkable)
    {
        return FLinearColor(0.0f, 1.0f, 0.0f); // Green
    }
    else
    {
        return FLinearColor(1.0f, 0.0f, 0.0f); // Red
    }
}

FLinearColor UKalkiGridInfoViewModel::GetRangeColor() const
{
    if (!bShowMovementInfo)
    {
        return FLinearColor::White;
    }

    if (bInMovementRange)
    {
        return FLinearColor(0.0f, 0.5f, 1.0f); // Blue (in range)
    }
    else
    {
        return FLinearColor(0.5f, 0.5f, 0.5f); // Gray (out of range)
    }
}