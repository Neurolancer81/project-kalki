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

# Naming Conventions

## Overview
This document defines all naming conventions used in the Kalki project. Consistent naming is critical for code generation, maintainability, and team collaboration. **All code must follow these conventions.**

---

## Project Prefix

### Primary Prefix: `Kalki`
All project-specific code uses the `Kalki` prefix to avoid naming conflicts with engine code and plugins.

**Examples:**
- `KalkiCharacter`
- `KalkiGridManager`
- `KalkiCombatSystem`

---

## Class Naming

### Unreal Type Prefixes

#### **Classes (UObject-derived):**
```cpp
// Prefix: U (Unreal Engine convention)
class UKalkiGridManager : public UWorldSubsystem { };
class UKalkiAbilityComponent : public UActorComponent { };
class UKalkiViewModel : public UObject { };
```

#### **Actors:**
```cpp
// Prefix: A (Unreal Engine convention)
class AKalkiCharacter : public ACharacter { };
class AKalkiGridVisualizer : public AActor { };
class AKalkiLevelManager : public AActor { };
```

#### **Structs:**
```cpp
// Prefix: F (Unreal Engine convention)
struct FKalkiGridCoord { };
struct FKalkiGridTile { };
struct FKalkiAbilityData { };
```

#### **Enums:**
```cpp
// Prefix: E (Unreal Engine convention)
enum class EKalkiLogChannel : uint8 { };
enum class EKalkiAbilityType : uint8 { };
enum class EKalkiDamageType : uint8 { };
```

#### **Interfaces:**
```cpp
// Prefix: I (Unreal Engine convention)
class IKalkiTargetable { };
class IKalkiInteractable { };
```

---

## File Naming

### Header Files
**Format:** `KalkiClassName.h`

**Examples:**
```
KalkiCharacter.h
KalkiGridManager.h
KalkiGridTypes.h
KalkiLog.h
```

### Implementation Files
**Format:** `KalkiClassName.cpp`

**Examples:**
```
KalkiCharacter.cpp
KalkiGridManager.cpp
KalkiLog.cpp
```

### Blueprint Files
**Format:** `BP_ClassName`

**Examples:**
```
BP_KalkiCharacter
BP_GridVisualizer
BP_LevelManager
```

### Widget Blueprints
**Format:** `WBP_WidgetName`

**Examples:**
```
WBP_CombatLog
WBP_GridInfo
WBP_CharacterSheet
WBP_MainHUD
```

### Material Files
**Format:** `M_MaterialName`

**Examples:**
```
M_GridTile_Base
M_Character_Skin
M_UI_Background
```

### Material Instances
**Format:** `MI_MaterialName_Variant`

**Examples:**
```
MI_GridTile_Walkable
MI_GridTile_Unwalkable
MI_Character_Skin_PlayerTeam
```

### Texture Files
**Format:** `T_TextureName_Type`

**Examples:**
```
T_GridTile_Albedo
T_GridTile_Normal
T_Character_Icon
```

---

## Variable Naming

### Boolean Variables
**Prefix:** `b`

**Examples:**
```cpp
bool bIsWalkable;
bool bHasAuthority;
bool bShowGrid;
bool bInCombat;
```

### Integer/Float Variables
**No prefix** (type is clear from context)

**Examples:**
```cpp
int32 GridSizeX;
float MovementSpeed;
int32 ActionPoints;
float DamageAmount;
```

### Pointers (UObject-derived)
**Use TObjectPtr** (Unreal 5.1+)

**Examples:**
```cpp
TObjectPtr<UKalkiGridManager> GridManager;
TObjectPtr<AKalkiCharacter> OwningCharacter;
TObjectPtr<UUserWidget> InfoWidget;
```

### Arrays
**Plural naming**

**Examples:**
```cpp
TArray<FKalkiGridCoord> PathWaypoints;
TArray<AKalkiCharacter*> TeamMembers;
TArray<FKalkiAbilityData> AvailableAbilities;
```

### Maps
**Descriptive naming**

**Examples:**
```cpp
TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;
TMap<int32, AKalkiCharacter*> CharactersByID;
TMap<FName, float> AbilityCooldowns;
```

### Sets
**Plural naming**

**Examples:**
```cpp
TSet<FKalkiGridCoord> WalkableTiles;
TSet<AKalkiCharacter*> VisibleEnemies;
TSet<FName> UnlockedAbilities;
```

---

## Function Naming

### Regular Functions
**PascalCase** (Unreal convention)

**Examples:**
```cpp
void CreateGrid(int32 SizeX, int32 SizeY);
FKalkiGridTile GetTile(const FKalkiGridCoord& Coord) const;
bool IsValidCoord(const FKalkiGridCoord& Coord) const;
void UpdateTileVisual(const FKalkiGridCoord& Coord);
```

### Getters
**Prefix:** `Get`

**Examples:**
```cpp
int32 GetGridSizeX() const;
float GetElevation() const;
AKalkiCharacter* GetOwningCharacter() const;
```

### Setters
**Prefix:** `Set`

**Examples:**
```cpp
void SetElevation(float NewElevation);
void SetWalkable(bool bWalkable);
void SetOccupant(AActor* Occupant);
```

### Boolean Queries
**Prefix:** `Is`, `Has`, `Can`, `Should`

**Examples:**
```cpp
bool IsWalkable() const;
bool HasAuthority() const;
bool CanMoveTo(const FKalkiGridCoord& Coord) const;
bool ShouldShowGrid() const;
```

### Event Handlers
**Prefix:** `On`

**Examples:**
```cpp
void OnTileChanged(const FKalkiGridCoord& Coord);
void OnGridCreated();
void OnCharacterDeath(AKalkiCharacter* Character);
```

### Blueprint Callable Functions
**Same naming, add UFUNCTION macro**

**Examples:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
void SelectTile(const FKalkiGridCoord& Coord);

UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
bool IsValidCoord(const FKalkiGridCoord& Coord) const;
```

---

## Parameter Naming

### Input Parameters
**Prefix:** `In` (optional but recommended for clarity)

**Examples:**
```cpp
void CreateGrid(int32 InSizeX, int32 InSizeY, float InTileSize);
void ApplyDamage(float InDamageAmount, EKalkiDamageType InDamageType);
```

### Output Parameters
**Prefix:** `Out` (required for clarity)

**Examples:**
```cpp
bool GetTileUnderCursor(FKalkiGridCoord& OutCoord) const;
void GetNeighbors(const FKalkiGridCoord& Coord, TArray<FKalkiGridCoord>& OutNeighbors) const;
```

### Reference Parameters
**Prefix:** `Ref` (optional)

**Examples:**
```cpp
void ModifyTile(FKalkiGridTile& RefTile);
void UpdateCharacterStats(FCharacterStats& RefStats);
```

---

## Property Naming (UPROPERTY)

### Private/Protected Members
**No prefix** (unless bool)

**Examples:**
```cpp
UPROPERTY()
TObjectPtr<UKalkiGridManager> GridManager;

UPROPERTY()
int32 GridSizeX;

UPROPERTY()
bool bGridInitialized;
```

### Editable Properties
**Descriptive names**

**Examples:**
```cpp
UPROPERTY(EditAnywhere, Category = "Grid")
int32 GridSizeX = 20;

UPROPERTY(EditAnywhere, Category = "Grid")
float TileSize = 100.0f;

UPROPERTY(EditDefaultsOnly, Category = "Grid|Mesh")
TObjectPtr<UStaticMesh> TileMesh;
```

### Blueprint Read-Only Properties
**Same naming**

**Examples:**
```cpp
UPROPERTY(BlueprintReadOnly, Category = "Grid")
FKalkiGridCoord CurrentCoord;

UPROPERTY(BlueprintReadOnly, Category = "Grid")
float MovementCost;
```

---

## Enum Naming

### Enum Class Names
**Prefix:** `EKalki`

**Examples:**
```cpp
enum class EKalkiLogChannel : uint8;
enum class EKalkiAbilityType : uint8;
enum class EKalkiDamageType : uint8;
```

### Enum Values
**No prefix** (already namespaced by enum)

**Examples:**
```cpp
enum class EKalkiLogChannel : uint8
{
    General,
    Combat,
    Grid,
    System,
    UI
};

// Usage:
EKalkiLogChannel::Grid
```

### Enum Display Names (UMETA)
**User-friendly names**

**Examples:**
```cpp
enum class EKalkiAbilityType : uint8
{
    Attack      UMETA(DisplayName = "Attack"),
    Heal        UMETA(DisplayName = "Healing"),
    Buff        UMETA(DisplayName = "Buff/Enhance"),
    Debuff      UMETA(DisplayName = "Debuff/Weaken"),
    Utility     UMETA(DisplayName = "Utility")
};
```

---

## Delegate Naming

### Regular Delegates
**Format:** `FOnEventName`

**Examples:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCreated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileChanged, const FKalkiGridCoord&, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDeath, AKalkiCharacter*, Character, float, Damage);
```

### Delegate Properties
**Format:** `OnEventName`

**Examples:**
```cpp
UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnGridCreated OnGridCreated;

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnTileChanged OnTileChanged;
```

---

## Category Naming

### UPROPERTY Categories
**Format:** `"Kalki|System|Subsection"`

**Examples:**
```cpp
UPROPERTY(EditAnywhere, Category = "Kalki|Grid")
UPROPERTY(EditAnywhere, Category = "Kalki|Grid|Visualization")
UPROPERTY(EditAnywhere, Category = "Kalki|Combat|Abilities")
UPROPERTY(EditAnywhere, Category = "Kalki|Character|Stats")
```

### UFUNCTION Categories
**Same format**

**Examples:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
UFUNCTION(BlueprintPure, Category = "Kalki|Combat|Damage")
UFUNCTION(Exec, Category = "Kalki|Cheats")
```

---

## Constant Naming

### Global Constants
**ALL_CAPS with underscores**

**Examples:**
```cpp
static constexpr float MAX_GRID_SIZE = 100.0f;
static constexpr int32 DEFAULT_ACTION_POINTS = 3;
static constexpr float TILE_SCALE_MIN = 0.1f;
```

### Class Constants
**Same convention**

**Examples:**
```cpp
class UKalkiGridManager
{
    static constexpr int32 MAX_GRID_DIMENSION = 100;
    static constexpr float DEFAULT_TILE_SIZE = 100.0f;
};
```

---

## Namespace Usage

### Avoid Global Namespaces
**Use static classes instead**

**❌ Don't:**
```cpp
namespace Kalki
{
    void DoSomething();
}
```

**✅ Do:**
```cpp
class KalkiUtilities
{
public:
    static void DoSomething();
};
```

---

## Widget Naming (UMG)

### Widget Variable Names
**Descriptive, no prefix unless bool**

**Examples:**
```cpp
UPROPERTY(meta = (BindWidget))
TObjectPtr<UTextBlock> CoordinateText;

UPROPERTY(meta = (BindWidget))
TObjectPtr<UBorder> RootBorder;

UPROPERTY(meta = (BindWidget))
TObjectPtr<UButton> ConfirmButton;
```

### Optional Widgets
**Use BindWidgetOptional**

**Examples:**
```cpp
UPROPERTY(meta = (BindWidgetOptional))
TObjectPtr<UTextBlock> MovementCostText;

UPROPERTY(meta = (BindWidgetOptional))
TObjectPtr<UImage> StatusIcon;
```

---

## Comments and Documentation

### File Headers
**Copyright + brief description**

**Example:**
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridManager.h

#pragma once

#include "CoreMinimal.h"
// ... rest of file
```

### Class Documentation
**Brief description before class**

**Example:**
```cpp
/**
 * Grid Manager
 * Manages the tactical grid system including tile data, 
 * elevation, walkability, and spatial queries.
 * 
 * Multiplayer: Server authority, deterministic replication
 */
UCLASS()
class KALKI_API UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()
};
```

### Function Documentation
**Brief description for public functions**

**Example:**
```cpp
/**
 * Get all tiles within specified range using Manhattan distance
 * @param Center - Origin coordinate
 * @param Range - Maximum distance in tiles
 * @param bRequireWalkable - If true, only return walkable tiles
 * @return Array of coordinates within range
 */
UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
TArray<FKalkiGridCoord> GetTilesInRange(const FKalkiGridCoord& Center, int32 Range, bool bRequireWalkable = false) const;
```

---

## Examples Summary

### Full Class Example:
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridManager.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Grid/KalkiGridTypes.h"
#include "KalkiGridManager.generated.h"

/**
 * Grid Manager
 * Central authority for grid data and queries
 */
UCLASS()
class KALKI_API UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
    FOnGridCreated OnGridCreated;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void CreateGrid(int32 InSizeX, int32 InSizeY, float InTileSize);

    UFUNCTION(BlueprintPure, Category = "Kalki|Grid")
    bool IsValidCoord(const FKalkiGridCoord& Coord) const;

protected:
    // Properties
    UPROPERTY()
    TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;

    UPROPERTY()
    int32 GridSizeX;

    UPROPERTY()
    bool bGridInitialized;

    // Helper functions
    bool HasGridAuthority() const;
    void BroadcastGridCreated();
};
```

---

## Checklist for New Code

**Before submitting code, verify:**
- ✅ Class has proper prefix (U/A/F/E/I)
- ✅ File names match class names
- ✅ Booleans have `b` prefix
- ✅ Functions use PascalCase
- ✅ Output params have `Out` prefix
- ✅ Categories use "Kalki|System" format
- ✅ Copyright header present
- ✅ Class has documentation comment
- ✅ Public functions have documentation
- ✅ Delegates follow FOn naming
- ✅ Enums use EKalki prefix
- ✅ Constants use ALL_CAPS

---

## Related Documents

### Core Conventions
- [[02_Conventions/Coding_Standards]] - Code formatting and structure
- [[02_Conventions/File_Organization]] - Where files go
- [[02_Conventions/Blueprint_Guidelines]] - Blueprint-specific conventions

### Implementation
- [[04_Implementation/Class_Templates]] - Template code with proper naming
- [[04_Implementation/Common_Patterns]] - Common code patterns

### For Claude
- [[06_How_To_Work_With_Claude/Code_Generation_Rules]] - How to generate code
- [[06_How_To_Work_With_Claude/Context_Guidelines]] - Understanding requirements

### Reference
- [[01_Architecture/Project_Structure]] - File locations
- [[05_API_Reference/KalkiLog_API]] - Example of proper naming

---

## Document Info
**File Path:** `Kalki_GDD/02_Conventions/Naming_Conventions.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation