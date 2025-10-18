// Copyright of V.S. Puranam and no one else

// Private/Grid/KalkiGridManager.cpp

#include "Grid/KalkiGridManager.h"

#include "Grid/Components/KalkiGridOccupant.h"
#include "Logging/KalkiLog.h"

void UKalkiGridManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    KalkiLog::Grid(TEXT("Grid Manager initialized"));
}

void UKalkiGridManager::Deinitialize()
{
    ClearGrid();
    
    KalkiLog::Grid(TEXT("Grid Manager deinitialized"));
    
    Super::Deinitialize();
}

bool UKalkiGridManager::HasGridAuthority() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    // In standalone or server, we have authority
    // In client, we don't
    ENetMode NetMode = World->GetNetMode();
    return NetMode == NM_Standalone || NetMode == NM_DedicatedServer || NetMode == NM_ListenServer;
}

void UKalkiGridManager::CreateGrid(int32 SizeX, int32 SizeY, float InTileSize, const FVector& Origin)
{
    // Only server can create grid
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("CreateGrid called on client - ignoring"), EKalkiLogSeverity::Warning);
        return;
    }

    // Clear existing grid
    ClearGrid();

    // Validate parameters
    if (SizeX <= 0 || SizeY <= 0)
    {
        KalkiLog::Grid(TEXT("Invalid grid size"), EKalkiLogSeverity::Error);
        return;
    }

    if (InTileSize <= 0.0f)
    {
        KalkiLog::Grid(TEXT("Invalid tile size"), EKalkiLogSeverity::Error);
        return;
    }

    // Store parameters
    GridSizeX = SizeX;
    GridSizeY = SizeY;
    TileSize = InTileSize;
    GridOrigin = Origin;

    // Create tiles
    GridTiles.Reserve(SizeX * SizeY);

    for (int32 X = 0; X < SizeX; ++X)
    {
        for (int32 Y = 0; Y < SizeY; ++Y)
        {
            FKalkiGridCoord Coord(X, Y);
            FVector WorldPos = CoordToWorldPosition(Coord);

            FKalkiGridTile Tile(Coord, WorldPos);
            GridTiles.Add(Coord, Tile);
        }
    }

    KalkiLog::Grid(
        FString::Printf(TEXT("Grid created: %dx%d, TileSize=%.1f, Origin=%s [Authority: %s]"), 
            SizeX, SizeY, InTileSize, *Origin.ToString(),
            HasGridAuthority() ? TEXT("Server") : TEXT("Client"))
    );

    // Broadcast grid created event
    OnGridCreated.Broadcast();
}

void UKalkiGridManager::ClearGrid()
{
    // Only server can clear grid
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("ClearGrid called on client - ignoring"), EKalkiLogSeverity::Warning);
        return;
    }

    // Broadcast grid cleared event
    OnGridCleared.Broadcast();

    GridTiles.Empty();
    GridSizeX = 0;
    GridSizeY = 0;
    TileSize = 100.0f;
    GridOrigin = FVector::ZeroVector;

    KalkiLog::Grid(TEXT("Grid cleared"));
}

bool UKalkiGridManager::IsValidCoord(const FKalkiGridCoord& Coord) const
{
    return Coord.X >= 0 && Coord.X < GridSizeX &&
           Coord.Y >= 0 && Coord.Y < GridSizeY;
}

FKalkiGridTile UKalkiGridManager::GetTile(const FKalkiGridCoord& Coord) const
{
    if (const FKalkiGridTile* Tile = GridTiles.Find(Coord))
    {
        return *Tile;
    }

    // Return invalid tile
    return FKalkiGridTile();
}

FKalkiGridTile* UKalkiGridManager::GetTileMutable(const FKalkiGridCoord& Coord)
{
    return GridTiles.Find(Coord);
}

bool UKalkiGridManager::SetTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile)
{
    // Only server can modify grid
    if (!HasGridAuthority())
    {
        return false;
    }

    if (!IsValidCoord(Coord))
    {
        return false;
    }

    GridTiles.Add(Coord, Tile);

    // Broadcast tile changed event
    OnTileChanged.Broadcast(Coord);
    
    
    return true;
}

bool UKalkiGridManager::IsTileWalkable(const FKalkiGridCoord& Coord) const
{
    const FKalkiGridTile* Tile = GridTiles.Find(Coord);
    return Tile ? Tile->IsWalkable() : false;
}

bool UKalkiGridManager::IsTileOccupied(const FKalkiGridCoord& Coord) const
{
    const FKalkiGridTile* Tile = GridTiles.Find(Coord);
    return Tile ? Tile->IsOccupied() : false;
}

FVector UKalkiGridManager::CoordToWorldPosition(const FKalkiGridCoord& Coord) const
{
    // Get tile to check elevation
    const FKalkiGridTile* Tile = GridTiles.Find(Coord);
    float TileElevation = Tile ? Tile->Elevation : 0.0f;

    // Calculate world position from grid coordinate
    float WorldX = GridOrigin.X + (Coord.X * TileSize);
    float WorldY = GridOrigin.Y + (Coord.Y * TileSize);
    float WorldZ = GridOrigin.Z + TileElevation;

    return FVector(WorldX, WorldY, WorldZ);
}

FKalkiGridCoord UKalkiGridManager::WorldPositionToCoord(const FVector& WorldPos) const
{
    // Convert world position to grid coordinate (ignoring Z for coordinate)
    int32 X = FMath::FloorToInt((WorldPos.X - GridOrigin.X) / TileSize);
    int32 Y = FMath::FloorToInt((WorldPos.Y - GridOrigin.Y) / TileSize);

    return FKalkiGridCoord(X, Y);
}

void UKalkiGridManager::SetTileOccupant(const FKalkiGridCoord& Coord, AActor* Occupant)
{
    if (!IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetTileOccupant - Invalid coord: %s"), *Coord.ToString()),
            EKalkiLogSeverity::Warning
        );
        return;
    }

    // ✅ ADD - Validate occupant implements interface (if not null)
    if (Occupant && !Occupant->Implements<UKalkiGridOccupant>())
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetTileOccupant - Actor '%s' doesn't implement IKalkiGridOccupant"), 
                *Occupant->GetName()),
            EKalkiLogSeverity::Warning
        );
        // We'll allow it for now but log warning
    }

    FKalkiGridTile& Tile = GridTiles[Coord];
    Tile.Occupant = Occupant;

    // Broadcast change
    OnTileChanged.Broadcast(Coord);

    KalkiLog::Grid(
        FString::Printf(TEXT("Tile %s occupant set to: %s"), 
            *Coord.ToString(),
            Occupant ? *Occupant->GetName() : TEXT("None")),
        EKalkiLogSeverity::Verbose
    );
}

bool UKalkiGridManager::ClearTileOccupant(const FKalkiGridCoord& Coord)
{
    // Only server can clear occupancy
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("ClearTileOccupant called on client - ignoring"), EKalkiLogSeverity::Warning);
        return false;
    }

    FKalkiGridTile* Tile = GetTileMutable(Coord);
    if (!Tile)
    {
        return false;
    }

    Tile->Occupant = nullptr;
    return true;
}

AActor* UKalkiGridManager::GetTileOccupant(const FKalkiGridCoord& Coord) const
{
    const FKalkiGridTile* Tile = GridTiles.Find(Coord);
    return Tile && Tile->Occupant.IsValid() ? Tile->Occupant.Get() : nullptr;
}

float UKalkiGridManager::GetElevation(const FKalkiGridCoord& Coord) const
{
    const FKalkiGridTile* Tile = GridTiles.Find(Coord);
    return Tile ? Tile->Elevation : 0.0f;
}

bool UKalkiGridManager::SetElevation(const FKalkiGridCoord& Coord, float Elevation)
{
    // Only server can modify elevation
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("SetElevation called on client - ignoring"), EKalkiLogSeverity::Warning);
        return false;
    }

    FKalkiGridTile* Tile = GetTileMutable(Coord);
    if (!Tile)
    {
        return false;
    }

    Tile->Elevation = Elevation;
    
    // Update world position to match new elevation
    Tile->WorldPosition = CoordToWorldPosition(Coord);

    // Broadcast tile changed event
    OnTileChanged.Broadcast(Coord);
    
    return true;
}

float UKalkiGridManager::GetElevationDifference(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const
{
    return GetElevation(To) - GetElevation(From);
}

bool UKalkiGridManager::IsClimbable(const FKalkiGridCoord& From, const FKalkiGridCoord& To, float MaxClimbHeight) const
{
    float ElevationDiff = GetElevationDifference(From, To);
    
    // Can't climb down (that's falling/jumping)
    if (ElevationDiff < 0.0f)
    {
        return false;
    }

    // Check if climb is within max height
    return ElevationDiff <= MaxClimbHeight;
}

bool UKalkiGridManager::HasLineOfSight(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const
{
    // Simplified line of sight check
    // Full implementation would check all tiles between From and To
    // For now, just check if target is visible based on elevation
    
    float FromElevation = GetElevation(From);
    float ToElevation = GetElevation(To);
    
    // Higher position can see lower positions
    // Equal height depends on obstacles (future implementation)
    
    // TODO: Implement proper raycast through grid tiles
    // For now, always return true
    return true;
}

TArray<FKalkiGridCoord> UKalkiGridManager::GetNeighbors(const FKalkiGridCoord& Coord, bool bDiagonalAllowed) const
{
    TArray<FKalkiGridCoord> Neighbors;

    // Cardinal directions (N, E, S, W)
    static const FKalkiGridCoord CardinalOffsets[] = {
        FKalkiGridCoord(0, 1),   // North
        FKalkiGridCoord(1, 0),   // East
        FKalkiGridCoord(0, -1),  // South
        FKalkiGridCoord(-1, 0)   // West
    };

    // Diagonal directions (NE, SE, SW, NW)
    static const FKalkiGridCoord DiagonalOffsets[] = {
        FKalkiGridCoord(1, 1),   // NE
        FKalkiGridCoord(1, -1),  // SE
        FKalkiGridCoord(-1, -1), // SW
        FKalkiGridCoord(-1, 1)   // NW
    };

    // Add cardinal neighbors
    for (const FKalkiGridCoord& Offset : CardinalOffsets)
    {
        FKalkiGridCoord Neighbor = Coord + Offset;
        if (IsValidCoord(Neighbor))
        {
            Neighbors.Add(Neighbor);
        }
    }

    // Add diagonal neighbors if allowed
    if (bDiagonalAllowed)
    {
        for (const FKalkiGridCoord& Offset : DiagonalOffsets)
        {
            FKalkiGridCoord Neighbor = Coord + Offset;
            if (IsValidCoord(Neighbor))
            {
                Neighbors.Add(Neighbor);
            }
        }
    }

    return Neighbors;
}

TArray<FKalkiGridCoord> UKalkiGridManager::GetWalkableNeighbors(const FKalkiGridCoord& Coord, bool bDiagonalAllowed) const
{
    TArray<FKalkiGridCoord> Neighbors = GetNeighbors(Coord, bDiagonalAllowed);
    
    // Filter to only walkable AND climbable
    Neighbors = Neighbors.FilterByPredicate([this, Coord](const FKalkiGridCoord& Neighbor)
    {
        // Must be walkable
        if (!IsTileWalkable(Neighbor))
        {
            return false;
        }

        // Check if elevation change is reasonable
        float ElevationDiff = GetElevationDifference(Coord, Neighbor);
        
        // Can't climb more than 2 levels (200 units)
        if (ElevationDiff > 200.0f)
        {
            return false;
        }

        // Can't drop more than 3 levels (300 units) without taking damage
        if (ElevationDiff < -300.0f)
        {
            return false;
        }

        return true;
    });

    return Neighbors;
}

TArray<FKalkiGridCoord> UKalkiGridManager::GetTilesInRange(const FKalkiGridCoord& Center, int32 Range, bool bRequireWalkable) const
{
    TArray<FKalkiGridCoord> TilesInRange;

    // Iterate through all tiles within Manhattan distance
    for (int32 X = Center.X - Range; X <= Center.X + Range; ++X)
    {
        for (int32 Y = Center.Y - Range; Y <= Center.Y + Range; ++Y)
        {
            FKalkiGridCoord Coord(X, Y);

            // Check if valid
            if (!IsValidCoord(Coord))
            {
                continue;
            }

            // Check if within range (Manhattan distance)
            if (Coord.DistanceTo(Center) > Range)
            {
                continue;
            }

            // Check walkability if required
            if (bRequireWalkable && !IsTileWalkable(Coord))
            {
                continue;
            }

            TilesInRange.Add(Coord);
        }
    }

    return TilesInRange;
}

float UKalkiGridManager::GetMovementCost(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const
{
    const FKalkiGridTile* FromTile = GridTiles.Find(From);
    const FKalkiGridTile* ToTile = GridTiles.Find(To);
    
    if (!FromTile || !ToTile)
    {
        return TNumericLimits<float>::Max(); // Invalid tile = infinite cost
    }

    // Base cost depends on direction
    bool bIsDiagonal = (From.X != To.X) && (From.Y != To.Y);
    float BaseCost = bIsDiagonal ? 1.414f : 1.0f; // sqrt(2) for diagonal

    // Apply terrain cost multiplier
    float TerrainCost = BaseCost * ToTile->MovementCost;

    // Add elevation change cost
    float ElevationDifference = ToTile->Elevation - FromTile->Elevation;
    
    // Climbing up costs more (like stairs/ladders)
    if (ElevationDifference > 0.0f)
    {
        // Cost increases based on height climbed
        // Each "level" of elevation (100 units) costs extra movement
        float ElevationLevels = ElevationDifference / 100.0f;
        float ClimbCost = ElevationLevels * 0.5f; // Climbing 1 level = +0.5 movement cost
        TerrainCost += ClimbCost;
    }
    // Going down is free (or even faster)
    else if (ElevationDifference < 0.0f)
    {
        // Check for dangerous drops
        float DropDistance = FMath::Abs(ElevationDifference);
        if (DropDistance > 200.0f) // More than 2 levels
        {
            // Prevent pathfinding through dangerous drops
            // (Can still jump, but pathfinder won't choose it automatically)
            TerrainCost += 10.0f; // High cost = avoid
        }
    }

    return TerrainCost;
}

FKalkiGridPath UKalkiGridManager::FindPath(const FKalkiGridCoord& Start, const FKalkiGridCoord& End, bool bDiagonalAllowed)
{
    FKalkiGridPath Path;

    // Validate inputs
    if (!IsValidCoord(Start) || !IsValidCoord(End))
    {
        KalkiLog::Grid(TEXT("FindPath: Invalid start or end coordinate"), EKalkiLogSeverity::Warning);
        return Path;
    }

    if (!IsTileWalkable(Start) || !IsTileWalkable(End))
    {
        KalkiLog::Grid(TEXT("FindPath: Start or end tile is not walkable"), EKalkiLogSeverity::Warning);
        return Path;
    }

    // If start == end, return path with just that tile
    if (Start == End)
    {
        Path.Waypoints.Add(Start);
        Path.TotalCost = 0.0f;
        Path.bIsValid = true;
        return Path;
    }

    // A* pathfinding implementation
    // We'll implement this in Phase 3
    // For now, return invalid path
    KalkiLog::Grid(TEXT("FindPath: A* pathfinding not yet implemented"));

    return Path;
}

TSet<FKalkiGridCoord> UKalkiGridManager::GetTilesInRangeEuclidean(const FKalkiGridCoord& Origin, float Range) const
{
    TSet<FKalkiGridCoord> TilesInRange;

    if (!IsValidCoord(Origin))
    {
        return TilesInRange;
    }

    // Calculate bounding box based on range
    int32 MaxOffset = FMath::CeilToInt(Range);

    // Check all tiles within bounding box
    for (int32 X = Origin.X - MaxOffset; X <= Origin.X + MaxOffset; ++X)
    {
        for (int32 Y = Origin.Y - MaxOffset; Y <= Origin.Y + MaxOffset; ++Y)
        {
            FKalkiGridCoord Coord(X, Y);

            // Skip invalid coords
            if (!IsValidCoord(Coord))
            {
                continue;
            }

            // Calculate Euclidean distance
            float DX = static_cast<float>(Coord.X - Origin.X);
            float DY = static_cast<float>(Coord.Y - Origin.Y);
            float Distance = FMath::Sqrt(DX * DX + DY * DY);

            // If within range, add to set
            if (Distance <= Range)
            {
                TilesInRange.Add(Coord);
            }
        }
    }

    return TilesInRange;
}