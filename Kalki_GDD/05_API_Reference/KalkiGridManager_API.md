---
tags:
  - kalki/api
  - kalki/grid
  - kalki/reference
  - type: api-reference
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# KalkiGridManager API Reference

## Overview
Complete API reference for the UKalkiGridManager WorldSubsystem. This document provides detailed information about all public functions, properties, events, and usage patterns for the grid system.

---

## Purpose

### What is KalkiGridManager?

**KalkiGridManager** is a WorldSubsystem that manages the tactical grid for Kalki. It provides:
- Grid creation and management
- Tile data storage and queries
- Spatial queries (neighbors, range)
- Coordinate conversion
- Multiplayer-safe operations (server authority)
- Event broadcasting for grid changes

**See also:** [[03_Systems/Grid_System]] for architectural details and design patterns.

---

## Quick Start

### Basic Usage
```cpp
#include "Grid/KalkiGridManager.h"

// Get the manager
UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();

// Create grid (server only)
GridManager->CreateGrid(20, 20, 100.0f, FVector::ZeroVector);

// Query tiles
FKalkiGridTile Tile = GridManager->GetTile(FKalkiGridCoord(5, 7));

// Check validity
if (GridManager->IsValidCoord(FKalkiGridCoord(10, 10)))
{
    // Coordinate is within bounds
}

// Get neighbors
TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(FKalkiGridCoord(5, 5));

// Convert coordinates
FVector WorldPos = GridManager->CoordToWorldPosition(FKalkiGridCoord(10, 10));
FKalkiGridCoord GridCoord = GridManager->WorldPositionToCoord(WorldPos);
```

---

## Class Overview

### UKalkiGridManager

**Type:** WorldSubsystem  
**Path:** `Source/Kalki/Public/Grid/KalkiGridManager.h`  
**Parent:** `UWorldSubsystem`

**Lifetime:** Per-world (created when world loads, destroyed when world unloads)

**Authority:** Server creates and modifies, clients query only

**Purpose:**
- Manage grid data (tile storage, properties)
- Provide spatial queries (neighbors, range, pathfinding)
- Handle coordinate conversion
- Broadcast events for grid changes
- Ensure multiplayer safety (server authority)

---

## Subsystem Lifecycle

### Initialize()

**Signature:**
```cpp
virtual void Initialize(FSubsystemCollectionBase& Collection) override;
```

**Description:**
Called when subsystem is created (world loads). Initializes internal state.

**When Called:**
- Automatically by Unreal when world loads
- Before BeginPlay on actors

**Usage:**
```cpp
// Called automatically - DO NOT call manually
```

**Notes:**
- Grid is NOT created in Initialize
- Call CreateGrid() explicitly to create grid
- Override to add custom initialization

---

### Deinitialize()

**Signature:**
```cpp
virtual void Deinitialize() override;
```

**Description:**
Called when subsystem is destroyed (world unloads). Cleans up resources.

**When Called:**
- Automatically when world unloads
- Before world destruction

**Usage:**
```cpp
// Called automatically - DO NOT call manually
```

**Notes:**
- Clears all grid data
- Unbinds any events
- Override to add custom cleanup

---

## Authority Functions

### HasGridAuthority()

**Signature:**
```cpp
bool HasGridAuthority() const;
```

**Returns:** `true` if running on server or standalone, `false` if client

**Description:**
Checks if this instance has authority to modify the grid. Only server/standalone can modify grid state.

**Example:**
```cpp
if (GridManager->HasGridAuthority())
{
    // Can modify grid
    GridManager->SetElevation(Coord, 200.0f);
}
else
{
    // Client - can only query
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
}
```

**Network Modes:**
- `NM_Standalone` → Returns `true`
- `NM_ListenServer` → Returns `true`
- `NM_DedicatedServer` → Returns `true`
- `NM_Client` → Returns `false`

**Usage Notes:**
- Always check before modifying grid
- Queries work on all machines
- Modifications ignored on clients (logged as warning)

---

## Grid Creation

### CreateGrid()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
void CreateGrid(
    int32 SizeX,
    int32 SizeY,
    float TileSize,
    FVector Origin
);
```

**Parameters:**
- `SizeX` - Width of grid in tiles (must be > 0)
- `SizeY` - Height of grid in tiles (must be > 0)
- `TileSize` - Size of each tile in world units (typically 100.0)
- `Origin` - World space origin point (bottom-left corner)

**Description:**
Creates a new grid with the specified dimensions. Destroys existing grid if present.

**Authority:** Server only (checked internally)

**Example:**
```cpp
// Create 20x20 grid, 100 units per tile, at world origin
GridManager->CreateGrid(20, 20, 100.0f, FVector::ZeroVector);

// Create 30x30 grid at offset position
GridManager->CreateGrid(30, 30, 100.0f, FVector(1000.0f, 1000.0f, 0.0f));
```

**Events Fired:**
- `OnGridCreated` - Broadcasts when grid creation complete

**Notes:**
- Only call on server/standalone
- Clears existing grid if present
- All tiles default: walkable, elevation 0, no occupant
- Grid is deterministic (same parameters = same grid on all machines)

**Common Sizes:**
- Small: 10x10 (100 tiles)
- Medium: 20x20 (400 tiles)
- Large: 50x50 (2500 tiles)

---

### ClearGrid()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
void ClearGrid();
```

**Description:**
Destroys the current grid and clears all tile data.

**Authority:** Server only

**Example:**
```cpp
GridManager->ClearGrid();
```

**Events Fired:**
- `OnGridCleared` - Broadcasts when grid cleared

**Notes:**
- Frees all memory used by grid
- Sets `bGridCreated` to `false`
- Safe to call even if no grid exists

---

### IsGridCreated()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
bool IsGridCreated() const;
```

**Returns:** `true` if grid exists, `false` if not created

**Description:**
Checks if a grid has been created and is ready for use.

**Example:**
```cpp
if (GridManager->IsGridCreated())
{
    // Safe to query grid
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
}
else
{
    // Grid not created yet
    UE_LOG(LogTemp, Warning, TEXT("Grid not created"));
}
```

**Notes:**
- Always check before grid operations
- Returns `false` after `ClearGrid()`
- Thread-safe (const function)

---

## Grid Properties

### GetGridSizeX()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
int32 GetGridSizeX() const;
```

**Returns:** Width of grid in tiles

**Example:**
```cpp
int32 Width = GridManager->GetGridSizeX();
// Width = 20 for 20x20 grid
```

---

### GetGridSizeY()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
int32 GetGridSizeY() const;
```

**Returns:** Height of grid in tiles

**Example:**
```cpp
int32 Height = GridManager->GetGridSizeY();
// Height = 20 for 20x20 grid
```

---

### GetTileSize()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
float GetTileSize() const;
```

**Returns:** Size of each tile in world units

**Example:**
```cpp
float Size = GridManager->GetTileSize();
// Size = 100.0 for standard grid
```

---

### GetGridOrigin()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
FVector GetGridOrigin() const;
```

**Returns:** World space origin point (bottom-left corner of grid)

**Example:**
```cpp
FVector Origin = GridManager->GetGridOrigin();
// Origin = (0, 0, 0) if grid at world origin
```

---

## Coordinate Validation

### IsValidCoord()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
bool IsValidCoord(const FKalkiGridCoord& Coord) const;
```

**Parameters:**
- `Coord` - Grid coordinate to validate

**Returns:** `true` if coordinate is within grid bounds, `false` otherwise

**Description:**
Checks if a coordinate is valid (within grid bounds).

**Example:**
```cpp
FKalkiGridCoord Coord(5, 7);
if (GridManager->IsValidCoord(Coord))
{
    // Coordinate is valid (0 <= X < SizeX, 0 <= Y < SizeY)
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
}
else
{
    // Out of bounds
}
```

**Validation Rules:**
```cpp
// Valid if:
Coord.X >= 0 && Coord.X < GridSizeX &&
Coord.Y >= 0 && Coord.Y < GridSizeY
```

**Notes:**
- Always validate before accessing tiles
- Returns `false` if grid not created
- Very fast (simple bounds check)

---

## Tile Queries

### GetTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
FKalkiGridTile GetTile(const FKalkiGridCoord& Coord) const;
```

**Parameters:**
- `Coord` - Grid coordinate to query

**Returns:** Tile data at coordinate, or default tile if invalid

**Description:**
Retrieves tile data for a specific coordinate.

**Example:**
```cpp
FKalkiGridCoord Coord(10, 10);
FKalkiGridTile Tile = GridManager->GetTile(Coord);

// Access tile properties
float Elevation = Tile.Elevation;
bool bWalkable = Tile.bWalkable;
FVector WorldPos = Tile.WorldPosition;
```

**Return Value:**
- Valid coord → Actual tile data
- Invalid coord → Default tile (unwalkable, elevation 0)

**Notes:**
- Does NOT validate coordinate
- Returns by value (copy)
- Safe to call on clients
- Use `IsValidCoord()` first for safety

---

### GetTilePtr()

**Signature:**
```cpp
FKalkiGridTile* GetTilePtr(const FKalkiGridCoord& Coord);
const FKalkiGridTile* GetTilePtr(const FKalkiGridCoord& Coord) const;
```

**Parameters:**
- `Coord` - Grid coordinate to query

**Returns:** Pointer to tile, or `nullptr` if invalid

**Description:**
Retrieves pointer to tile data (for modification or read-only access).

**Example:**
```cpp
// Mutable access
FKalkiGridTile* Tile = GridManager->GetTilePtr(Coord);
if (Tile)
{
    Tile->Elevation = 200.0f;
}

// Read-only access
const FKalkiGridTile* Tile = GridManager->GetTilePtr(Coord);
if (Tile)
{
    float Elevation = Tile->Elevation;
}
```

**Notes:**
- Returns `nullptr` if coordinate invalid
- Faster than `GetTile()` (no copy)
- Always check for `nullptr`
- Mutable version for modification

---

## Tile Modifications

### SetTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
bool SetTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile);
```

**Parameters:**
- `Coord` - Grid coordinate to modify
- `Tile` - New tile data

**Returns:** `true` if successful, `false` if failed (no authority or invalid coord)

**Authority:** Server only

**Description:**
Replaces tile data at specified coordinate.

**Example:**
```cpp
FKalkiGridCoord Coord(5, 5);
FKalkiGridTile NewTile;
NewTile.Coordinate = Coord;
NewTile.Elevation = 200.0f;
NewTile.bWalkable = true;
NewTile.WorldPosition = GridManager->CoordToWorldPosition(Coord);

if (GridManager->SetTile(Coord, NewTile))
{
    // Tile updated successfully
}
```

**Events Fired:**
- `OnTileChanged` - Broadcasts coordinate of changed tile

**Notes:**
- Server only (checked internally)
- Validates coordinate
- Broadcasts event on success
- Use for bulk tile updates

---

### SetElevation()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
bool SetElevation(const FKalkiGridCoord& Coord, float Elevation);
```

**Parameters:**
- `Coord` - Grid coordinate
- `Elevation` - New elevation value (world units)

**Returns:** `true` if successful, `false` if failed

**Authority:** Server only

**Description:**
Sets the elevation of a specific tile.

**Example:**
```cpp
// Set tile to 200 units elevation
GridManager->SetElevation(FKalkiGridCoord(10, 10), 200.0f);

// Create elevated platform
for (int32 Y = 5; Y < 10; ++Y)
{
    for (int32 X = 5; X < 10; ++X)
    {
        GridManager->SetElevation(FKalkiGridCoord(X, Y), 300.0f);
    }
}
```

**Events Fired:**
- `OnTileChanged` - Broadcasts coordinate

**Notes:**
- Server only
- Elevation is abstract height (not literal Z)
- Affects movement cost, line of sight
- Typical range: 0-1000 units

---

### SetTileWalkable()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
bool SetTileWalkable(const FKalkiGridCoord& Coord, bool bWalkable);
```

**Parameters:**
- `Coord` - Grid coordinate
- `bWalkable` - New walkability state

**Returns:** `true` if successful, `false` if failed

**Authority:** Server only

**Description:**
Sets whether a tile is walkable (characters can move there).

**Example:**
```cpp
// Block a tile (wall, obstacle)
GridManager->SetTileWalkable(FKalkiGridCoord(5, 5), false);

// Open a tile (door opened)
GridManager->SetTileWalkable(FKalkiGridCoord(10, 10), true);

// Create walls around perimeter
for (int32 X = 0; X < GridSizeX; ++X)
{
    GridManager->SetTileWalkable(FKalkiGridCoord(X, 0), false);
    GridManager->SetTileWalkable(FKalkiGridCoord(X, GridSizeY - 1), false);
}
```

**Events Fired:**
- `OnTileChanged` - Broadcasts coordinate

**Notes:**
- Server only
- Affects pathfinding
- Visualizer shows unwalkable tiles differently
- Can change at runtime (doors, traps)

---

### SetTileOccupant()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
bool SetTileOccupant(const FKalkiGridCoord& Coord, AActor* Occupant);
```

**Parameters:**
- `Coord` - Grid coordinate
- `Occupant` - Actor occupying the tile (or `nullptr` to clear)

**Returns:** `true` if successful, `false` if failed

**Authority:** Server only

**Description:**
Sets which actor is currently occupying a tile.

**Example:**
```cpp
// Character moves to tile
GridManager->SetTileOccupant(NewCoord, Character);

// Character leaves tile
GridManager->SetTileOccupant(OldCoord, nullptr);

// Check if tile occupied
FKalkiGridTile Tile = GridManager->GetTile(Coord);
if (Tile.IsOccupied())
{
    AActor* Occupant = Tile.Occupant.Get();
    // Tile is occupied
}
```

**Events Fired:**
- `OnTileChanged` - Broadcasts coordinate

**Notes:**
- Server only
- Stored as `TWeakObjectPtr` (doesn't prevent GC)
- One occupant per tile (current limitation)
- Check `IsOccupied()` before moving

---

## Spatial Queries

### GetNeighbors()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
TArray<FKalkiGridCoord> GetNeighbors(
    const FKalkiGridCoord& Coord,
    bool bIncludeDiagonals = false
) const;
```

**Parameters:**
- `Coord` - Center coordinate
- `bIncludeDiagonals` - Include diagonal neighbors (default: false)

**Returns:** Array of neighboring coordinates (within bounds)

**Description:**
Gets all valid neighboring tiles of a coordinate.

**Example:**
```cpp
// Get 4 cardinal neighbors
TArray<FKalkiGridCoord> Cardinals = GridManager->GetNeighbors(
    FKalkiGridCoord(10, 10),
    false
);
// Returns: (9,10), (11,10), (10,9), (10,11)

// Get 8 neighbors (with diagonals)
TArray<FKalkiGridCoord> All = GridManager->GetNeighbors(
    FKalkiGridCoord(10, 10),
    true
);
// Returns: 8 tiles around center
```

**Neighbor Pattern:**

**Cardinal (4 neighbors):**
```
       (X, Y+1)
          ↑
(X-1, Y) ← • → (X+1, Y)
          ↓
       (X, Y-1)
```

**With Diagonals (8 neighbors):**
```
(X-1,Y+1)  (X,Y+1)  (X+1,Y+1)
    ↖        ↑        ↗
(X-1,Y)  ←  •  →  (X+1,Y)
    ↙        ↓        ↘
(X-1,Y-1)  (X,Y-1)  (X+1,Y-1)
```

**Notes:**
- Only returns valid coordinates (within bounds)
- Corner tiles return fewer neighbors
- Edge tiles return 2-3 neighbors (4 direction)
- Edge tiles return 3-5 neighbors (8 direction)

---

### GetTilesInRange()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
TArray<FKalkiGridCoord> GetTilesInRange(
    const FKalkiGridCoord& Center,
    int32 Range,
    bool bRequireWalkable = false
) const;
```

**Parameters:**
- `Center` - Center coordinate
- `Range` - Maximum distance (Manhattan distance)
- `bRequireWalkable` - Only return walkable tiles (default: false)

**Returns:** Array of coordinates within range

**Description:**
Gets all tiles within a specified Manhattan distance from center.

**Example:**
```cpp
// Get all tiles within 5 tiles
TArray<FKalkiGridCoord> AllTiles = GridManager->GetTilesInRange(
    FKalkiGridCoord(10, 10),
    5
);

// Get only walkable tiles within 5 tiles
TArray<FKalkiGridCoord> WalkableTiles = GridManager->GetTilesInRange(
    FKalkiGridCoord(10, 10),
    5,
    true
);

// Character movement range
int32 MovementRange = Character->GetMovementRange();
TArray<FKalkiGridCoord> ReachableTiles = GridManager->GetTilesInRange(
    Character->GetGridPosition(),
    MovementRange,
    true
);
```

**Manhattan Distance:**
```
Distance = |X1 - X2| + |Y1 - Y2|

Range 2 example:
      2
    2 1 2
  2 1 0 1 2
    2 1 2
      2
```

**Performance:**
- Time: O(R²) where R = range
- Range 5: ~80 tiles checked
- Range 10: ~300 tiles checked

**Notes:**
- Includes center coordinate (distance 0)
- Only returns valid coordinates
- Use `bRequireWalkable` for movement ranges
- Good for ability ranges, movement previews

---

## Coordinate Conversion

### CoordToWorldPosition()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
FVector CoordToWorldPosition(const FKalkiGridCoord& Coord) const;
```

**Parameters:**
- `Coord` - Grid coordinate to convert

**Returns:** World space position (center of tile)

**Description:**
Converts grid coordinate to world space position.

**Example:**
```cpp
FKalkiGridCoord Coord(5, 7);
FVector WorldPos = GridManager->CoordToWorldPosition(Coord);
// WorldPos = Origin + (5.5 * TileSize, 7.5 * TileSize, Elevation)

// Spawn actor at tile center
AActor* Actor = GetWorld()->SpawnActor<AActor>(
    ActorClass,
    WorldPos,
    FRotator::ZeroRotator
);
```

**Formula:**
```cpp
WorldPos.X = Origin.X + (Coord.X * TileSize) + (TileSize * 0.5f);
WorldPos.Y = Origin.Y + (Coord.Y * TileSize) + (TileSize * 0.5f);
WorldPos.Z = Origin.Z + Elevation;
```

**Notes:**
- Returns tile CENTER, not corner
- Includes elevation in Z coordinate
- Safe to call on clients
- Works even if coordinate invalid

---

### WorldPositionToCoord()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
FKalkiGridCoord WorldPositionToCoord(const FVector& WorldPosition) const;
```

**Parameters:**
- `WorldPosition` - World space position

**Returns:** Grid coordinate containing that position

**Description:**
Converts world space position to grid coordinate.

**Example:**
```cpp
// Get grid coordinate under mouse cursor
FVector WorldLocation, WorldDirection;
PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

FHitResult Hit;
GetWorld()->LineTraceSingleByChannel(
    Hit,
    WorldLocation,
    WorldLocation + WorldDirection * 10000,
    ECC_Visibility
);

if (Hit.bBlockingHit)
{
    FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(Hit.Location);
    // Coord is the tile at that world position
}

// Get character's current tile
FKalkiGridCoord CharCoord = GridManager->WorldPositionToCoord(
    Character->GetActorLocation()
);
```

**Formula:**
```cpp
FVector LocalPos = WorldPosition - Origin;
Coord.X = FMath::FloorToInt(LocalPos.X / TileSize);
Coord.Y = FMath::FloorToInt(LocalPos.Y / TileSize);
```

**Notes:**
- Uses floor (rounds down)
- Ignores Z coordinate
- May return invalid coordinate (check with `IsValidCoord`)
- Safe to call on clients

---

## Events

### OnGridCreated

**Signature:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCreated);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnGridCreated OnGridCreated;
```

**Fired When:** Grid creation completes

**Parameters:** None

**Description:**
Broadcast when `CreateGrid()` completes successfully.

**Example:**
```cpp
// C++ Binding
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->OnGridCreated.AddDynamic(this, &AMyActor::OnGridCreated);
}

void AMyActor::OnGridCreated()
{
    // Grid is ready
    InitializeGridDependentSystems();
}

void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GridManager)
    {
        GridManager->OnGridCreated.RemoveDynamic(this, &AMyActor::OnGridCreated);
    }
    
    Super::EndPlay(EndPlayReason);
}
```

**Blueprint Usage:**
```
Event BeginPlay
  → Get Grid Manager
  → Bind Event to OnGridCreated
  → OnGridCreated Event
    → Initialize Grid Systems
```

**Use Cases:**
- Initialize GridVisualizer
- Start pathfinding system
- Spawn characters on grid
- Update UI with grid info

---

### OnTileChanged

**Signature:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileChanged, const FKalkiGridCoord&, Coord);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnTileChanged OnTileChanged;
```

**Fired When:** Tile properties change

**Parameters:**
- `Coord` - Coordinate of changed tile

**Description:**
Broadcast when tile is modified (elevation, walkability, occupancy).

**Example:**
```cpp
// C++ Binding
void AKalkiGridVisualizer::BeginPlay()
{
    Super::BeginPlay();
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->OnTileChanged.AddDynamic(this, &AKalkiGridVisualizer::OnTileChanged);
}

void AKalkiGridVisualizer::OnTileChanged(const FKalkiGridCoord& Coord)
{
    // Update visual for this tile
    UpdateTileVisual(Coord);
}
```

**Use Cases:**
- Update GridVisualizer
- Invalidate pathfinding cache
- Update AI knowledge
- Trigger tile effects

---

### OnGridCleared

**Signature:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCleared);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnGridCleared OnGridCleared;
```

**Fired When:** Grid is destroyed

**Parameters:** None

**Description:**
Broadcast when `ClearGrid()` is called.

**Example:**
```cpp
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->OnGridCleared.AddDynamic(this, &AMyActor::OnGridCleared);
}

void AMyActor::OnGridCleared()
{
    // Grid destroyed - clean up
    ClearPathCache();
    DestroyGridVisuals();
}
```

**Use Cases:**
- Clean up visualizer
- Clear pathfinding cache
- Reset grid-dependent systems
- Transition to new level

---

## Usage Patterns

### Pattern: Grid Initialization
```cpp
void AKalkiLevelManager::InitializeGrid()
{
    if (!HasAuthority())
    {
        return; // Clients don't create grid
    }

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Create grid
    GridManager->CreateGrid(GridSizeX, GridSizeY, TileSize, GridOrigin);
    
    // Set up terrain
    CreateElevatedPlatforms();
    CreateWalls();
    
    // Grid is ready - visualizer will receive OnGridCreated event
}

void AKalkiLevelManager::CreateElevatedPlatforms()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Platform 1 (5x5, elevation 200)
    for (int32 Y = 5; Y < 10; ++Y)
    {
        for (int32 X = 5; X < 10; ++X)
        {
            GridManager->SetElevation(FKalkiGridCoord(X, Y), 200.0f);
        }
    }
    
    // Platform 2 (3x3, elevation 400)
    for (int32 Y = 15; Y < 18; ++Y)
    {
        for (int32 X = 15; X < 18; ++X)
        {
            GridManager->SetElevation(FKalkiGridCoord(X, Y), 400.0f);
        }
    }
}
```

---

### Pattern: Character Movement
```cpp
void AKalkiCharacter::MoveTo(const FKalkiGridCoord& Target)
{
    if (!HasAuthority())
    {
        // Client requests server to move
        ServerMoveTo(Target);
        return;
    }

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Validate target
    if (!GridManager->IsValidCoord(Target))
    {
        KalkiLog::Movement(TEXT("Invalid target coordinate"), EKalkiLogSeverity::Warning, this);
        return;
    }

    FKalkiGridTile TargetTile = GridManager->GetTile(Target);
    if (!TargetTile.bWalkable)
    {
        KalkiLog::Movement(TEXT("Target tile not walkable"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (TargetTile.IsOccupied())
    {
        KalkiLog::Movement(TEXT("Target tile occupied"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Clear old position
    GridManager->SetTileOccupant(GridPosition, nullptr);

    // Move to new position
    GridPosition = Target;
    GridManager->SetTileOccupant(Target, this);

    // Update visual position
    FVector WorldPos = GridManager->CoordToWorldPosition(Target);
    SetActorLocation(WorldPos);

    KalkiLog::Movement(
        FString::Printf(TEXT("%s moved to (%d, %d)```cpp
        FString::Printf(TEXT("%s moved to (%d, %d)"), *GetName(), Target.X, Target.Y),
        EKalkiLogSeverity::Log,
        this
    );
}
```

---

### Pattern: Movement Range Query

```cpp
TArray<FKalkiGridCoord> AKalkiCharacter::GetReachableTiles() const
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Get all tiles within movement range
    TArray<FKalkiGridCoord> InRange = GridManager->GetTilesInRange(
        GridPosition,
        MovementRange,
        true  // Only walkable tiles
    );

    // Filter out occupied tiles
    TArray<FKalkiGridCoord> Reachable;
    for (const FKalkiGridCoord& Coord : InRange)
    {
        FKalkiGridTile Tile = GridManager->GetTile(Coord);
        if (!Tile.IsOccupied() || Coord == GridPosition)
        {
            Reachable.Add(Coord);
        }
    }

    return Reachable;
}

void AKalkiCharacter::ShowMovementRange()
{
    TArray<FKalkiGridCoord> Reachable = GetReachableTiles();
    
    // Visualize reachable tiles
    OnMovementRangeUpdated.Broadcast(Reachable);
}
```

---

### Pattern: Area of Effect Query

```cpp
TArray<AKalkiCharacter*> UKalkiAbility::GetTargetsInArea(
    const FKalkiGridCoord& Center,
    int32 Radius
) const
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Get all tiles in radius
    TArray<FKalkiGridCoord> TilesInArea = GridManager->GetTilesInRange(Center, Radius);
    
    // Find characters on those tiles
    TArray<AKalkiCharacter*> Targets;
    for (const FKalkiGridCoord& Coord : TilesInArea)
    {
        FKalkiGridTile Tile = GridManager->GetTile(Coord);
        if (Tile.IsOccupied())
        {
            if (AKalkiCharacter* Character = Cast<AKalkiCharacter>(Tile.Occupant.Get()))
            {
                Targets.Add(Character);
            }
        }
    }

    return Targets;
}

void UKalkiAbility::CastFireball(const FKalkiGridCoord& Target)
{
    // Get all characters in 2-tile radius
    TArray<AKalkiCharacter*> Targets = GetTargetsInArea(Target, 2);
    
    // Apply damage to all targets
    for (AKalkiCharacter* Character : Targets)
    {
        Character->TakeDamage(DamageAmount);
    }

    KalkiLog::Abilities(
        FString::Printf(TEXT("Fireball hit %d targets"), Targets.Num())
    );
}
```

---

### Pattern: Line of Sight Check

```cpp
bool UKalkiCombatSystem::HasLineOfSight(
    const FKalkiGridCoord& From,
    const FKalkiGridCoord& To
) const
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Get tiles between From and To using Bresenham's line algorithm
    TArray<FKalkiGridCoord> Line = GetLineCoords(From, To);
    
    FKalkiGridTile FromTile = GridManager->GetTile(From);
    FKalkiGridTile ToTile = GridManager->GetTile(To);
    
    // Check each tile in line
    for (const FKalkiGridCoord& Coord : Line)
    {
        // Skip start and end
        if (Coord == From || Coord == To)
        {
            continue;
        }

        FKalkiGridTile Tile = GridManager->GetTile(Coord);
        
        // Check if tile blocks line of sight
        if (!Tile.bWalkable)
        {
            return false; // Wall or obstacle
        }

        // Check elevation difference (high ground blocks sight)
        if (Tile.Elevation > FromTile.Elevation + 100.0f &&
            Tile.Elevation > ToTile.Elevation + 100.0f)
        {
            return false; // High ground blocks
        }
    }

    return true; // Clear line of sight
}
```

---

### Pattern: Pathfinding Integration

```cpp
TArray<FKalkiGridCoord> UKalkiPathfinding::FindPath(
    const FKalkiGridCoord& Start,
    const FKalkiGridCoord& Goal
)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Validate start and goal
    if (!GridManager->IsValidCoord(Start) || !GridManager->IsValidCoord(Goal))
    {
        KalkiLog::Movement(TEXT("Invalid pathfinding coordinates"), EKalkiLogSeverity::Error);
        return TArray<FKalkiGridCoord>();
    }

    // A* pathfinding
    TSet<FKalkiGridCoord> Visited;
    TMap<FKalkiGridCoord, FKalkiGridCoord> CameFrom;
    TMap<FKalkiGridCoord, float> GScore;
    
    GScore.Add(Start, 0.0f);
    TArray<FKalkiGridCoord> OpenSet;
    OpenSet.Add(Start);

    while (OpenSet.Num() > 0)
    {
        // Get node with lowest F score
        FKalkiGridCoord Current = GetLowestFScore(OpenSet, GScore, Goal);
        
        if (Current == Goal)
        {
            return ReconstructPath(CameFrom, Current);
        }

        OpenSet.Remove(Current);
        Visited.Add(Current);

        // Check neighbors
        TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(Current, false);
        for (const FKalkiGridCoord& Neighbor : Neighbors)
        {
            if (Visited.Contains(Neighbor))
            {
                continue;
            }

            // Check if walkable
            FKalkiGridTile Tile = GridManager->GetTile(Neighbor);
            if (!Tile.bWalkable || (Tile.IsOccupied() && Neighbor != Goal))
            {
                continue;
            }

            // Calculate cost (consider elevation change)
            float MoveCost = CalculateMoveCost(Current, Neighbor);
            float TentativeGScore = GScore[Current] + MoveCost;

            if (!GScore.Contains(Neighbor) || TentativeGScore < GScore[Neighbor])
            {
                CameFrom.Add(Neighbor, Current);
                GScore.Add(Neighbor, TentativeGScore);
                
                if (!OpenSet.Contains(Neighbor))
                {
                    OpenSet.Add(Neighbor);
                }
            }
        }
    }

    // No path found
    return TArray<FKalkiGridCoord>();
}

float UKalkiPathfinding::CalculateMoveCost(
    const FKalkiGridCoord& From,
    const FKalkiGridCoord& To
)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    FKalkiGridTile FromTile = GridManager->GetTile(From);
    FKalkiGridTile ToTile = GridManager->GetTile(To);
    
    float BaseCost = 1.0f;
    float ElevationDiff = FMath::Abs(ToTile.Elevation - FromTile.Elevation);
    
    // Moving uphill costs more
    if (ToTile.Elevation > FromTile.Elevation)
    {
        BaseCost += ElevationDiff / 100.0f;
    }

    return BaseCost;
}
```

---

### Pattern: Grid Interaction (Click to Move)

```cpp
void AKalkiPlayerController::OnMouseClick()
{
    // Get world position under cursor
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    
    if (!Hit.bBlockingHit)
    {
        return;
    }

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Convert to grid coordinate
    FKalkiGridCoord ClickedCoord = GridManager->WorldPositionToCoord(Hit.Location);
    
    // Validate coordinate
    if (!GridManager->IsValidCoord(ClickedCoord))
    {
        KalkiLog::UI(TEXT("Clicked outside grid"), EKalkiLogSeverity::Warning);
        return;
    }

    // Check if walkable
    FKalkiGridTile Tile = GridManager->GetTile(ClickedCoord);
    if (!Tile.bWalkable)
    {
        KalkiLog::UI(TEXT("Clicked unwalkable tile"), EKalkiLogSeverity::Warning);
        ShowFeedback("Cannot move there");
        return;
    }

    // Move character
    if (SelectedCharacter)
    {
        SelectedCharacter->MoveTo(ClickedCoord);
    }
}
```

---

### Pattern: Grid Hover Preview

```cpp
void AKalkiPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateHoveredTile();
}

void AKalkiPlayerController::UpdateHoveredTile()
{
    // Get world position under cursor
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    FKalkiGridCoord NewHoveredCoord;
    bool bValidHover = false;

    if (Hit.bBlockingHit)
    {
        NewHoveredCoord = GridManager->WorldPositionToCoord(Hit.Location);
        bValidHover = GridManager->IsValidCoord(NewHoveredCoord);
    }

    // Check if hover changed
    if (bValidHover && NewHoveredCoord != CurrentHoveredCoord)
    {
        CurrentHoveredCoord = NewHoveredCoord;
        OnTileHovered.Broadcast(CurrentHoveredCoord);
    }
    else if (!bValidHover && CurrentHoveredCoord.IsValid())
    {
        CurrentHoveredCoord = FKalkiGridCoord(-1, -1);
        OnTileHoverEnded.Broadcast();
    }
}
```

---

### Pattern: Dynamic Obstacles

```cpp
void AKalkiDoor::OnDoorOpened()
{
    if (!HasAuthority())
    {
        return;
    }

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Door opens - make tile walkable
    GridManager->SetTileWalkable(DoorCoordinate, true);
    
    KalkiLog::Grid(
        FString::Printf(TEXT("Door opened at (%d, %d)"), DoorCoordinate.X, DoorCoordinate.Y)
    );

    // Play animation
    PlayOpenAnimation();
}

void AKalkiDoor::OnDoorClosed()
{
    if (!HasAuthority())
    {
        return;
    }

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Check if anyone on tile
    FKalkiGridTile Tile = GridManager->GetTile(DoorCoordinate);
    if (Tile.IsOccupied())
    {
        KalkiLog::Grid(TEXT("Cannot close door - tile occupied"), EKalkiLogSeverity::Warning);
        return;
    }

    // Door closes - make tile unwalkable
    GridManager->SetTileWalkable(DoorCoordinate, false);
    
    KalkiLog::Grid(
        FString::Printf(TEXT("Door closed at (%d, %d)"), DoorCoordinate.X, DoorCoordinate.Y)
    );

    // Play animation
    PlayCloseAnimation();
}
```

---

## Performance Considerations

### Memory Usage

**Grid Storage:**

```cpp
// Memory per tile: ~48 bytes
// 20x20 grid: 400 tiles × 48 bytes = 19.2 KB
// 50x50 grid: 2500 tiles × 48 bytes = 120 KB
// 100x100 grid: 10000 tiles × 48 bytes = 480 KB
```

**Recommendations:**

- Keep grids under 100×100 for best performance
- Use single large grid instead of multiple small grids
- TMap storage is efficient for sparse grids

---

### Query Performance

**Operation Complexity:**

|Operation|Complexity|Notes|
|---|---|---|
|`GetTile()`|O(1)|Hash map lookup|
|`SetTile()`|O(1)|Hash map insertion|
|`IsValidCoord()`|O(1)|Simple comparison|
|`GetNeighbors()`|O(1)|Fixed 4-8 checks|
|`GetTilesInRange()`|O(R²)|R = range|
|`CoordToWorldPosition()`|O(1)|Math calculation|
|`WorldPositionToCoord()`|O(1)|Math calculation|

**Tips:**

- Cache results of expensive queries
- Use `GetTilePtr()` instead of `GetTile()` when possible
- Limit `GetTilesInRange()` radius
- Invalidate cache when tiles change

---

### Network Performance

**Replication:**

- Grid data is NOT replicated (too large)
- Server creates grid via `CreateGrid()`
- Clients receive grid parameters and create locally
- Tile changes broadcast via events (replicated)

**Best Practices:**

- Create identical grid on all machines
- Use deterministic grid generation
- Replicate only tile changes, not entire grid
- Batch tile updates when possible

---

## Common Mistakes

### Mistake 1: Not Checking Authority

```cpp
// ❌ BAD - Modifies on client
void AMyActor::SomeFunction()
{
    GridManager->SetElevation(Coord, 200.0f); // Fails on client, logs warning
}

// ✅ GOOD - Check authority
void AMyActor::SomeFunction()
{
    if (!HasAuthority())
    {
        return;
    }

    GridManager->SetElevation(Coord, 200.0f);
}
```

---

### Mistake 2: Not Validating Coordinates

```cpp
// ❌ BAD - No validation
FKalkiGridTile Tile = GridManager->GetTile(Coord); // Returns default if invalid
Tile.Elevation = 200.0f; // Modifies default, not actual tile!

// ✅ GOOD - Validate first
if (GridManager->IsValidCoord(Coord))
{
    FKalkiGridTile* Tile = GridManager->GetTilePtr(Coord);
    if (Tile)
    {
        Tile->Elevation = 200.0f;
    }
}
```

---

### Mistake 3: Not Checking Grid Created

```cpp
// ❌ BAD - Assumes grid exists
FKalkiGridTile Tile = GridManager->GetTile(Coord); // Crash if grid not created!

// ✅ GOOD - Check first
if (GridManager->IsGridCreated())
{
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
}
```

---

### Mistake 4: Modifying Returned Tile

```cpp
// ❌ BAD - Modifies copy, not original
FKalkiGridTile Tile = GridManager->GetTile(Coord);
Tile.Elevation = 200.0f; // Changes local copy only!

// ✅ GOOD - Use setter or pointer
GridManager->SetElevation(Coord, 200.0f);

// OR
FKalkiGridTile* Tile = GridManager->GetTilePtr(Coord);
if (Tile)
{
    Tile->Elevation = 200.0f;
}
```

---

### Mistake 5: Not Clearing Occupancy

```cpp
// ❌ BAD - Doesn't clear old tile
GridPosition = NewCoord;
GridManager->SetTileOccupant(NewCoord, this);
// Old tile still shows occupied!

// ✅ GOOD - Clear old, set new
GridManager->SetTileOccupant(GridPosition, nullptr);
GridPosition = NewCoord;
GridManager->SetTileOccupant(NewCoord, this);
```

---

## Troubleshooting

### Grid Not Appearing

**Problem:** Grid created but not visible

**Solutions:**

1. Check if GridVisualizer exists and is bound to `OnGridCreated`
2. Verify GridVisualizer is receiving event
3. Check if visualizer mesh/material set correctly
4. Verify grid created on server (clients can't create)
5. Check `CreateGrid()` called after world fully loaded

---

### Coordinate Mismatch

**Problem:** World position doesn't match expected grid coordinate

**Solutions:**

1. Verify `TileSize` matches visual tile size
2. Check `GridOrigin` is correct
3. Ensure using tile CENTER not corner for positioning
4. Verify no floating point precision issues
5. Check elevation is included in Z coordinate

---

### Pathfinding Not Working

**Problem:** Characters can't find path

**Solutions:**

1. Check tiles marked as walkable
2. Verify no occupied tiles blocking path
3. Check elevation differences reasonable
4. Ensure start and goal coordinates valid
5. Verify pathfinding considers obstacles

---

### Events Not Firing

**Problem:** `OnTileChanged` not received

**Solutions:**

1. Verify bound with `AddDynamic` not `AddUniqueDynamic` repeatedly
2. Check if modification happened on server
3. Ensure object still alive when event fires
4. Verify event bound before tile changes
5. Check if grid cleared (unbinds events)

---

## Related Documents

### Architecture

- [[03_Systems/Grid_System]] - System design and patterns
- [[03_Systems/Grid_Visualizer]] - Visual representation
- [[04_Implementation/Common_Patterns]] - Usage patterns

### Related Systems

- [[05_API_Reference/KalkiGridVisualizer_API]] - Visualizer API
- [[05_API_Reference/KalkiPathfinding_API]] - Pathfinding API
- [[05_API_Reference/KalkiCharacter_API]] - Character integration

### Implementation

- [[04_Implementation/Multiplayer_Guidelines]] - Network considerations
- [[04_Implementation/Error_Handling]] - Validation patterns
- [[04_Implementation/Testing_Procedures]] - Testing grid system

---

## Quick Reference

### Essential Functions

```cpp
// Get manager
UKalkiGridManager* Grid = GetWorld()->GetSubsystem<UKalkiGridManager>();

// Create grid (server only)
Grid->CreateGrid(20, 20, 100.0f, FVector::ZeroVector);

// Check if ready
if (Grid->IsGridCreated())

// Validate coordinate
if (Grid->IsValidCoord(Coord))

// Get tile
FKalkiGridTile Tile = Grid->GetTile(Coord);

// Modify tile (server only)
Grid->SetElevation(Coord, 200.0f);
Grid->SetTileWalkable(Coord, false);
Grid->SetTileOccupant(Coord, Actor);

// Spatial queries
TArray<FKalkiGridCoord> Neighbors = Grid->GetNeighbors(Coord);
TArray<FKalkiGridCoord> InRange = Grid->GetTilesInRange(Coord, Range);

// Coordinate conversion
FVector WorldPos = Grid->CoordToWorldPosition(Coord);
FKalkiGridCoord Coord = Grid->WorldPositionToCoord(WorldPos);

// Events
Grid->OnGridCreated.AddDynamic(this, &AMyActor::OnGridCreated);
Grid->OnTileChanged.AddDynamic(this, &AMyActor::OnTileChanged);
```

---

## Document Info

**File Path:** `Kalki_GDD/05_API_Reference/KalkiGridManager_API.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate  
**Note:** Complete API reference for KalkiGridManager subsystem