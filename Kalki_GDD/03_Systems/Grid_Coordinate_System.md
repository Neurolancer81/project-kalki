---
tags:
  - kalki/grid
  - kalki/reference
  - kalki/coordinates
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Grid Coordinate System

## Overview
This document defines the coordinate system used for the tactical grid in Kalki. It explains the FKalkiGridCoord structure, coordinate conventions, conversion formulas, and spatial operations.

---

## Purpose

### Why a Grid Coordinate System?

**Problems without a standard coordinate system:**
1. **Inconsistency** - Different parts of code use different conventions
2. **Bugs** - Coordinate confusion causes position errors
3. **Difficult Debugging** - Hard to trace coordinate issues
4. **Poor Collaboration** - Team members confused about coordinate meaning

**Benefits of standardized coordinates:**
- ✅ Clear, consistent coordinate meaning
- ✅ Easy conversion between grid and world space
- ✅ Simple spatial queries (neighbors, distance)
- ✅ Predictable behavior across all systems
- ✅ Easy debugging and testing

---

## FKalkiGridCoord Structure

### Definition
```cpp
// Public/Grid/KalkiGridTypes.h

USTRUCT(BlueprintType)
struct KALKI_API FKalkiGridCoord
{
    GENERATED_BODY()

    // X coordinate (column)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 X;

    // Y coordinate (row)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 Y;

    // Constructors
    FKalkiGridCoord()
        : X(0)
        , Y(0)
    {
    }

    FKalkiGridCoord(int32 InX, int32 InY)
        : X(InX)
        , Y(InY)
    {
    }

    // Equality operators
    bool operator==(const FKalkiGridCoord& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    bool operator!=(const FKalkiGridCoord& Other) const
    {
        return !(*this == Other);
    }

    // Validation
    bool IsValid() const
    {
        return X >= 0 && Y >= 0;
    }

    // String conversion
    FString ToString() const
    {
        return FString::Printf(TEXT("(%d, %d)"), X, Y);
    }

    // Static invalid coordinate
    static FKalkiGridCoord Invalid()
    {
        return FKalkiGridCoord(-1, -1);
    }
};

// Hash function for use in TMap/TSet
FORCEINLINE uint32 GetTypeHash(const FKalkiGridCoord& Coord)
{
    // Combine X and Y into single hash
    return HashCombine(GetTypeHash(Coord.X), GetTypeHash(Coord.Y));
}
```

---

## Coordinate Convention

### Grid Layout
```
Grid Layout (Top-Down View):

     X →
   ┌─────────────────────────────►
Y  │  (0,0)  (1,0)  (2,0)  (3,0)
↓  │  (0,1)  (1,1)  (2,1)  (3,1)
   │  (0,2)  (1,2)  (2,2)  (3,2)
   │  (0,3)  (1,3)  (2,3)  (3,3)
   ▼

Origin (0,0) is at top-left
X increases to the right (columns)
Y increases downward (rows)
```

**Why This Convention?**
- Matches 2D array indexing: `Array[Y][X]`
- Consistent with most game grids
- Natural for top-down camera view
- Matches UI coordinate systems

---

### World Space Mapping
```
World Space (3D):

            Y (North)
            ↑
            |
            |
            |
            └─────────► X (East)
           /
          /
         ↓ Z (Down)

Grid to World Mapping:
Grid X → World X (East/West)
Grid Y → World Y (North/South)
Elevation → World Z (Up/Down)
```

**Convention:**
- Grid origin (0,0) maps to world origin (or specified origin point)
- Each grid unit = `TileSize` world units (typically 100)
- Grid coordinates map to tile **centers** in world space

---

## Coordinate Operations

### Basic Construction
```cpp
// Create coordinate
FKalkiGridCoord Coord(5, 7);

// Create at origin
FKalkiGridCoord Origin(0, 0);

// Create invalid coordinate
FKalkiGridCoord Invalid = FKalkiGridCoord::Invalid();
FKalkiGridCoord Invalid2(-1, -1);

// Default constructor (0, 0)
FKalkiGridCoord Default;  // (0, 0)
```

---

### Validation
```cpp
// Check if coordinate is valid (non-negative)
FKalkiGridCoord Coord(5, 7);
if (Coord.IsValid())
{
    // X >= 0 and Y >= 0
}

FKalkiGridCoord Invalid(-1, 5);
if (!Invalid.IsValid())
{
    // Invalid: X < 0
}

// Check if in grid bounds
UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
FKalkiGridCoord Coord(10, 10);

if (GridManager->IsValidCoord(Coord))
{
    // Within grid bounds (0 <= X < SizeX, 0 <= Y < SizeY)
}
```

---

### Equality
```cpp
FKalkiGridCoord A(5, 7);
FKalkiGridCoord B(5, 7);
FKalkiGridCoord C(3, 2);

if (A == B)
{
    // True: Same coordinates
}

if (A != C)
{
    // True: Different coordinates
}
```

---

### String Conversion
```cpp
FKalkiGridCoord Coord(10, 15);
FString CoordString = Coord.ToString();
// Output: "(10, 15)"

// Use in logging
KalkiLog::Grid(
    FString::Printf(TEXT("Character at %s"), *Coord.ToString())
);
```

---

## Distance Calculations

### Manhattan Distance

**Definition:** Sum of absolute differences in X and Y

**Formula:**
```cpp
Distance = |X1 - X2| + |Y1 - Y2|
```

**Implementation:**
```cpp
// In KalkiMathLibrary
int32 ManhattanDistance(const FKalkiGridCoord& A, const FKalkiGridCoord& B)
{
    return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
}

// Usage
FKalkiGridCoord A(0, 0);
FKalkiGridCoord B(3, 4);
int32 Distance = UKalkiMathLibrary::ManhattanDistance(A, B);
// Distance = |0-3| + |0-4| = 3 + 4 = 7
```

**Visual Example:**
```
From (0,0) to (3,4) - Manhattan Distance = 7

  0 1 2 3
0 S→→→*
1 ↓   *
2 ↓   *
3 ↓   *
4 ↓→→→E

Path: Right 3, Down 4 = 7 steps
```

**When to Use:**
- Movement range calculations
- Ability range checks
- Pathfinding heuristics
- Turn-based tactical games (like Kalki)

---

### Euclidean Distance

**Definition:** Straight-line distance

**Formula:**
```cpp
Distance = √((X1 - X2)² + (Y1 - Y2)²)
```

**Implementation:**
```cpp
// In KalkiMathLibrary
float EuclideanDistance(const FKalkiGridCoord& A, const FKalkiGridCoord& B)
{
    int32 DX = A.X - B.X;
    int32 DY = A.Y - B.Y;
    return FMath::Sqrt(static_cast<float>(DX * DX + DY * DY));
}

// Usage
FKalkiGridCoord A(0, 0);
FKalkiGridCoord B(3, 4);
float Distance = UKalkiMathLibrary::EuclideanDistance(A, B);
// Distance = √(3² + 4²) = √(9 + 16) = √25 = 5.0
```

**Visual Example:**
```
From (0,0) to (3,4) - Euclidean Distance = 5

  0 1 2 3
0 S   
1   ╲
2     ╲
3       ╲
4         E

Direct diagonal distance = 5 tiles
```

**When to Use:**
- Line of sight checks
- Visual range calculations
- "As the crow flies" distances
- Circular area effects

---

### Chebyshev Distance

**Definition:** Maximum of absolute differences (diagonal movement)

**Formula:**
```cpp
Distance = max(|X1 - X2|, |Y1 - Y2|)
```

**Implementation:**
```cpp
// In KalkiMathLibrary
int32 ChebyshevDistance(const FKalkiGridCoord& A, const FKalkiGridCoord& B)
{
    return FMath::Max(FMath::Abs(A.X - B.X), FMath::Abs(A.Y - B.Y));
}

// Usage
FKalkiGridCoord A(0, 0);
FKalkiGridCoord B(3, 4);
int32 Distance = UKalkiMathLibrary::ChebyshevDistance(A, B);
// Distance = max(|0-3|, |0-4|) = max(3, 4) = 4
```

**Visual Example:**
```
From (0,0) to (3,4) - Chebyshev Distance = 4

  0 1 2 3
0 S→↘
1   ↓ ↘
2     ↓ ↘
3       ↓ 
4         E

Diagonal moves allowed = 4 steps
```

**When to Use:**
- 8-directional movement
- Chess-like movement (king moves)
- Diagonal movement allowed

---

## Neighbor Queries

### Cardinal Neighbors (4-Direction)

**Definition:** Up, Down, Left, Right
```cpp
// Get 4 cardinal neighbors
TArray<FKalkiGridCoord> GetCardinalNeighbors(const FKalkiGridCoord& Coord)
{
    TArray<FKalkiGridCoord> Neighbors;
    
    // Up (Y-1)
    Neighbors.Add(FKalkiGridCoord(Coord.X, Coord.Y - 1));
    
    // Down (Y+1)
    Neighbors.Add(FKalkiGridCoord(Coord.X, Coord.Y + 1));
    
    // Left (X-1)
    Neighbors.Add(FKalkiGridCoord(Coord.X - 1, Coord.Y));
    
    // Right (X+1)
    Neighbors.Add(FKalkiGridCoord(Coord.X + 1, Coord.Y));
    
    return Neighbors;
}
```

**Visual:**
```
      (X, Y-1)
         ↑
(X-1, Y) ← • → (X+1, Y)
         ↓
      (X, Y+1)
```

**Example:**
```cpp
FKalkiGridCoord Center(5, 5);
TArray<FKalkiGridCoord> Neighbors = GetCardinalNeighbors(Center);

// Neighbors:
// (5, 4) - Up
// (5, 6) - Down
// (4, 5) - Left
// (6, 5) - Right
```

---

### All Neighbors (8-Direction)

**Definition:** Cardinals + Diagonals
```cpp
// Get 8 neighbors (including diagonals)
TArray<FKalkiGridCoord> GetAllNeighbors(const FKalkiGridCoord& Coord)
{
    TArray<FKalkiGridCoord> Neighbors;
    
    for (int32 DY = -1; DY <= 1; ++DY)
    {
        for (int32 DX = -1; DX <= 1; ++DX)
        {
            // Skip center
            if (DX == 0 && DY == 0)
            {
                continue;
            }
            
            Neighbors.Add(FKalkiGridCoord(Coord.X + DX, Coord.Y + DY));
        }
    }
    
    return Neighbors;
}
```

**Visual:**
```
(X-1,Y-1)  (X,Y-1)  (X+1,Y-1)
    ↖        ↑        ↗
(X-1,Y)  ←   •   →  (X+1,Y)
    ↙        ↓        ↘
(X-1,Y+1)  (X,Y+1)  (X+1,Y+1)
```

**Example:**
```cpp
FKalkiGridCoord Center(5, 5);
TArray<FKalkiGridCoord> Neighbors = GetAllNeighbors(Center);

// Neighbors (8 total):
// (4, 4), (5, 4), (6, 4),  // Top row
// (4, 5),         (6, 5),  // Middle row
// (4, 6), (5, 6), (6, 6)   // Bottom row
```

---

## Range Queries

### Tiles in Range (Manhattan)
```cpp
// Get all tiles within Manhattan distance
TArray<FKalkiGridCoord> GetTilesInRange(
    const FKalkiGridCoord& Center,
    int32 Range
)
{
    TArray<FKalkiGridCoord> Tiles;
    
    for (int32 DY = -Range; DY <= Range; ++DY)
    {
        for (int32 DX = -Range; DX <= Range; ++DX)
        {
            // Check Manhattan distance
            if (FMath::Abs(DX) + FMath::Abs(DY) <= Range)
            {
                Tiles.Add(FKalkiGridCoord(Center.X + DX, Center.Y + DY));
            }
        }
    }
    
    return Tiles;
}
```

**Visual (Range 2):**
```
      2
    2 1 2
  2 1 0 1 2
    2 1 2
      2

Numbers show Manhattan distance from center
```

**Example:**
```cpp
FKalkiGridCoord Center(10, 10);
TArray<FKalkiGridCoord> InRange = GetTilesInRange(Center, 2);

// Returns 13 tiles:
// Distance 0: (10, 10)
// Distance 1: (9,10), (11,10), (10,9), (10,11)
// Distance 2: (8,10), (12,10), (10,8), (10,12), (9,9), (11,9), (9,11), (11,11)
```

---

### Tiles in Range (Euclidean)
```cpp
// Get all tiles within Euclidean distance
TArray<FKalkiGridCoord> GetTilesInRangeEuclidean(
    const FKalkiGridCoord& Center,
    float Range
)
{
    TArray<FKalkiGridCoord> Tiles;
    
    int32 IntRange = FMath::CeilToInt(Range);
    
    for (int32 DY = -IntRange; DY <= IntRange; ++DY)
    {
        for (int32 DX = -IntRange; DX <= IntRange; ++DX)
        {
            // Check Euclidean distance
            float Distance = FMath::Sqrt(static_cast<float>(DX * DX + DY * DY));
            if (Distance <= Range)
            {
                Tiles.Add(FKalkiGridCoord(Center.X + DX, Center.Y + DY));
            }
        }
    }
    
    return Tiles;
}
```

**Visual (Range 2.0):**
```
      *
    * * *
  * * • * *
    * * *
      *

Circular area of effect
```

---

## Coordinate Conversion

### Grid to World Position

**Formula:**
```cpp
WorldPos.X = Origin.X + (GridCoord.X * TileSize) + (TileSize * 0.5f)
WorldPos.Y = Origin.Y + (GridCoord.Y * TileSize) + (TileSize * 0.5f)
WorldPos.Z = Origin.Z + Elevation
```

**Implementation:**
```cpp
FVector CoordToWorldPosition(const FKalkiGridCoord& Coord) const
{
    FVector WorldPos;
    
    // Calculate center of tile
    WorldPos.X = GridOrigin.X + (Coord.X * TileSize) + (TileSize * 0.5f);
    WorldPos.Y = GridOrigin.Y + (Coord.Y * TileSize) + (TileSize * 0.5f);
    
    // Get elevation from tile data
    FKalkiGridTile Tile = GetTile(Coord);
    WorldPos.Z = GridOrigin.Z + Tile.Elevation;
    
    return WorldPos;
}
```

**Example:**
```cpp
// Grid setup
FVector GridOrigin = FVector(0, 0, 0);
float TileSize = 100.0f;

// Convert grid coordinate
FKalkiGridCoord Coord(5, 7);
FVector WorldPos = GridManager->CoordToWorldPosition(Coord);

// WorldPos = (0 + (5 * 100) + 50, 0 + (7 * 100) + 50, 0)
//          = (550, 750, 0)
//          = Center of tile at (5, 7)
```

**Why add 0.5 * TileSize?**
- Returns center of tile, not corner
- Consistent with character positioning
- Better for visualization

---

### World to Grid Position

**Formula:**
```cpp
GridCoord.X = floor((WorldPos.X - Origin.X) / TileSize)
GridCoord.Y = floor((WorldPos.Y - Origin.Y) / TileSize)
```

**Implementation:**
```cpp
FKalkiGridCoord WorldPositionToCoord(const FVector& WorldPosition) const
{
    FVector LocalPos = WorldPosition - GridOrigin;
    
    FKalkiGridCoord Coord;
    Coord.X = FMath::FloorToInt(LocalPos.X / TileSize);
    Coord.Y = FMath::FloorToInt(LocalPos.Y / TileSize);
    
    return Coord;
}
```

**Example:**
```cpp
// Grid setup
FVector GridOrigin = FVector(0, 0, 0);
float TileSize = 100.0f;

// World position
FVector WorldPos(550, 750, 0);
FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(WorldPos);

// Coord.X = floor((550 - 0) / 100) = floor(5.5) = 5
// Coord.Y = floor((750 - 0) / 100) = floor(7.5) = 7
// Result: (5, 7)
```

**Why use floor?**
- Any position within tile returns same coordinate
- Consistent rounding (always down)
- Matches array indexing convention

---

## Line Drawing (Bresenham)

### Getting Tiles in Line
```cpp
// Get all tiles between two coordinates (Bresenham's line algorithm)
TArray<FKalkiGridCoord> GetLineCoords(
    const FKalkiGridCoord& Start,
    const FKalkiGridCoord& End
)
{
    TArray<FKalkiGridCoord> Line;
    
    int32 X0 = Start.X;
    int32 Y0 = Start.Y;
    int32 X1 = End.X;
    int32 Y1 = End.Y;
    
    int32 DX = FMath::Abs(X1 - X0);
    int32 DY = FMath::Abs(Y1 - Y0);
    
    int32 SX = (X0 < X1) ? 1 : -1;
    int32 SY = (Y0 < Y1) ? 1 : -1;
    
    int32 Err = DX - DY;
    
    while (true)
    {
        Line.Add(FKalkiGridCoord(X0, Y0));
        
        if (X0 == X1 && Y0 == Y1)
        {
            break;
        }
        
        int32 E2 = 2 * Err;
        
        if (E2 > -DY)
        {
            Err -= DY;
            X0 += SX;
        }
        
        if (E2 < DX)
        {
            Err += DX;
            Y0 += SY;
        }
    }
    
    return Line;
}
```

**Usage:**
```cpp
// Get line from (0,0) to (5,3)
FKalkiGridCoord Start(0, 0);
FKalkiGridCoord End(5, 3);
TArray<FKalkiGridCoord> Line = GetLineCoords(Start, End);

// Result (approximate):
// (0,0), (1,1), (2,1), (3,2), (4,2), (5,3)
```

**Use Cases:**
- Line of sight checks
- Projectile paths
- Ray casting
- Area effects along line

---

## Common Usage Patterns

### Pattern: Iterate Grid
```cpp
// Iterate all tiles in grid
void ProcessAllTiles()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    int32 SizeX = GridManager->GetGridSizeX();
    int32 SizeY = GridManager->GetGridSizeY();
    
    for (int32 Y = 0; Y < SizeY; ++Y)
    {
        for (int32 X = 0; X < SizeX; ++X)
        {
            FKalkiGridCoord Coord(X, Y);
            FKalkiGridTile Tile = GridManager->GetTile(Coord);
            
            // Process tile
            ProcessTile(Coord, Tile);
        }
    }
}
```

---

### Pattern: Find Nearest Tile
```cpp
// Find nearest tile to world position
FKalkiGridCoord FindNearestTile(const FVector& WorldPos)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Convert to grid coordinate
    FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(WorldPos);
    
    // Clamp to grid bounds
    Coord.X = FMath::Clamp(Coord.X, 0, GridManager->GetGridSizeX() - 1);
    Coord.Y = FMath::Clamp(Coord.Y, 0, GridManager->GetGridSizeY() - 1);
    
    return Coord;
}
```

---

### Pattern: Check if Tiles Adjacent
```cpp
// Check if two tiles are adjacent (4-direction)
bool AreTilesAdjacent(const FKalkiGridCoord& A, const FKalkiGridCoord& B)
{
    int32 Distance = UKalkiMathLibrary::ManhattanDistance(A, B);
    return Distance == 1;
}

// Check if diagonally adjacent (8-direction)
bool AreTilesDiagonallyAdjacent(const FKalkiGridCoord& A, const FKalkiGridCoord& B)
{
    int32 Distance = UKalkiMathLibrary::ChebyshevDistance(A, B);
    return Distance == 1;
}
```

---

### Pattern: Get Direction Vector
```cpp
// Get direction from one coord to another
FKalkiGridCoord GetDirection(const FKalkiGridCoord& From, const FKalkiGridCoord& To)
{
    FKalkiGridCoord Direction;
    
    // Normalize to -1, 0, or 1
    Direction.X = FMath::Clamp(To.X - From.X, -1, 1);
    Direction.Y = FMath::Clamp(To.Y - From.Y, -1, 1);
    
    return Direction;
}

// Example
FKalkiGridCoord From(5, 5);
FKalkiGridCoord To(8, 3);
FKalkiGridCoord Dir = GetDirection(From, To);
// Dir = (1, -1) - Southeast direction
```

---

## Advanced Operations

### Rotation
```cpp
// Rotate coordinate 90 degrees clockwise around origin
FKalkiGridCoord RotateClockwise(const FKalkiGridCoord& Coord)
{
    return FKalkiGridCoord(Coord.Y, -Coord.X);
}

// Rotate 90 degrees counter-clockwise
FKalkiGridCoord RotateCounterClockwise(const FKalkiGridCoord& Coord)
{
    return FKalkiGridCoord(-Coord.Y, Coord.X);
}

// Rotate around specific point
FKalkiGridCoord RotateAroundPoint(
    const FKalkiGridCoord& Coord,
    const FKalkiGridCoord& Center,
    int32 Rotation  // 0=0°, 1=90°, 2=180°, 3=270°
)
{
    // Translate to origin
    FKalkiGridCoord Relative(Coord.X - Center.X, Coord.Y - Center.Y);
    
    // Rotate
    for (int32 i = 0; i < Rotation; ++i)
    {
        Relative = RotateClockwise(Relative);
    }
    
    // Translate back
    return FKalkiGridCoord(Relative.X + Center.X, Relative.Y + Center.Y);
}
```

---

### Mirroring
```cpp
// Mirror horizontally
FKalkiGridCoord MirrorHorizontal(const FKalkiGridCoord& Coord, int32 GridWidth)
{
    return FKalkiGridCoord(GridWidth - 1 - Coord.X, Coord.Y);
}

// Mirror vertically
FKalkiGridCoord MirrorVertical(const FKalkiGridCoord& Coord, int32 GridHeight)
{
    return FKalkiGridCoord(Coord.X, GridHeight - 1 - Coord.Y);
}
```

---

## Debugging

### Visualize Coordinates
```cpp
// Draw debug coordinate in world
void DrawDebugCoordinate(const FKalkiGridCoord& Coord)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    FVector WorldPos = GridManager->CoordToWorldPosition(Coord);
    
    // Draw sphere at coordinate
    DrawDebugSphere(
        GetWorld(),
        WorldPos,
        25.0f,
        12,
        FColor::Green,
        false,
        1.0f
    );
    
    // Draw coordinate text
    DrawDebugString(
        GetWorld(),
        WorldPos + FVector(0, 0, 50),
        Coord.ToString(),
        nullptr,
        FColor::White,
        1.0f
    );
}
```

---

### Logging Coordinates
```cpp
// Log coordinate with context
void LogCoordinate(const FString& Context, const FKalkiGridCoord& Coord)
{
    KalkiLog::Grid(
        FString::Printf(TEXT("%s: %s"), *Context, *Coord.ToString())
    );
}

// Usage
LogCoordinate(TEXT("Character Position"), Character->GetGridPosition());
// Output: "Character Position: (10, 15)"
```

---

## Performance Considerations

### Coordinate Operations Cost

**Operation Complexity:**

| Operation | Time | Notes |
|-----------|------|-------|
| Construction | O(1) | Instant |
| Equality check | O(1) | Instant |
| Manhattan distance | O(1) | Instant |
| Euclidean distance | O(1) | Involves sqrt (slower) |
| Hash calculation | O(1) | Fast |
| ToString() | O(1) | String allocation |
| GetNeighbors(4) | O(1) | Fixed 4 operations |
| GetNeighbors(8) | O(1) | Fixed 8 operations |
| GetTilesInRange(R) | O(R²) | Scales with range |
| GetLineCoords() | O(D) | D = distance |

**Optimization Tips:**
1. Cache coordinate calculations when possible
2. Use Manhattan distance over Euclidean when appropriate
3. Limit range queries to necessary radius
4. Pre-calculate common neighbor sets

---

## Testing

### Unit Test Example
```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridCoordTest,
    "Kalki.Grid.GridCoord.Operations",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridCoordTest::RunTest(const FString& Parameters)
{
    // Test construction
    FKalkiGridCoord Coord(5, 7);
    TestEqual(TEXT("X coordinate"), Coord.X, 5);
    TestEqual(TEXT("Y coordinate"), Coord.Y, 7);

    // Test equality
    FKalkiGridCoord Same(5, 7);
    FKalkiGridCoord Different(3, 2);
    TestTrue(TEXT("Equal coordinates"), Coord == Same);
    TestTrue(TEXT("Different coordinates"), Coord != Different);

    // Test validation
    FKalkiGridCoord Valid(0, 0);
    FKalkiGridCoord Invalid(-1, 5);
    TestTrue(TEXT("Valid coordinate"), Valid.IsValid());
    TestFalse(TEXT("Invalid coordinate"), Invalid.IsValid());

    // Test Manhattan distance
    FKalkiGridCoord A(0, 0);
    FKalkiGridCoord B(3, 4);
    int32 Distance = UKalkiMathLibrary::ManhattanDistance(A, B);
    TestEqual(TEXT("Manhattan distance"), Distance, 7);

    // Test hash
    uint32 Hash1 = GetTypeHash(Coord);
    uint32 Hash2 = GetTypeHash(Same);
    TestEqual(TEXT("Equal hashes"), Hash1, Hash2);

    return true;
}
```

---

## Common Mistakes

### Mistake 1: Wrong Coordinate Order
```cpp
// ❌ BAD - Confusing X and Y
for (int32 X = 0; X < SizeX; ++X)
{
    for (int32 Y = 0; Y < SizeY; ++Y)
    {    
        // Inner loop should be X, outer should be Y for cache efficiency!
        FKalkiGridCoord Coord(X, Y);
    }
}

// ✅ GOOD - Correct order for cache efficiency
for (int32 Y = 0; Y < SizeY; ++Y)
{
    for (int32 X = 0; X < SizeX; ++X)
    {
        // Row-major order matches memory layout
        FKalkiGridCoord Coord(X, Y);
    }
}
```

---

### Mistake 2: Not Validating Coordinates

```cpp
// ❌ BAD - No validation
FKalkiGridCoord Coord = CalculateSomeCoord();
FKalkiGridTile Tile = GridManager->GetTile(Coord);
// Might be out of bounds!

// ✅ GOOD - Validate first
FKalkiGridCoord Coord = CalculateSomeCoord();
if (GridManager->IsValidCoord(Coord))
{
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
}
else
{
    KalkiLog::Grid(
        FString::Printf(TEXT("Invalid coordinate: %s"), *Coord.ToString()),
        EKalkiLogSeverity::Warning
    );
}
```

---

### Mistake 3: Using Wrong Distance Metric

```cpp
// ❌ BAD - Euclidean for grid movement
float Distance = UKalkiMathLibrary::EuclideanDistance(Start, Goal);
if (Distance <= MovementRange)
{
    // Wrong! Character can't move diagonally in one turn
}

// ✅ GOOD - Manhattan for grid movement
int32 Distance = UKalkiMathLibrary::ManhattanDistance(Start, Goal);
if (Distance <= MovementRange)
{
    // Correct! Matches actual grid movement
}
```

---

### Mistake 4: Forgetting Tile Center Offset

```cpp
// ❌ BAD - Position at corner of tile
FVector WorldPos;
WorldPos.X = GridOrigin.X + (Coord.X * TileSize);
WorldPos.Y = GridOrigin.Y + (Coord.Y * TileSize);
// Character positioned at corner, looks wrong!

// ✅ GOOD - Position at center of tile
FVector WorldPos;
WorldPos.X = GridOrigin.X + (Coord.X * TileSize) + (TileSize * 0.5f);
WorldPos.Y = GridOrigin.Y + (Coord.Y * TileSize) + (TileSize * 0.5f);
// Character centered on tile
```

---

### Mistake 5: Invalid Coordinate as Default

```cpp
// ❌ BAD - Using (0,0) as "invalid"
class AKalkiCharacter
{
    FKalkiGridCoord GridPosition = FKalkiGridCoord(0, 0);
    // (0,0) is a valid position! Ambiguous!
};

// ✅ GOOD - Use explicit invalid marker
class AKalkiCharacter
{
    FKalkiGridCoord GridPosition = FKalkiGridCoord::Invalid();  // (-1, -1)
    
    bool HasValidGridPosition() const
    {
        return GridPosition.IsValid();
    }
};
```

---

## Troubleshooting

### Coordinates Don't Match Visuals

**Problem:** Grid coordinates don't align with visual tiles

**Solutions:**

1. Verify TileSize matches visual tile size
2. Check GridOrigin is correct
3. Ensure using tile center offset (+ 0.5 * TileSize)
4. Verify coordinate conversion formulas
5. Check for floating point precision issues

---

### Distance Calculations Wrong

**Problem:** Distance queries returning unexpected results

**Solutions:**

1. Verify using correct distance metric (Manhattan vs Euclidean)
2. Check coordinate order (X, Y not Y, X)
3. Ensure coordinates are valid before calculating
4. Test with simple known cases first
5. Log intermediate values for debugging

---

### Neighbor Queries Missing Tiles

**Problem:** GetNeighbors returns fewer than expected

**Solutions:**

1. Check if on edge/corner of grid (naturally fewer neighbors)
2. Verify IsValidCoord checks working
3. Ensure not filtering out valid neighbors
4. Check for off-by-one errors in loops
5. Test with center tile first (should return 4 or 8)

---

## Related Documents

### Core Systems

- [[03_Systems/Grid_System]] - Grid architecture
- [[05_API_Reference/KalkiGridManager_API]] - Grid manager API
- [[05_API_Reference/KalkiGridTile_API]] - Tile data structure

### Implementation

- [[04_Implementation/Common_Patterns]] - Coordinate usage patterns
- [[04_Implementation/Testing_Procedures]] - Testing coordinates
- [[04_Implementation/Error_Handling]] - Coordinate validation

### Reference

- [[05_API_Reference/KalkiMathLibrary_API]] - Distance calculations
- [[03_Systems/Pathfinding_System]] - Using coordinates for pathfinding

---

## Quick Reference

### Construction

```cpp
// Create coordinate
FKalkiGridCoord Coord(5, 7);

// Default (0, 0)
FKalkiGridCoord Origin;

// Invalid marker
FKalkiGridCoord Invalid = FKalkiGridCoord::Invalid();  // (-1, -1)
```

---

### Validation

```cpp
// Check non-negative
if (Coord.IsValid())

// Check in grid bounds
if (GridManager->IsValidCoord(Coord))
```

---

### Distance

```cpp
// Manhattan (grid movement)
int32 Dist = UKalkiMathLibrary::ManhattanDistance(A, B);

// Euclidean (straight line)
float Dist = UKalkiMathLibrary::EuclideanDistance(A, B);

// Chebyshev (diagonal movement)
int32 Dist = UKalkiMathLibrary::ChebyshevDistance(A, B);
```

---

### Neighbors

```cpp
// 4 cardinal directions
TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(Coord, false);

// 8 directions (with diagonals)
TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(Coord, true);
```

---

### Conversion

```cpp
// Grid to World
FVector WorldPos = GridManager->CoordToWorldPosition(Coord);

// World to Grid
FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(WorldPos);
```

---

### Range Queries

```cpp
// Get all tiles within range
TArray<FKalkiGridCoord> Tiles = GridManager->GetTilesInRange(Center, Range);
```

---

### Debugging

```cpp
// String representation
FString Str = Coord.ToString();  // "(5, 7)"

// Logging
KalkiLog::Grid(
    FString::Printf(TEXT("Position: %s"), *Coord.ToString())
);
```

---

## Visual Guide

### Coordinate System Summary

```
Grid Space:              World Space:
                        
Y                       Y (North)
↓                       ↑
  0  1  2  3            |
0 •                     |
1    •                  |
2       •               +────► X (East)
3          •
   ────► X              

Origin: (0,0)           Origin: Variable
Top-Left                Center of (0,0) tile
Row-major order         3D space
```

---

### Distance Comparison

```
From (0,0) to (3,4):

Manhattan:    7 steps   (3 + 4)
Euclidean:    5.0 units (√(3² + 4²))
Chebyshev:    4 steps   (max(3, 4))

  0 1 2 3
0 S→→→*     Manhattan: Follow grid
1 ↓   *     Euclidean: Straight line
2 ↓   *     Chebyshev: Diagonal allowed
3 ↓   *
4 ↓→→→E
```

---

### Neighbor Patterns

```
Cardinal (4):           All Neighbors (8):

    ↑                       ↖ ↑ ↗
  ← • →                   ← • →
    ↓                       ↙ ↓ ↘
```

---

## Code Templates

### Template: Iterate Grid Region

```cpp
void ProcessRegion(
    const FKalkiGridCoord& TopLeft,
    const FKalkiGridCoord& BottomRight
)
{
    for (int32 Y = TopLeft.Y; Y <= BottomRight.Y; ++Y)
    {
        for (int32 X = TopLeft.X; X <= BottomRight.X; ++X)
        {
            FKalkiGridCoord Coord(X, Y);
            
            if (GridManager->IsValidCoord(Coord))
            {
                ProcessTile(Coord);
            }
        }
    }
}
```

---

### Template: Find Closest Tile

```cpp
FKalkiGridCoord FindClosestTile(
    const FKalkiGridCoord& From,
    const TArray<FKalkiGridCoord>& Candidates
)
{
    if (Candidates.Num() == 0)
    {
        return FKalkiGridCoord::Invalid();
    }

    FKalkiGridCoord Closest = Candidates[0];
    int32 MinDistance = UKalkiMathLibrary::ManhattanDistance(From, Closest);

    for (int32 i = 1; i < Candidates.Num(); ++i)
    {
        int32 Distance = UKalkiMathLibrary::ManhattanDistance(From, Candidates[i]);
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            Closest = Candidates[i];
        }
    }

    return Closest;
}
```

---

### Template: Check Line of Sight

```cpp
bool HasLineOfSight(
    const FKalkiGridCoord& From,
    const FKalkiGridCoord& To
)
{
    // Get all tiles in line
    TArray<FKalkiGridCoord> Line = GetLineCoords(From, To);

    // Check each tile (skip start and end)
    for (int32 i = 1; i < Line.Num() - 1; ++i)
    {
        FKalkiGridTile Tile = GridManager->GetTile(Line[i]);
        
        // Check if tile blocks LOS
        if (!Tile.bWalkable)
        {
            return false;
        }
    }

    return true;
}
```

---

### Template: Get Random Coordinate

```cpp
FKalkiGridCoord GetRandomCoord()
{
    int32 X = FMath::RandRange(0, GridManager->GetGridSizeX() - 1);
    int32 Y = FMath::RandRange(0, GridManager->GetGridSizeY() - 1);
    
    return FKalkiGridCoord(X, Y);
}

FKalkiGridCoord GetRandomWalkableCoord()
{
    const int32 MaxAttempts = 100;
    
    for (int32 i = 0; i < MaxAttempts; ++i)
    {
        FKalkiGridCoord Coord = GetRandomCoord();
        FKalkiGridTile Tile = GridManager->GetTile(Coord);
        
        if (Tile.bWalkable && !Tile.IsOccupied())
        {
            return Coord;
        }
    }

    // Fallback
    KalkiLog::Grid(TEXT("Failed to find random walkable coord"), EKalkiLogSeverity::Warning);
    return FKalkiGridCoord::Invalid();
}
```

---

## Best Practices

### ✅ Do This:

1. **Always validate coordinates** before use
2. **Use appropriate distance metric** for your needs
3. **Cache coordinate calculations** when possible
4. **Use FKalkiGridCoord::Invalid()** for invalid markers
5. **Check IsValidCoord()** before accessing tiles
6. **Use meaningful variable names** (From, To, Center)
7. **Log coordinates** in error messages for debugging
8. **Test edge cases** (corners, boundaries)

---

### ❌ Don't Do This:

1. **Don't assume (0,0) is invalid** - it's a valid position
2. **Don't confuse X and Y** order
3. **Don't forget tile center offset** in conversion
4. **Don't use Euclidean for grid movement**
5. **Don't iterate inefficiently** (Y then X for cache)
6. **Don't skip validation** "it should be valid"
7. **Don't hardcode grid size** - use GridManager functions
8. **Don't mix distance metrics** in same calculation

---

## Example Use Cases

### Use Case 1: Character Movement

```cpp
void AKalkiCharacter::MoveTo(const FKalkiGridCoord& Target)
{
    // Validate target
    if (!GridManager->IsValidCoord(Target))
    {
        KalkiLog::Movement(
            FString::Printf(TEXT("Invalid target: %s"), *Target.ToString()),
            EKalkiLogSeverity::Error,
            this
        );
        return;
    }

    // Check distance
    int32 Distance = UKalkiMathLibrary::ManhattanDistance(GridPosition, Target);
    if (Distance > MovementRange)
    {
        KalkiLog::Movement(TEXT("Target out of range"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Move
    GridPosition = Target;
    
    // Update world position
    FVector WorldPos = GridManager->CoordToWorldPosition(Target);
    SetActorLocation(WorldPos);
}
```

---

### Use Case 2: Ability Range Check

```cpp
bool UKalkiAbility::IsTargetInRange(const FKalkiGridCoord& Target) const
{
    AKalkiCharacter* Caster = GetOwningCharacter();
    FKalkiGridCoord CasterPos = Caster->GetGridPosition();

    // Use Manhattan distance for range
    int32 Distance = UKalkiMathLibrary::ManhattanDistance(CasterPos, Target);
    
    return Distance <= AbilityRange;
}

TArray<FKalkiGridCoord> UKalkiAbility::GetValidTargets() const
{
    AKalkiCharacter* Caster = GetOwningCharacter();
    FKalkiGridCoord CasterPos = Caster->GetGridPosition();

    // Get all tiles in range
    TArray<FKalkiGridCoord> InRange = GridManager->GetTilesInRange(
        CasterPos,
        AbilityRange
    );

    // Filter by targeting rules
    TArray<FKalkiGridCoord> ValidTargets;
    for (const FKalkiGridCoord& Coord : InRange)
    {
        if (IsValidTarget(Coord))
        {
            ValidTargets.Add(Coord);
        }
    }

    return ValidTargets;
}
```

---

### Use Case 3: AI Target Selection

```cpp
FKalkiGridCoord AKalkiAIController::SelectBestTarget()
{
    TArray<AKalkiCharacter*> EnemyCharacters = GetEnemyCharacters();
    
    if (EnemyCharacters.Num() == 0)
    {
        return FKalkiGridCoord::Invalid();
    }

    // Find closest enemy
    FKalkiGridCoord MyPos = GetCharacter()->GetGridPosition();
    AKalkiCharacter* ClosestEnemy = nullptr;
    int32 ClosestDistance = INT_MAX;

    for (AKalkiCharacter* Enemy : EnemyCharacters)
    {
        FKalkiGridCoord EnemyPos = Enemy->GetGridPosition();
        int32 Distance = UKalkiMathLibrary::ManhattanDistance(MyPos, EnemyPos);

        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ClosestEnemy = Enemy;
        }
    }

    return ClosestEnemy ? ClosestEnemy->GetGridPosition() : FKalkiGridCoord::Invalid();
}
```

---

## Document Info

**File Path:** `Kalki_GDD/03_Systems/Grid_Coordinate_System.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Beginner  
**Note:** Complete reference for grid coordinate system, operations, and conversions