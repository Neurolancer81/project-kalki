---
tags:
  - kalki/system/grid
  - kalki/architecture
  - kalki/phase-2
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Grid System

## Overview
The Grid System is the foundation for tactical gameplay in Kalki. It provides a square tile-based grid with elevation support, walkability tracking, occupancy management, and spatial queries. The system is designed for multiplayer with server authority and deterministic replication.

---

## Purpose

### Core Problems Solved
1. **Spatial Organization** - Discrete tile-based positioning for tactical gameplay
2. **Elevation Handling** - Support for multi-level terrain (XCOM-style)
3. **Pathfinding Foundation** - Data structure optimized for A* pathfinding
4. **Multiplayer Safety** - Server authority prevents desync
5. **Performance** - Efficient queries for range, neighbors, and paths

### Design Goals
- ✅ **Simple API** - Easy to use, hard to misuse
- ✅ **Multiplayer-Safe** - Server authority, deterministic replication
- ✅ **Performance** - O(1) tile lookups, efficient spatial queries
- ✅ **Extensible** - Easy to add new tile properties
- ✅ **Event-Driven** - Notify systems when grid changes

---

## Architecture

### High-Level Design
```
┌─────────────────────────────────────────────────┐
│           AKalkiLevelManager (Actor)            │
│  - Per-level grid configuration                 │
│  - Spawns GridVisualizer                        │
│  - Replicates config to clients                 │
└────────────┬────────────────────────────────────┘
             │ creates
             ↓
┌─────────────────────────────────────────────────┐
│        UKalkiGridManager (WorldSubsystem)       │
│  - Grid data storage (TMap)                     │
│  - Tile queries and modifications               │
│  - Spatial queries (range, neighbors)           │
│  - Event broadcasting                           │
└────────────┬────────────────────────────────────┘
             │ uses
             ↓
┌─────────────────────────────────────────────────┐
│     Data Structures (KalkiGridTypes.h)         │
│  - FKalkiGridCoord (X, Y)                      │
│  - FKalkiGridTile (properties)                 │
│  - Operators and utilities                      │
└─────────────────────────────────────────────────┘
```

---

## Core Classes

### UKalkiGridManager
**Type:** WorldSubsystem  
**Lifetime:** Per-world  
**Authority:** Server creates/modifies, clients query

**Responsibilities:**
- Store grid data (TMap of tiles)
- Validate coordinates
- Handle tile modifications (server only)
- Provide spatial queries (neighbors, range)
- Convert between grid and world coordinates
- Broadcast events (OnGridCreated, OnTileChanged)

**Key Properties:**
```cpp
TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;  // Sparse storage
int32 GridSizeX;                                    // Grid width
int32 GridSizeY;                                    // Grid height
float TileSize;                                     // World units per tile
FVector GridOrigin;                                 // World space origin
```

---

### AKalkiLevelManager
**Type:** Actor  
**Lifetime:** Per-level  
**Authority:** Replicates to all clients

**Responsibilities:**
- Configure grid per level (size, tile size, origin)
- Create grid on server
- Replicate configuration to clients
- Spawn and manage GridVisualizer
- Provide helper functions for designers

**Replicated Properties:**
```cpp
int32 GridSizeX;           // Replicates to clients
int32 GridSizeY;           // Replicates to clients
float TileSize;            // Replicates to clients
FVector GridOrigin;        // Replicates to clients
```

---

### FKalkiGridCoord
**Type:** Struct (USTRUCT)  
**Purpose:** Represent grid coordinates

**Properties:**
```cpp
int32 X;  // Grid X coordinate (0-based)
int32 Y;  // Grid Y coordinate (0-based)
```

**Features:**
- Equality operators (==, !=)
- Hash function (for use in TMap/TSet)
- ToString() for debugging
- Default constructor (invalid coord: -1, -1)

**Usage:**
```cpp
FKalkiGridCoord Origin(0, 0);
FKalkiGridCoord Target(5, 7);

if (Origin == Target)  // Equality check
{
    // Same tile
}

TMap<FKalkiGridCoord, FKalkiGridTile> Tiles;  // Can be map key
```

---

### FKalkiGridTile
**Type:** Struct (USTRUCT)  
**Purpose:** Store tile properties

**Properties:**
```cpp
FKalkiGridCoord Coordinate;     // Tile position
FVector WorldPosition;          // World space position (center of tile)
float Elevation;                // Height in world units
bool bWalkable;                 // Can characters walk here?
TWeakObjectPtr<AActor> Occupant;  // Actor currently on this tile
```

**Helper Functions:**
```cpp
bool IsOccupied() const;        // Returns true if Occupant is valid
void ClearOccupant();           // Removes occupant
```

---

## Grid Coordinate System

### Coordinate Space
```
Y ↑
  │
  │  (0,2)  (1,2)  (2,2)
  │  
  │  (0,1)  (1,1)  (2,1)
  │  
  │  (0,0)  (1,0)  (2,0)
  └──────────────────────→ X
     Origin
```

**Key Points:**
- Origin (0, 0) at bottom-left
- X increases to the right
- Y increases upward
- Coordinates are integers (discrete tiles)

---

### World Space Mapping

**Grid to World:**
```cpp
FVector WorldPos;
WorldPos.X = GridOrigin.X + (Coord.X * TileSize) + (TileSize * 0.5f);
WorldPos.Y = GridOrigin.Y + (Coord.Y * TileSize) + (TileSize * 0.5f);
WorldPos.Z = GridOrigin.Z + Elevation;
```

**World to Grid:**
```cpp
FVector LocalPos = WorldPos - GridOrigin;
int32 X = FMath::FloorToInt(LocalPos.X / TileSize);
int32 Y = FMath::FloorToInt(LocalPos.Y / TileSize);
FKalkiGridCoord Coord(X, Y);
```

**Notes:**
- World position is tile CENTER
- Z coordinate includes elevation
- Grid is aligned to world axes (no rotation)

---

## Elevation System

### Elevation Model
**Elevation is abstract height, not literal Z position**
```
Elevation: 400
   ┌───┐
   │   │  ← Tile visually at ground level
   └───┘     but has elevation property

Elevation: 200
   ┌───┐
   │   │  ← Also at ground level visually
   └───┘     but different elevation value

Elevation: 0
   ┌───┐
   │   │  ← Ground level
   └───┘
```

**Purpose:**
- Tactical positioning (high ground advantage)
- Movement costs (climbing costs more)
- Line of sight calculations
- Cover mechanics

**Configuration:**
```cpp
float MaxClimbHeight = 200.0f;   // Can climb up to 200 units
float MaxDropHeight = 300.0f;    // Can drop down 300 units
```

### Elevation Rules
1. **Characters can climb** up to MaxClimbHeight
2. **Characters can drop** down to MaxDropHeight
3. **Movement cost increases** with elevation change
4. **Elevation 0** = ground level (default)

---

## Walkability System

### Walkability States
```cpp
bool bWalkable = true;   // Character can move here
bool bWalkable = false;  // Blocked (wall, obstacle, etc.)
```

### Usage
- Set `bWalkable = false` for:
  - Walls
  - Pits
  - Hazards
  - Out of bounds areas
  
- Pathfinding respects walkability
- Visualization shows red for unwalkable tiles

### Dynamic Walkability
**Walkability can change at runtime:**
```cpp
// Block a tile (door closes)
GridManager->SetTile(Coord, Tile);  // Tile.bWalkable = false

// Unblock a tile (door opens)
GridManager->SetTile(Coord, Tile);  // Tile.bWalkable = true
```

---

## Occupancy System

### Occupant Tracking
**Each tile can have one occupant (character)**
```cpp
FKalkiGridTile Tile = GridManager->GetTile(Coord);
Tile.Occupant = Character;  // Character is on this tile
```

### Occupancy Rules
1. **One occupant per tile** (for now)
2. **Occupant is TWeakObjectPtr** (doesn't prevent GC)
3. **Check IsOccupied()** before moving to tile
4. **Update occupancy** when character moves

### Future Enhancements
- Multiple occupants per tile (stacking)
- Occupant size (large creatures take multiple tiles)
- Flying/underground layers

---

## Spatial Queries

### Neighbor Queries

**Get Adjacent Tiles (4-directional):**
```cpp
TArray<FKalkiGridCoord> GetNeighbors(const FKalkiGridCoord& Coord, bool bIncludeDiagonals = false);
```

**Neighbors:**
```
       (X, Y+1)
          ↑
(X-1, Y) ← • → (X+1, Y)
          ↓
       (X, Y-1)
```

**With Diagonals:**
```
(X-1,Y+1)  (X,Y+1)  (X+1,Y+1)
    ↖        ↑        ↗
(X-1,Y)  ←  •  →  (X+1,Y)
    ↙        ↓        ↘
(X-1,Y-1)  (X,Y-1)  (X+1,Y-1)
```

---

### Range Queries

**Get All Tiles in Range (Manhattan Distance):**
```cpp
TArray<FKalkiGridCoord> GetTilesInRange(
    const FKalkiGridCoord& Center, 
    int32 Range, 
    bool bRequireWalkable = false
);
```

**Manhattan Distance:**
```
Distance = |X1 - X2| + |Y1 - Y2|

Range 2 from center:
      2
    2 1 2
  2 1 0 1 2
    2 1 2
      2
```

**Example:**
```cpp
// Get all tiles within 5 tiles
TArray<FKalkiGridCoord> Tiles = GridManager->GetTilesInRange(Origin, 5);

// Get only walkable tiles within 5 tiles
TArray<FKalkiGridCoord> WalkableTiles = GridManager->GetTilesInRange(Origin, 5, true);
```

---

## Event System

### Available Events

**OnGridCreated:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCreated);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnGridCreated OnGridCreated;
```
**Fired:** When grid is fully created and ready

---

**OnTileChanged:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileChanged, const FKalkiGridCoord&, Coord);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnTileChanged OnTileChanged;
```
**Fired:** When tile properties change (elevation, walkability, occupancy)

---

**OnGridCleared:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCleared);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnGridCleared OnGridCleared;
```
**Fired:** When grid is cleared/destroyed

---

### Event Usage

**Binding:**
```cpp
// In BeginPlay
GridManager->OnGridCreated.AddDynamic(this, &AMyActor::OnGridCreated);
GridManager->OnTileChanged.AddDynamic(this, &AMyActor::OnTileChanged);

// Handler functions
void AMyActor::OnGridCreated()
{
    // Grid is ready, initialize systems that need grid
}

void AMyActor::OnTileChanged(const FKalkiGridCoord& Coord)
{
    // Update visuals, recalculate paths, etc.
}
```

**Unbinding:**
```cpp
// In EndPlay
GridManager->OnGridCreated.RemoveDynamic(this, &AMyActor::OnGridCreated);
GridManager->OnTileChanged.RemoveDynamic(this, &AMyActor::OnTileChanged);
```

---

## Multiplayer Architecture

### Server Authority

**Only server can modify grid:**
```cpp
bool UKalkiGridManager::HasGridAuthority() const
{
    UWorld* World = GetWorld();
    return World && (World->GetNetMode() == NM_Standalone || World->GetNetMode() == NM_ListenServer || World->GetNetMode() == NM_DedicatedServer);
}
```

**Modification functions check authority:**
```cpp
bool UKalkiGridManager::SetElevation(const FKalkiGridCoord& Coord, float Elevation)
{
    if (!HasGridAuthority())
    {
        // Client tried to modify - reject
        return false;
    }
    
    // Server modifies
    // ...
}
```

---

### Deterministic Replication

**How it works:**
1. **Server** creates grid (calls CreateGrid)
2. **LevelManager** replicates grid config (size, tile size, origin)
3. **Clients** receive config via replication
4. **Clients** build identical grid locally using same config
5. **Result:** Same grid on all machines, no tile data replication needed

**What Replicates:**
```cpp
// In AKalkiLevelManager:
UPROPERTY(Replicated)
int32 GridSizeX;

UPROPERTY(Replicated)
int32 GridSizeY;

UPROPERTY(Replicated)
float TileSize;

UPROPERTY(Replicated)
FVector GridOrigin;
```

**What Doesn't Replicate:**
- Individual tile data (elevation, walkability)
- Grid contents (TMap)
- Events (fired locally on all machines)

---

### Client Behavior

**Clients can:**
- ✅ Query tile data (GetTile, IsValidCoord)
- ✅ Get neighbors and ranges
- ✅ Convert coordinates
- ✅ Listen to events

**Clients cannot:**
- ❌ Create/clear grid
- ❌ Modify tiles (SetElevation, SetWalkable)
- ❌ Set occupants

**Why this works:**
- Grid structure is deterministic (same config → same grid)
- Modifications come from gameplay (replicated separately)
- Example: Character moves → Character replicates position → Grid occupancy updated on all machines

---

## Performance Characteristics

### Time Complexity

**Grid Operations:**
```
CreateGrid:           O(N) where N = GridSizeX × GridSizeY
GetTile:             O(1) - TMap lookup
SetTile:             O(1) - TMap insert/update
IsValidCoord:        O(1) - bounds check
CoordToWorld:        O(1) - arithmetic
WorldToCoord:        O(1) - arithmetic
```

**Spatial Queries:**
```
GetNeighbors:        O(1) - max 8 neighbors
GetTilesInRange:     O(R²) where R = range
                     For range 5: ~80 tiles checked
                     For range 10: ~300 tiles checked
```

---

### Memory Usage

**20×20 Grid:**
```
TMap overhead:       ~1 KB
Per tile:           ~32 bytes
Total tiles:        400 tiles × 32 bytes = 12.8 KB
Total:              ~14 KB
```

**50×50 Grid:**
```
Total tiles:        2500 tiles × 32 bytes = 80 KB
Total:              ~81 KB
```

**Notes:**
- TMap is sparse (only stores occupied tiles)
- Memory scales linearly with tile count
- Very memory-efficient for large sparse grids

---

## Usage Examples

### Creating a Grid
```cpp
// In AKalkiLevelManager (server only)
void AKalkiLevelManager::CreateGrid()
{
    if (!HasAuthority())
    {
        return;  // Clients don't create
    }
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->CreateGrid(GridSizeX, GridSizeY, TileSize, GridOrigin);
    
    // OnGridCreated event fires automatically
}
```

---

### Querying Tiles
```cpp
// Get tile at coordinate
FKalkiGridCoord Coord(5, 7);
FKalkiGridTile Tile = GridManager->GetTile(Coord);

// Check properties
if (Tile.bWalkable && !Tile.IsOccupied())
{
    // Can move character here
}

// Check elevation
float ElevationDifference = TargetTile.Elevation - CurrentTile.Elevation;
if (ElevationDifference <= MaxClimbHeight)
{
    // Can climb to this tile
}
```

---

### Modifying Tiles
```cpp
// Set elevation (server only)
FKalkiGridCoord Coord(10, 10);
GridManager->SetElevation(Coord, 200.0f);  // OnTileChanged fires

// Set walkability (server only)
GridManager->SetTileWalkable(Coord, false);  // OnTileChanged fires

// Set occupant (server only)
GridManager->SetTileOccupant(Coord, Character);  // OnTileChanged fires
```

---

### Spatial Queries
```cpp
// Get neighbors
FKalkiGridCoord Center(10, 10);
TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(Center, false);  // 4 cardinal
TArray<FKalkiGridCoord> AllNeighbors = GridManager->GetNeighbors(Center, true);  // 8 with diagonals

// Get tiles in range
TArray<FKalkiGridCoord> TilesInRange = GridManager->GetTilesInRange(Center, 5);

// Get only walkable tiles in range
TArray<FKalkiGridCoord> WalkableTiles = GridManager->GetTilesInRange(Center, 5, true);

// Check if coordinate is valid
if (GridManager->IsValidCoord(Coord))
{
    // Coordinate is within grid bounds
}
```

---

### Coordinate Conversion
```cpp
// Grid to World
FKalkiGridCoord GridCoord(5, 7);
FVector WorldPos = GridManager->CoordToWorldPosition(GridCoord);
// WorldPos is at center of tile

// World to Grid
FVector WorldLocation = Character->GetActorLocation();
FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(WorldLocation);
// Coord is the tile containing WorldLocation
```

---

## Testing Commands

**Available console commands for testing:**
```cpp
// Grid creation
CreateTestGrid 20 20

// Elevation
SetTileElevation 10 10 200
CreateTestPlatform 5 5 8 8 200
CreateTestRamp 0 0 10 10 0 500

// Walkability
SetTileWalkable 12 12 false

// Info
PrintGridInfo
PrintTileInfo 5 7
PrintNeighbors 10 10
```

---

## Known Limitations

### Current Limitations
1. **One occupant per tile** - Can't stack characters
2. **Square tiles only** - No hexagonal grids
3. **Fixed grid size** - Can't resize after creation
4. **No rotation** - Grid always axis-aligned
5. **2D pathfinding only** - No flying/underground layers yet

### Future Enhancements
- Multiple occupant support
- Hex grid support
- Dynamic grid resizing
- Rotated grids
- 3D pathfinding (flying, underground)
- Grid serialization (save/load)
- Tile types (water, lava, etc.)
- Terrain effects (slow, damage over time)

---

## Related Documents

### Prerequisites
Read these first to understand grid system:
- [[02_Conventions/Naming_Conventions]] - Understand class naming
- [[02_Conventions/Coding_Standards]] - Code structure
- [[01_Architecture/Project_Structure]] - Where files are located

### Core Dependencies
Grid system depends on:
- [[03_Systems/Logging_System]] - Uses KalkiLog for debugging
- [[04_Implementation/Multiplayer_Guidelines]] - Server authority pattern

### Related Systems
Systems that use the grid:
- [[03_Systems/Grid_Visualization]] - Renders the grid
- [[07_Roadmap/Phase_3_Pathfinding]] - Pathfinding on grid
- [[07_Roadmap/Phase_4_Character_Movement]] - Character grid movement

### API Reference
Detailed API documentation:
- [[05_API_Reference/KalkiGridManager_API]] - Full GridManager API
- [[01_Architecture/Class_Hierarchy]] - Class relationships

### Implementation
How to use grid system:
- [[04_Implementation/Class_Templates]] - Creating grid-based classes
- [[04_Implementation/Testing_Procedures]] - Testing grid functionality
- [[04_Implementation/Common_Patterns]] - Common grid patterns

---

## Document Info
**File Path:** `Kalki_GDD/03_Systems/Grid_System.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate