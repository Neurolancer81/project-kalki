---
tags:
  - kalki/convention
  - kalki/coding-standards
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
---

# Coding Standards

## Overview
This document defines the coding standards for the Kalki project. These standards ensure consistency, readability, and maintainability across the codebase. **All code must follow these standards.**

---

## File Structure

### Header File (.h) Structure
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassName.h

#pragma once

// Engine includes (CoreMinimal first)
#include "CoreMinimal.h"

// Engine includes (specific)
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"

// Project includes (alphabetical)
#include "Grid/KalkiGridTypes.h"
#include "Logging/KalkiLog.h"

// Generated include (ALWAYS LAST)
#include "KalkiClassName.generated.h"

// Forward declarations (before class)
class UKalkiGridManager;
class AKalkiCharacter;

/**
 * Class Name
 * Brief description of what this class does
 * 
 * Additional context if needed:
 * - Multiplayer considerations
 * - Performance notes
 * - Usage guidelines
 */
UCLASS()
class KALKI_API UKalkiClassName : public UParentClass
{
    GENERATED_BODY()

public:
    // ========================================
    // PUBLIC INTERFACE
    // ========================================
    
    // Constructor
    UKalkiClassName();
    
    // Lifecycle functions
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Public API functions
    UFUNCTION(BlueprintCallable, Category = "Kalki|System")
    void PublicFunction();

protected:
    // ========================================
    // PROTECTED MEMBERS
    // ========================================
    
    // Properties
    UPROPERTY(EditAnywhere, Category = "Kalki|Settings")
    int32 SomeValue;
    
    // Helper functions
    void ProtectedHelperFunction();

private:
    // ========================================
    // PRIVATE MEMBERS
    // ========================================
    
    // Internal state
    bool bInternalFlag;
    
    // Internal helpers
    void InternalFunction();
};
```

---

### Implementation File (.cpp) Structure
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Category/KalkiClassName.cpp

#include "Category/KalkiClassName.h"

// Additional includes (alphabetical)
#include "Grid/KalkiGridManager.h"
#include "Logging/KalkiLog.h"
#include "Engine/World.h"

// Constructor
UKalkiClassName::UKalkiClassName()
{
    // Initialize members
    SomeValue = 0;
    bInternalFlag = false;
}

// Lifecycle functions
void UKalkiClassName::BeginPlay()
{
    Super::BeginPlay();
    
    // BeginPlay logic
}

void UKalkiClassName::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Tick logic
}

// Public functions
void UKalkiClassName::PublicFunction()
{
    // Implementation
}

// Protected functions
void UKalkiClassName::ProtectedHelperFunction()
{
    // Implementation
}

// Private functions
void UKalkiClassName::InternalFunction()
{
    // Implementation
}
```

---

## Code Formatting

### Indentation
- **Use TABS** (Unreal Engine standard)
- Tab width: 4 spaces equivalent
- Never mix tabs and spaces

### Braces
**Always use braces, even for single-line statements**

**✅ Correct:**
```cpp
if (bCondition)
{
    DoSomething();
}
```

**❌ Incorrect:**
```cpp
if (bCondition)
    DoSomething(); // No braces
```

### Brace Placement
**Opening brace on same line for functions, new line for control flow**

**Functions:**
```cpp
void MyFunction()
{
    // Function body
}
```

**Control Flow:**
```cpp
if (bCondition)
{
    // Body
}
else
{
    // Body
}

for (int32 i = 0; i < Count; ++i)
{
    // Loop body
}
```

### Spacing
**Space after keywords, no space after function names**

**✅ Correct:**
```cpp
if (bValue)
for (int32 i = 0; i < Max; ++i)
while (bRunning)
MyFunction(Param1, Param2);
```

**❌ Incorrect:**
```cpp
if(bValue)  // No space after 'if'
MyFunction (Param);  // Space before parenthesis
```

**Space around operators:**
```cpp
int32 Result = A + B;
bool bCheck = (Value >= Min) && (Value <= Max);
```

---

## Comments

### File Headers
**Every file must have copyright header**
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassName.h
```

### Class Documentation
**Use Doxygen-style comments for classes**
```cpp
/**
 * Grid Manager
 * Manages the tactical grid system including tile data,
 * elevation, walkability, and spatial queries.
 * 
 * MULTIPLAYER:
 * - Server authority for modifications
 * - Deterministic replication via config
 * 
 * PERFORMANCE:
 * - TMap-based storage for sparse grids
 * - O(1) tile lookups
 */
UCLASS()
class KALKI_API UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()
};
```

### Function Documentation
**Document public and protected functions**
```cpp
/**
 * Get all tiles within range using Manhattan distance
 * @param Center - Origin coordinate
 * @param Range - Maximum distance in tiles
 * @param bRequireWalkable - Only return walkable tiles if true
 * @return Array of coordinates within range
 */
UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
TArray<FKalkiGridCoord> GetTilesInRange(
    const FKalkiGridCoord& Center, 
    int32 Range, 
    bool bRequireWalkable = false
) const;
```

### Inline Comments
**Explain WHY, not WHAT**

**✅ Good:**
```cpp
// Offset by 1 unit to prevent z-fighting with ground
WorldPos.Z += TileZOffset;

// Only server can modify grid in multiplayer
if (!HasGridAuthority())
{
    return false;
}
```

**❌ Bad:**
```cpp
// Add 1 to Z
WorldPos.Z += 1.0f;  // What, not why

// Check authority
if (!HasAuthority())  // Obvious from code
```

### Section Comments
**Use section dividers for organization**
```cpp
// ========================================
// GRID CREATION
// ========================================

void CreateGrid();
void ClearGrid();

// ========================================
// TILE QUERIES
// ========================================

FKalkiGridTile GetTile(const FKalkiGridCoord& Coord) const;
bool IsValidCoord(const FKalkiGridCoord& Coord) const;
```

### TODO Comments
**Mark incomplete work clearly**
```cpp
// TODO: Implement diagonal movement costs
// TODO: Add line-of-sight calculation
// FIXME: This breaks with negative coordinates
// HACK: Temporary workaround until proper system exists
```

---

## UPROPERTY Usage

### EditAnywhere vs EditDefaultsOnly
**EditAnywhere:** Can edit per-instance  
**EditDefaultsOnly:** Can only edit in Blueprint defaults
```cpp
// Per-instance editing allowed
UPROPERTY(EditAnywhere, Category = "Kalki|Grid")
int32 GridSizeX = 20;

// Only in Blueprint defaults
UPROPERTY(EditDefaultsOnly, Category = "Kalki|Mesh")
TObjectPtr<UStaticMesh> TileMesh;
```

### BlueprintReadOnly vs BlueprintReadWrite
**BlueprintReadOnly:** Read access only  
**BlueprintReadWrite:** Full access
```cpp
// Blueprint can read but not modify
UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid")
int32 CurrentGridSize;

// Blueprint can read and modify
UPROPERTY(BlueprintReadWrite, Category = "Kalki|Grid")
bool bShowDebugInfo;
```

### Categories
**Always specify categories for organization**
```cpp
UPROPERTY(EditAnywhere, Category = "Kalki|Grid|Visualization")
FLinearColor TileColor;

UPROPERTY(BlueprintReadOnly, Category = "Kalki|Combat|Stats")
float CurrentHealth;
```

### Tooltips
**Add tooltips for designer-facing properties**
```cpp
UPROPERTY(EditAnywhere, Category = "Kalki|Grid", meta = (
    Tooltip = "Scale factor for tiles. 0.9 = 10% gap between tiles"))
float TileScale = 0.9f;

UPROPERTY(EditAnywhere, Category = "Kalki|Grid", meta = (
    ClampMin = "0.1", ClampMax = "1.0", 
    Tooltip = "Tile scale from 0.1 to 1.0"))
float ClampedTileScale = 0.9f;
```

---

## UFUNCTION Usage

### Function Specifiers
```cpp
// Callable from Blueprint, shows in palette
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
void CreateGrid(int32 SizeX, int32 SizeY);

// Pure function (no side effects), appears as getter node
UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
int32 GetGridSizeX() const;

// Implementable in Blueprint
UFUNCTION(BlueprintImplementableEvent, Category = "Kalki|Events")
void OnGridCreated();

// Can be implemented in Blueprint, has C++ default
UFUNCTION(BlueprintNativeEvent, Category = "Kalki|Events")
void OnTileChanged(const FKalkiGridCoord& Coord);

// Console command (cheats)
UFUNCTION(Exec, Category = "Kalki|Cheats")
void CreateTestGrid(int32 SizeX, int32 SizeY);
```

### Function Naming in Macros
**Match C++ convention even in UFUNCTION**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
void SelectTile(const FKalkiGridCoord& Coord);  // PascalCase

UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
bool IsValidCoord(const FKalkiGridCoord& Coord) const;  // Is/Has/Can prefix
```

---

## Const Correctness

### Use Const for Read-Only Functions
**Mark functions const if they don't modify state**
```cpp
// ✅ Correct - doesn't modify object
int32 GetGridSizeX() const;
bool IsValidCoord(const FKalkiGridCoord& Coord) const;

// ❌ Incorrect - should be const
int32 GetGridSizeX();  // Missing const
```

### Use Const References for Parameters
**Pass by const reference for non-primitive types**
```cpp
// ✅ Correct
void SetTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile);

// ❌ Incorrect - unnecessary copies
void SetTile(FKalkiGridCoord Coord, FKalkiGridTile Tile);
```

### Const Pointers
**Use const appropriately**
```cpp
// Const pointer (can't change what it points to)
const UKalkiGridManager* GridManager;

// Pointer to const (can't modify through pointer)
UKalkiGridManager* const GridManager;

// Const pointer to const
const UKalkiGridManager* const GridManager;
```

---

## Memory Management

### Use TObjectPtr (UE 5.1+)
**For UObject-derived pointers**
```cpp
// ✅ Correct (UE 5.1+)
UPROPERTY()
TObjectPtr<UKalkiGridManager> GridManager;

// ⚠️ Legacy (still works but deprecated)
UPROPERTY()
UKalkiGridManager* GridManager;
```

### Raw Pointers for Non-UObject
**Only when necessary**
```cpp
// Non-UObject types
float* RawDataArray;
APlayerController* PC;  // Temporary, not stored
```

### Smart Pointers
**Use for non-UObject memory management**
```cpp
TSharedPtr<FMyData> SharedData;
TUniquePtr<FMyResource> UniqueResource;
TWeakPtr<FMyData> WeakReference;
```

---

## Error Handling

### Logging Errors
**Use KalkiLog for all logging**
```cpp
if (!GridManager)
{
    KalkiLog::Grid(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error, this);
    return false;
}
```

### Early Returns
**Validate inputs early**
```cpp
void UKalkiGridManager::SetTile(const FKalkiGridCoord& Coord, const FKalkiGridTile& Tile)
{
    // Validate authority
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("SetTile - No authority"), EKalkiLogSeverity::Warning);
        return;
    }
    
    // Validate coordinates
    if (!IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetTile - Invalid coord: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Error
        );
        return;
    }
    
    // Main logic here
    GridTiles.Add(Coord, Tile);
}
```

### Assertions
**Use check() for critical errors in development**
```cpp
// Critical error - should never happen
check(GridManager != nullptr);
check(IsValidCoord(Coord));

// Verification in development, removed in shipping
checkSlow(ExpensiveValidation());

// With message
checkf(IsValid(Actor), TEXT("Actor is invalid: %s"), *GetNameSafe(Actor));
```

---

## Multiplayer Considerations

### Authority Checks
**Always check authority for modifications**
```cpp
void ModifyGameState()
{
    // Only server can modify
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("ModifyGameState - Client attempted modification"), 
            EKalkiLogSeverity::Warning);
        return;
    }
    
    // Modification logic
}
```

### Replication
**Mark replicated properties correctly**
```cpp
UPROPERTY(Replicated)
int32 GridSizeX;

UPROPERTY(ReplicatedUsing = OnRep_GridCreated)
bool bGridCreated;

// In .cpp:
void UKalkiClassName::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UKalkiClassName, GridSizeX);
    DOREPLIFETIME(UKalkiClassName, bGridCreated);
}
```

---

## Performance Guidelines

### Prefer TMap for Sparse Data
**Use TMap for grid storage (not TArray[X][Y])**
```cpp
// ✅ Good for sparse grids
TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;

// ❌ Bad - wastes memory for sparse grids
TArray<TArray<FKalkiGridTile>> GridTiles;  // 100x100 always allocates 10,000 tiles
```

### Cache Expensive Lookups
**Don't repeat expensive operations**
```cpp
// ✅ Good
UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
if (GridManager)
{
    GridManager->DoWork1();
    GridManager->DoWork2();
    GridManager->DoWork3();
}

// ❌ Bad - repeated lookups
if (UKalkiGridManager* GM = GetWorld()->GetSubsystem<UKalkiGridManager>())
{
    GM->DoWork1();
}
if (UKalkiGridManager* GM = GetWorld()->GetSubsystem<UKalkiGridManager>())  // Lookup again!
{
    GM->DoWork2();
}
```

### Minimize Tick Usage
**Only tick when necessary**
```cpp
// In constructor:
PrimaryActorTick.bCanEverTick = true;
PrimaryActorTick.bStartWithTickEnabled = false;  // Start disabled

// Enable when needed:
SetActorTickEnabled(true);

// Disable when done:
SetActorTickEnabled(false);
```

---

## String Handling

### Use TEXT() Macro
**For string literals**
```cpp
// ✅ Correct
FString Message = TEXT("Hello World");
KalkiLog::Grid(TEXT("Grid created"));

// ❌ Incorrect
FString Message = "Hello World";  // Compiler warning
```

### String Formatting
**Use FString::Printf or FText::Format**
```cpp
// FString::Printf (C-style)
FString Message = FString::Printf(TEXT("Grid size: %dx%d"), SizeX, SizeY);

// FText::Format (Unreal style, better for localization)
FText Message = FText::Format(
    INVTEXT("Grid size: {0}x{1}"),
    SizeX,
    SizeY
);
```

### INVTEXT vs LOCTEXT
**INVTEXT:** Not localized (internal/debug text)  
**LOCTEXT:** Localized (player-facing text)
```cpp
// Debug/internal text
FText DebugText = INVTEXT("Debug info");

// Player-facing text
FText UIText = LOCTEXT("GridSizeLabel", "Grid Size");
```

---

## Code Organization

### Group Related Functions
**Keep related functionality together**
```cpp
// ========================================
// GRID CREATION
// ========================================

void CreateGrid(int32 SizeX, int32 SizeY);
void ClearGrid();
void InitializeGrid();

// ========================================
// TILE QUERIES
// ========================================

FKalkiGridTile GetTile(const FKalkiGridCoord& Coord) const;
bool IsValidCoord(const FKalkiGridCoord& Coord) const;
TArray<FKalkiGridCoord> GetNeighbors(const FKalkiGridCoord& Coord) const;

// ========================================
// TILE MODIFICATION
// ========================================

bool SetElevation(const FKalkiGridCoord& Coord, float Elevation);
bool SetWalkable(const FKalkiGridCoord& Coord, bool bWalkable);
bool SetOccupant(const FKalkiGridCoord& Coord, AActor* Occupant);
```

### Public Before Private
**Order: public → protected → private**
```cpp
class UMyClass
{
public:
    // Public interface first
    
protected:
    // Protected members
    
private:
    // Private implementation
};
```

---

## Examples

### Complete Class Example
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridHelper.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridHelper.generated.h"

/**
 * Grid Helper
 * Static utility functions for grid calculations
 * 
 * USAGE:
 * - Call static functions directly
 * - No instance needed (all functions static)
 */
UCLASS()
class KALKI_API UKalkiGridHelper : public UObject
{
    GENERATED_BODY()

public:
    // ========================================
    // DISTANCE CALCULATIONS
    // ========================================
    
    /**
     * Calculate Manhattan distance between two coordinates
     * @param A - First coordinate
     * @param B - Second coordinate
     * @return Distance in tiles (|X1-X2| + |Y1-Y2|)
     */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid|Math")
    static int32 ManhattanDistance(const FKalkiGridCoord& A, const FKalkiGridCoord& B);
    
    /**
     * Calculate Euclidean distance between two coordinates
     * @param A - First coordinate
     * @param B - Second coordinate
     * @return Distance in tiles (sqrt((X1-X2)² + (Y1-Y2)²))
     */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid|Math")
    static float EuclideanDistance(const FKalkiGridCoord& A, const FKalkiGridCoord& B);
    
    // ========================================
    // DIRECTION HELPERS
    // ========================================
    
    /**
     * Get direction from A to B
     * @param From - Starting coordinate
     * @param To - Target coordinate
     * @return Normalized direction vector
     */
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid|Math")
    static FVector2D GetDirection(const FKalkiGridCoord& From, const FKalkiGridCoord& To);
};
```

---

## Checklist for Code Review

**Before submitting code:**
- ✅ Copyright header present
- ✅ Class documentation present
- ✅ Public functions documented
- ✅ Proper const usage
- ✅ Error logging with KalkiLog
- ✅ Authority checks for multiplayer
- ✅ UPROPERTY categories specified
- ✅ Consistent formatting (tabs, braces, spacing)
- ✅ No TODO/FIXME in production code
- ✅ TObjectPtr used for UObject pointers
- ✅ Section dividers for organization
- ✅ Follows naming conventions

---

## Related Documents

### Core Conventions
- [[02_Conventions/Naming_Conventions]] - How to name things
- [[02_Conventions/File_Organization]] - Where files go
- [[02_Conventions/Blueprint_Guidelines]] - Blueprint standards

### Implementation
- [[04_Implementation/Class_Templates]] - Code templates
- [[04_Implementation/Common_Patterns]] - Common patterns
- [[04_Implementation/Error_Handling]] - Error handling guidelines
- [[04_Implementation/Multiplayer_Guidelines]] - Multiplayer best practices

### For Claude
- [[06_How_To_Work_With_Claude/Code_Generation_Rules]] - How to generate code
- [[06_How_To_Work_With_Claude/Context_Guidelines]] - Response format

### Reference
- [[03_Systems/Logging_System]] - KalkiLog usage
- [[01_Architecture/Project_Structure]] - Project organization

---

## Document Info
**File Path:** `Kalki_GDD/02_Conventions/Coding_Standards.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation