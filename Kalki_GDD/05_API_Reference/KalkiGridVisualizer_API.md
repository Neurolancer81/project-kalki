---
tags:
  - kalki/api
  - kalki/grid
  - kalki/visualization
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# KalkiGridVisualizer API Reference

## Overview
Complete API reference for the AKalkiGridVisualizer actor. This document provides detailed information about all public functions, properties, and usage patterns for visualizing the tactical grid.

---

## Purpose

### What is KalkiGridVisualizer?

**KalkiGridVisualizer** is an actor that renders the tactical grid using Instanced Static Meshes. It provides:
- Visual representation of all grid tiles
- Hover highlighting (single tile)
- Selection highlighting (single tile)
- Range highlighting (multiple tiles)
- Elevation visualization
- Walkability visualization
- Real-time updates when grid changes
- Efficient rendering (instanced meshes)

**See also:** [[03_Systems/Grid_Visualizer]] for architectural details and design patterns.

---

## Quick Start

### Basic Usage
```cpp
#include "Grid/KalkiGridVisualizer.h"

// Spawn visualizer (usually done in level blueprint or game mode)
AKalkiGridVisualizer* Visualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>(
    AKalkiGridVisualizer::StaticClass(),
    FVector::ZeroVector,
    FRotator::ZeroRotator
);

// Visualizer automatically creates visuals when grid is ready

// Highlight tile on hover
Visualizer->SetHoveredTile(FKalkiGridCoord(5, 7));

// Select tile
Visualizer->SetSelectedTile(FKalkiGridCoord(10, 10));

// Show movement range
TArray<FKalkiGridCoord> Range = {
    FKalkiGridCoord(9, 10),
    FKalkiGridCoord(10, 9),
    FKalkiGridCoord(11, 10),
    FKalkiGridCoord(10, 11)
};
Visualizer->SetHighlightedTiles(Range);

// Clear highlights
Visualizer->ClearHoveredTile();
Visualizer->ClearSelectedTile();
Visualizer->ClearHighlightedTiles();
```

---

## Class Overview

### AKalkiGridVisualizer

**Type:** Actor  
**Path:** `Source/Kalki/Public/Grid/KalkiGridVisualizer.h`  
**Parent:** `AActor`

**Components:**
- `TileInstancedMeshComponent` - Renders all grid tiles efficiently
- `OverlayInstancedMeshComponent` - Renders highlights/overlays

**Lifetime:** Spawned in level, persists until level unload

**Authority:** Works on all machines (server + clients)

**Purpose:**
- Render grid tiles visually
- Show hover/selection/range highlights
- Update visuals when grid changes
- Provide visual feedback to players
- Efficient rendering (one draw call per instance type)

---

## Actor Lifecycle

### BeginPlay()

**Signature:**
```cpp
virtual void BeginPlay() override;
```

**Description:**
Called when actor is spawned. Binds to GridManager events and initializes components.

**Automatic Actions:**
1. Gets GridManager subsystem
2. Binds to `OnGridCreated` event
3. Binds to `OnTileChanged` event
4. Creates visuals if grid already exists

**Usage:**
```cpp
// Called automatically - DO NOT call manually
```

**Notes:**
- If grid already created, calls `CreateGridVisuals()` immediately
- If grid not created, waits for `OnGridCreated` event
- Works on both server and clients

---

### EndPlay()

**Signature:**
```cpp
virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
```

**Description:**
Called when actor is destroyed. Cleans up event bindings.

**Automatic Actions:**
1. Unbinds from GridManager events
2. Clears all instances
3. Cleans up components

**Usage:**
```cpp
// Called automatically - DO NOT call manually
```

---

## Initialization

### CreateGridVisuals()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void CreateGridVisuals();
```

**Description:**
Creates visual instances for all tiles in the grid. Called automatically when grid is created.

**Example:**
```cpp
// Usually called automatically, but can call manually if needed
Visualizer->CreateGridVisuals();
```

**Actions:**
1. Clears existing instances
2. Validates GridManager and grid existence
3. Creates one instance per tile
4. Sets instance transforms and colors
5. Updates component bounds

**Performance:**
- Time: O(n) where n = number of tiles
- 20×20 grid (400 tiles): ~1-2ms
- 50×50 grid (2500 tiles): ~5-10ms

**Notes:**
- Only call once per grid
- Automatically called on `OnGridCreated` event
- Works on all machines
- Efficient (instanced rendering)

---

### ClearGridVisuals()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void ClearGridVisuals();
```

**Description:**
Destroys all visual instances. Called automatically when grid is cleared.

**Example:**
```cpp
Visualizer->ClearGridVisuals();
```

**Actions:**
1. Clears all tile instances
2. Clears all overlay instances
3. Resets instance counts
4. Updates component bounds

**Notes:**
- Automatically called on `OnGridCleared` event
- Safe to call even if no visuals exist
- Frees GPU memory

---

## Hover Highlighting

### SetHoveredTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void SetHoveredTile(const FKalkiGridCoord& Coord);
```

**Parameters:**
- `Coord` - Grid coordinate to highlight

**Description:**
Highlights a single tile as "hovered" (usually under mouse cursor).

**Example:**
```cpp
// In PlayerController::Tick()
FHitResult Hit;
GetHitResultUnderCursor(ECC_Visibility, false, Hit);

if (Hit.bBlockingHit)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(Hit.Location);
    
    if (GridManager->IsValidCoord(Coord))
    {
        Visualizer->SetHoveredTile(Coord);
    }
}
```

**Visual Effect:**
- Tile rendered with `HoverColor` (default: semi-transparent blue)
- Previous hovered tile returns to normal
- Overlay rendered slightly above tile

**Notes:**
- Only one tile can be hovered at a time
- Automatically clears previous hover
- Validates coordinate internally
- Does nothing if coordinate invalid
- Works on all machines

---

### ClearHoveredTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void ClearHoveredTile();
```

**Description:**
Removes hover highlight from current hovered tile.

**Example:**
```cpp
// Mouse left grid area
Visualizer->ClearHoveredTile();

// Character moved, clear hover
if (bCharacterMoved)
{
    Visualizer->ClearHoveredTile();
}
```

**Notes:**
- Safe to call even if no tile hovered
- Removes overlay instance
- Fast operation

---

### GetHoveredTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid|Visualizer")
FKalkiGridCoord GetHoveredTile() const;
```

**Returns:** Currently hovered coordinate, or invalid coord if none

**Description:**
Gets the coordinate of the currently hovered tile.

**Example:**
```cpp
FKalkiGridCoord Hovered = Visualizer->GetHoveredTile();
if (Hovered.IsValid())
{
    // A tile is hovered
    UE_LOG(LogTemp, Log, TEXT("Hovering tile (%d, %d)"), Hovered.X, Hovered.Y);
}
```

**Notes:**
- Returns invalid coord if no tile hovered
- Check with `IsValid()` before using

---

## Selection Highlighting

### SetSelectedTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void SetSelectedTile(const FKalkiGridCoord& Coord);
```

**Parameters:**
- `Coord` - Grid coordinate to select

**Description:**
Highlights a single tile as "selected" (usually clicked tile).

**Example:**
```cpp
// On mouse click
void AKalkiPlayerController::OnMouseClick()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
    
    if (Hit.bBlockingHit)
    {
        UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
        FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(Hit.Location);
        
        if (GridManager->IsValidCoord(Coord))
        {
            Visualizer->SetSelectedTile(Coord);
        }
    }
}
```

**Visual Effect:**
- Tile rendered with `SelectionColor` (default: semi-transparent yellow)
- Previous selected tile returns to normal
- Overlay rendered slightly above tile
- Persists until cleared or new selection

**Notes:**
- Only one tile can be selected at a time
- Automatically clears previous selection
- Different color than hover (distinguishable)
- Validates coordinate internally

---

### ClearSelectedTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void ClearSelectedTile();
```

**Description:**
Removes selection highlight from current selected tile.

**Example:**
```cpp
// Deselect when action completes
Character->OnMovementComplete.AddLambda([this]()
{
    Visualizer->ClearSelectedTile();
});

// Clear selection on right-click
if (bRightClick)
{
    Visualizer->ClearSelectedTile();
}
```

**Notes:**
- Safe to call even if no tile selected
- Removes overlay instance
- Fast operation

---

### GetSelectedTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid|Visualizer")
FKalkiGridCoord GetSelectedTile() const;
```

**Returns:** Currently selected coordinate, or invalid coord if none

**Description:**
Gets the coordinate of the currently selected tile.

**Example:**
```cpp
FKalkiGridCoord Selected = Visualizer->GetSelectedTile();
if (Selected.IsValid())
{
    // A tile is selected
    Character->MoveTo(Selected);
}
```

---

## Range Highlighting

### SetHighlightedTiles()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void SetHighlightedTiles(const TArray<FKalkiGridCoord>& Coords);
```

**Parameters:**
- `Coords` - Array of grid coordinates to highlight

**Description:**
Highlights multiple tiles (usually movement range or ability area).

**Example:**
```cpp
// Show movement range
void AKalkiCharacter::ShowMovementRange()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    // Get reachable tiles
    TArray<FKalkiGridCoord> Reachable = GridManager->GetTilesInRange(
        GridPosition,
        MovementRange,
        true  // Only walkable
    );
    
    // Highlight them
    Visualizer->SetHighlightedTiles(Reachable);
}

// Show ability area of effect
void UKalkiAbility::PreviewTargetArea(const FKalkiGridCoord& Target)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    
    TArray<FKalkiGridCoord> Area = GridManager->GetTilesInRange(Target, Radius);
    
    Visualizer->SetHighlightedTiles(Area);
}
```

**Visual Effect:**
- All tiles rendered with `HighlightColor` (default: semi-transparent green)
- Previous highlights cleared
- Overlays rendered slightly above tiles
- All highlighted tiles same color

**Notes:**
- Can highlight any number of tiles
- Automatically clears previous highlights
- Validates all coordinates internally (skips invalid)
- Efficient for large ranges (instanced rendering)

---

### AddHighlightedTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void AddHighlightedTile(const FKalkiGridCoord& Coord);
```

**Parameters:**
- `Coord` - Grid coordinate to add to highlights

**Description:**
Adds a single tile to the current highlight set (doesn't clear existing).

**Example:**
```cpp
// Add tiles incrementally
for (const FKalkiGridCoord& Coord : PathTiles)
{
    Visualizer->AddHighlightedTile(Coord);
    FPlatformProcess::Sleep(0.1f); // Animate path reveal
}
```

**Notes:**
- Does NOT clear existing highlights
- Use for incremental additions
- Validates coordinate internally
- Use `SetHighlightedTiles()` for bulk operations

---

### RemoveHighlightedTile()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void RemoveHighlightedTile(const FKalkiGridCoord& Coord);
```

**Parameters:**
- `Coord` - Grid coordinate to remove from highlights

**Description:**
Removes a single tile from the current highlight set.

**Example:**
```cpp
// Remove tiles as character moves through them
void AKalkiCharacter::OnReachedWaypoint(const FKalkiGridCoord& Coord)
{
    Visualizer->RemoveHighlightedTile(Coord);
}
```

**Notes:**
- Only removes specified tile
- Other highlights remain
- Does nothing if tile not highlighted

---

### ClearHighlightedTiles()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void ClearHighlightedTiles();
```

**Description:**
Removes all tile highlights.

**Example:**
```cpp
// Clear when turn ends
void AKalkiCharacter::OnTurnEnd()
{
    Visualizer->ClearHighlightedTiles();
}

// Clear when ability cancelled
if (bAbilityCancelled)
{
    Visualizer->ClearHighlightedTiles();
}
```

**Notes:**
- Removes all highlight overlays
- Safe to call even if no tiles highlighted
- Fast operation

---

### GetHighlightedTiles()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid|Visualizer")
TArray<FKalkiGridCoord> GetHighlightedTiles() const;
```

**Returns:** Array of currently highlighted coordinates

**Description:**
Gets all coordinates that are currently highlighted.

**Example:**
```cpp
TArray<FKalkiGridCoord> Highlighted = Visualizer->GetHighlightedTiles();

UE_LOG(LogTemp, Log, TEXT("Highlighting %d tiles"), Highlighted.Num());

for (const FKalkiGridCoord& Coord : Highlighted)
{
    // Process each highlighted tile
}
```

---

## Tile Updates

### UpdateTileVisual()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void UpdateTileVisual(const FKalkiGridCoord& Coord);
```

**Parameters:**
- `Coord` - Grid coordinate to update

**Description:**
Updates visual for a single tile (called when tile properties change).

**Example:**
```cpp
// Automatically called by OnTileChanged event
// Can call manually if needed
Visualizer->UpdateTileVisual(FKalkiGridCoord(10, 10));
```

**What Updates:**
- Tile position (if elevation changed)
- Tile color (if walkability changed)
- Instance transform

**Notes:**
- Automatically called when GridManager fires `OnTileChanged`
- Usually don't need to call manually
- Fast operation (updates single instance)

---

### RefreshAllVisuals()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Grid|Visualizer")
void RefreshAllVisuals();
```

**Description:**
Updates visuals for all tiles. Used when visual settings change.

**Example:**
```cpp
// After changing color scheme
Visualizer->WalkableColor = NewColor;
Visualizer->UnwalkableColor = NewColor;
Visualizer->RefreshAllVisuals();

// After loading settings
LoadVisualizerSettings();
Visualizer->RefreshAllVisuals();
```

**Performance:**
- Time: O(n) where n = number of tiles
- Expensive for large grids
- Use sparingly

**Notes:**
- Updates all instances
- Use only when necessary (settings change, etc.)
- Consider `UpdateTileVisual()` for single tiles

---

## Visual Properties

### Tile Mesh and Material

**Properties:**
```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Visuals")
UStaticMesh* TileMesh;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Visuals")
UMaterialInterface* TileMaterial;
```

**Description:**
Mesh and material used for grid tiles.

**Default Values:**
- `TileMesh`: `/Engine/BasicShapes/Plane` (flat plane)
- `TileMaterial`: Basic material with color parameter

**Customization:**
```cpp
// Set in Blueprint or C++
Visualizer->TileMesh = LoadObject<UStaticMesh>(
    nullptr,
    TEXT("/Game/Meshes/CustomTile.CustomTile")
);

Visualizer->TileMaterial = LoadObject<UMaterialInterface>(
    nullptr,
    TEXT("/Game/Materials/M_GridTile.M_GridTile")
);

// Refresh visuals after changing
Visualizer->RefreshAllVisuals();
```

**Requirements:**
- Mesh should be ~100×100 units (matches TileSize)
- Material should support color tinting
- Material should support transparency (for overlays)

---

### Tile Colors

**Properties:**
```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Colors")
FLinearColor WalkableColor;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Colors")
FLinearColor UnwalkableColor;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Colors")
FLinearColor ElevatedColor;
```

**Description:**
Colors used for different tile states.

**Default Values:**
- `WalkableColor`: White (1, 1, 1, 1)
- `UnwalkableColor`: Dark Gray (0.2, 0.2, 0.2, 1)
- `ElevatedColor`: Light Blue (0.5, 0.7, 1, 1)

**Example:**
```cpp
// Customize colors
Visualizer->WalkableColor = FLinearColor::White;
Visualizer->UnwalkableColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
Visualizer->ElevatedColor = FLinearColor(0.3f, 0.6f, 1.0f, 1.0f);

Visualizer->RefreshAllVisuals();
```

---

### Overlay Colors

**Properties:**
```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Colors")
FLinearColor HoverColor;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Colors")
FLinearColor SelectionColor;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Colors")
FLinearColor HighlightColor;
```

**Description:**
Colors used for hover/selection/range overlays.

**Default Values:**
- `HoverColor`: Blue with alpha (0, 0.5, 1, 0.5)
- `SelectionColor`: Yellow with alpha (1, 1, 0, 0.6)
- `HighlightColor`: Green with alpha (0, 1, 0, 0.4)

**Example:**
```cpp
// Enemy movement range (red)
Visualizer->HighlightColor = FLinearColor(1, 0, 0, 0.4f);
Visualizer->SetHighlightedTiles(EnemyRange);

// Restore normal (green)
Visualizer->HighlightColor = FLinearColor(0, 1, 0, 0.4f);
```

**Notes:**
- Alpha < 1.0 for transparency
- Must support transparency in material
- Change at runtime for different contexts

---

### Visual Settings

**Properties:**
```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Settings")
float TileHeightOffset;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Settings")
float OverlayHeightOffset;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Settings")
bool bShowElevation;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visualizer|Settings")
float ElevationScale;
```

**Description:**
Visual rendering settings.

**TileHeightOffset:**
- Default: `0.0f`
- Height above origin to render base tiles
- Use to prevent Z-fighting with ground

**OverlayHeightOffset:**
- Default: `5.0f`
- Height above tiles to render overlays
- Ensures overlays visible above tiles

**bShowElevation:**
- Default: `true`
- Whether to render tiles at different Z heights
- If `false`, all tiles at same Z (flat grid)

**ElevationScale:**
- Default: `1.0f`
- Multiplier for elevation display
- Higher = more exaggerated elevation differences

**Example:**
```cpp
// Flat grid (no elevation)
Visualizer->bShowElevation = false;
Visualizer->RefreshAllVisuals();

// Exaggerated elevation
Visualizer->ElevationScale = 2.0f;
Visualizer->RefreshAllVisuals();

// Prevent Z-fighting
Visualizer->TileHeightOffset = 1.0f;
Visualizer->RefreshAllVisuals();
```

---

## Instance Management

### GetInstanceCount()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid|Visualizer")
int32 GetInstanceCount() const;
```

**Returns:** Number of tile instances currently rendered

**Description:**
Gets the total number of tile instances (should match grid tile count).

**Example:**
```cpp
int32 Count = Visualizer->GetInstanceCount();
UE_LOG(LogTemp, Log, TEXT("Rendering %d tile instances"), Count);

// Verify matches grid size
UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
int32 ExpectedCount = GridManager->GetGridSizeX() * GridManager->GetGridSizeY();

if (Count != ExpectedCount)
{
    UE_LOG(LogTemp, Warning, TEXT("Instance count mismatch!"));
}
```

---

### GetOverlayInstanceCount()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid|Visualizer")
int32 GetOverlayInstanceCount() const;
```

**Returns:** Number of overlay instances currently rendered

**Description:**
Gets the number of overlay instances (hover + selection + highlights).

**Example:**
```cpp
int32 OverlayCount = Visualizer->GetOverlayInstanceCount();
// OverlayCount = 1 (hover) + 1 (selection) + N (highlights)
```

---

## Usage Patterns

### Pattern: Grid Hover Feedback
```cpp
void AKalkiPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateGridHover();
}

void AKalkiPlayerController::UpdateGridHover()
{
    // Get world position under cursor
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    if (!GridManager || !Visualizer)
    {
        return;
    }

    FKalkiGridCoord NewHover;
    bool bValidHover = false;

    if (Hit.bBlockingHit)
    {
        NewHover = GridManager->WorldPositionToCoord(Hit.Location);
        bValidHover = GridManager->IsValidCoord(NewHover);
    }

    // Update hover if changed
    if (bValidHover)
    {
        if (NewHover != CurrentHoveredCoord)
        {
            CurrentHoveredCoord = NewHover;
            Visualizer->SetHoveredTile(NewHover);

            // Show tile info UI
            ShowTileInfoWidget(NewHover);
        }
    }
    else
    {
        if (CurrentHoveredCoord.IsValid())
        {
            CurrentHoveredCoord = FKalkiGridCoord(-1, -1);
            Visualizer->ClearHoveredTile();
            HideTileInfoWidget();
        }
    }
}
```

---

### Pattern: Click to Select
```cpp
void AKalkiPlayerController::OnMouseLeftClick()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (!Hit.bBlockingHit)
    {
        return;
    }

    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    FKalkiGridCoord ClickedCoord = GridManager->WorldPositionToCoord(Hit.Location);

    if (!GridManager->IsValidCoord(ClickedCoord))
    {
        return;
    }

    // Select tile
    Visualizer->SetSelectedTile(ClickedCoord);
    CurrentSelectedCoord = ClickedCoord;

    // Check if character on tile
    FKalkiGridTile Tile = GridManager->GetTile(ClickedCoord);
    if (Tile.IsOccupied())
    {
        if (AKalkiCharacter* Character = Cast<AKalkiCharacter>(Tile.Occupant.Get()))
        {
            SelectCharacter(Character);
        }
    }
    else
    {
        // Empty tile clicked - move selected character there
        if (SelectedCharacter)
        {
            SelectedCharacter->MoveTo(ClickedCoord);
        }
    }
}
```

---

### Pattern: Movement Range Preview
```cpp
void AKalkiCharacter::OnSelected()
{
    // Show movement range when character selected
    ShowMovementRange();
}

void AKalkiCharacter::ShowMovementRange()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Get reachable tiles
    TArray<FKalkiGridCoord> Reachable = GridManager->GetTilesInRange(
        GridPosition,
        MovementRange,
        true  // Only walkable
    );

    // Filter out occupied tiles
    TArray<FKalkiGridCoord> ValidTargets;
    for (const FKalkiGridCoord& Coord : Reachable)
    {
        FKalkiGridTile Tile = GridManager->GetTile(Coord);
        if (!Tile.IsOccupied() || Coord == GridPosition)
        {
            ValidTargets.Add(Coord);
        }
    }

    // Highlight valid movement targets
    Visualizer->SetHighlightedTiles(ValidTargets);
}

void AKalkiCharacter::OnDeselected()
{
    // Clear movement range when deselected
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();
    Visualizer->ClearHighlightedTiles();
}
```

---

### Pattern: Ability Targeting
```cpp
void UKalkiAbility::StartTargeting()
{
    bIsTargeting = true;
    
    // Show valid targets
    ShowValidTargets();
}

void UKalkiAbility::ShowValidTargets()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    AKalkiCharacter* Caster = GetOwningCharacter();
    
    // Get tiles in ability range
    TArray<FKalkiGridCoord> InRange = GridManager->GetTilesInRange(
        Caster->GetGridPosition(),
        AbilityRange
    );

    // Filter based on targeting rules
    TArray<FKalkiGridCoord> ValidTargets;
    for (const FKalkiGridCoord& Coord : InRange)
    {
        if (IsValidTarget(Coord))
        {
            ValidTargets.Add(Coord);
        }
    }

    // Highlight valid targets
    Visualizer->SetHighlightedTiles(ValidTargets);
}

void UKalkiAbility::UpdateTargeting(const FKalkiGridCoord& HoveredCoord)
{
    if (!bIsTargeting)
    {
        return;
    }

    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Show area of effect preview
    if (AreaOfEffect > 0)
    {
        UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
        TArray<FKalkiGridCoord> AffectedTiles = GridManager->GetTilesInRange(
            HoveredCoord,
            AreaOfEffect
        );

        // Use different color for AoE preview
        FLinearColor OldColor = Visualizer->HighlightColor;
        Visualizer->HighlightColor = FLinearColor(1, 0.5f, 0, 0.4f);  // Orange
        Visualizer->SetHighlightedTiles(AffectedTiles);
        Visualizer->HighlightColor = OldColor;
    }
}

void UKalkiAbility::CancelTargeting()
{
    bIsTargeting = false;
    bIsTargeting = false;
    
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();
    Visualizer->ClearHighlightedTiles();
    Visualizer->ClearHoveredTile();
}

void UKalkiAbility::ConfirmTarget(const FKalkiGridCoord& Target)
{
    if (!IsValidTarget(Target))
    {
        KalkiLog::Abilities(TEXT("Invalid target"), EKalkiLogSeverity::Warning);
        return;
    }

    // Execute ability
    ExecuteAbility(Target);

    // Clear visuals
    CancelTargeting();
}
```

---

### Pattern: Animated Path Preview

```cpp
void AKalkiCharacter::ShowPathToTarget(const FKalkiGridCoord& Target)
{
    UKalkiPathfinding* Pathfinding = GetWorld()->GetSubsystem<UKalkiPathfinding>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Find path
    TArray<FKalkiGridCoord> Path = Pathfinding->FindPath(GridPosition, Target);

    if (Path.Num() == 0)
    {
        // No path - highlight target in red
        Visualizer->HighlightColor = FLinearColor(1, 0, 0, 0.5f);
        Visualizer->SetHighlightedTiles({Target});
        return;
    }

    // Animate path reveal
    AnimatePathReveal(Path);
}

void AKalkiCharacter::AnimatePathReveal(const TArray<FKalkiGridCoord>& Path)
{
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();
    
    // Clear previous path
    Visualizer->ClearHighlightedTiles();

    // Use path color
    Visualizer->HighlightColor = FLinearColor(0, 0.8f, 1, 0.5f);  // Cyan

    // Add tiles one by one
    for (int32 i = 0; i < Path.Num(); ++i)
    {
        // Add tile with delay
        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(
            Handle,
            [this, Visualizer, Path, i]()
            {
                Visualizer->AddHighlightedTile(Path[i]);
            },
            0.05f * i,  // 50ms per tile
            false
        );
    }
}
```

---

### Pattern: Multiple Highlight Colors

```cpp
void AKalkiCombatManager::ShowCombatRanges(AKalkiCharacter* ActiveCharacter)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Get movement range
    TArray<FKalkiGridCoord> MovementRange = GridManager->GetTilesInRange(
        ActiveCharacter->GetGridPosition(),
        ActiveCharacter->GetMovementRange(),
        true
    );

    // Get attack range from current position
    TArray<FKalkiGridCoord> AttackRange = GridManager->GetTilesInRange(
        ActiveCharacter->GetGridPosition(),
        ActiveCharacter->GetAttackRange()
    );

    // Show movement in green
    Visualizer->HighlightColor = FLinearColor(0, 1, 0, 0.4f);
    Visualizer->SetHighlightedTiles(MovementRange);

    // Can't show multiple colors with single visualizer
    // Solution: Spawn second visualizer for attack range
    if (!AttackRangeVisualizer)
    {
        AttackRangeVisualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>(
            AKalkiGridVisualizer::StaticClass()
        );
    }

    // Show attack range in red (on separate visualizer)
    AttackRangeVisualizer->HighlightColor = FLinearColor(1, 0, 0, 0.3f);
    AttackRangeVisualizer->SetHighlightedTiles(AttackRange);
}

void AKalkiCombatManager::ClearCombatRanges()
{
    if (Visualizer)
    {
        Visualizer->ClearHighlightedTiles();
    }

    if (AttackRangeVisualizer)
    {
        AttackRangeVisualizer->ClearHighlightedTiles();
    }
}
```

---

### Pattern: Enemy Turn Indicators

```cpp
void AKalkiAIController::ShowIntentions()
{
    // Show where AI will move and attack
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    if (!Visualizer)
    {
        return;
    }

    // AI's planned move
    FKalkiGridCoord PlannedMove = CalculateOptimalPosition();

    // Tiles AI will threaten after moving
    TArray<FKalkiGridCoord> ThreatenedTiles = GetThreatenedTilesFrom(PlannedMove);

    // Show planned position in yellow
    Visualizer->SelectionColor = FLinearColor(1, 1, 0, 0.6f);
    Visualizer->SetSelectedTile(PlannedMove);

    // Show threatened area in red
    Visualizer->HighlightColor = FLinearColor(1, 0, 0, 0.3f);
    Visualizer->SetHighlightedTiles(ThreatenedTiles);
}

void AKalkiAIController::HideIntentions()
{
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();
    
    if (Visualizer)
    {
        Visualizer->ClearSelectedTile();
        Visualizer->ClearHighlightedTiles();
    }
}
```

---

### Pattern: Custom Visualization Settings

```cpp
void AKalkiGameMode::InitializeGrid()
{
    // Create grid
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->CreateGrid(30, 30, 100.0f, FVector::ZeroVector);

    // Spawn visualizer
    AKalkiGridVisualizer* Visualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>(
        AKalkiGridVisualizer::StaticClass()
    );

    // Customize appearance
    CustomizeVisualizer(Visualizer);
}

void AKalkiGameMode::CustomizeVisualizer(AKalkiGridVisualizer* Visualizer)
{
    // Load custom mesh and material
    Visualizer->TileMesh = LoadObject<UStaticMesh>(
        nullptr,
        TEXT("/Game/Meshes/HexTile.HexTile")
    );

    Visualizer->TileMaterial = LoadObject<UMaterialInterface>(
        nullptr,
        TEXT("/Game/Materials/M_GridTile_Fancy.M_GridTile_Fancy")
    );

    // Custom color scheme
    Visualizer->WalkableColor = FLinearColor(0.9f, 0.9f, 1.0f, 1.0f);  // Light blue-white
    Visualizer->UnwalkableColor = FLinearColor(0.3f, 0.2f, 0.2f, 1.0f);  // Dark red
    Visualizer->ElevatedColor = FLinearColor(0.7f, 0.85f, 1.0f, 1.0f);  // Sky blue

    // Custom overlay colors
    Visualizer->HoverColor = FLinearColor(1, 1, 1, 0.3f);  // White hover
    Visualizer->SelectionColor = FLinearColor(1, 0.8f, 0, 0.7f);  // Gold selection
    Visualizer->HighlightColor = FLinearColor(0, 0.9f, 0.3f, 0.4f);  // Bright green

    // Elevation settings
    Visualizer->bShowElevation = true;
    Visualizer->ElevationScale = 1.5f;  // More pronounced elevation
    Visualizer->TileHeightOffset = 2.0f;  // Prevent Z-fighting
    Visualizer->OverlayHeightOffset = 10.0f;  // Overlays more visible

    // Apply changes
    Visualizer->RefreshAllVisuals();
}
```

---

## Performance Considerations

### Instance Rendering

**Benefits:**

- Single draw call per instance type (base tiles, overlays)
- Efficient GPU memory usage
- Scales well to large grids

**Memory Usage:**

```
Per Instance: ~64 bytes (transform + color)
20×20 grid: 400 tiles × 64 bytes = 25.6 KB
50×50 grid: 2500 tiles × 64 bytes = 160 KB
100×100 grid: 10000 tiles × 64 bytes = 640 KB
```

**Draw Calls:**

- Base tiles: 1 draw call (all tiles)
- Overlays: 1 draw call (all overlays)
- Total: 2 draw calls regardless of grid size

---

### Update Performance

**Operation Performance:**

|Operation|Time|Notes|
|---|---|---|
|`CreateGridVisuals()`|1-10ms|Depends on grid size|
|`SetHoveredTile()`|<0.1ms|Single instance update|
|`SetSelectedTile()`|<0.1ms|Single instance update|
|`SetHighlightedTiles(10)`|<0.5ms|10 instance updates|
|`SetHighlightedTiles(100)`|2-5ms|100 instance updates|
|`UpdateTileVisual()`|<0.1ms|Single instance update|
|`RefreshAllVisuals()`|5-20ms|Updates all instances|

**Optimization Tips:**

1. Minimize `RefreshAllVisuals()` calls
2. Batch highlight updates with `SetHighlightedTiles()` instead of multiple `AddHighlightedTile()` calls
3. Clear highlights when not needed
4. Reuse visualizer instances instead of spawning new ones
5. Use lower poly tile meshes for better performance

---

### LOD Considerations

**For Large Grids (50×50+):**

```cpp
void AKalkiGridVisualizer::SetupLOD()
{
    // Set LOD distances for instances
    if (TileInstancedMeshComponent)
    {
        TileInstancedMeshComponent->InstanceStartCullDistance = 5000.0f;
        TileInstancedMeshComponent->InstanceEndCullDistance = 10000.0f;
    }

    // Reduce detail at distance
    if (OverlayInstancedMeshComponent)
    {
        OverlayInstancedMeshComponent->InstanceStartCullDistance = 3000.0f;
        OverlayInstancedMeshComponent->InstanceEndCullDistance = 6000.0f;
    }
}
```

---

## Common Mistakes

### Mistake 1: Forgetting to Spawn Visualizer

```cpp
// ❌ BAD - Visualizer not spawned
void AGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->CreateGrid(20, 20, 100.0f, FVector::ZeroVector);
    
    // Grid created but nothing renders!
}

// ✅ GOOD - Spawn visualizer
void AGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Spawn visualizer first
    GridVisualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>(
        AKalkiGridVisualizer::StaticClass()
    );

    // Create grid (visualizer auto-detects and renders)
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->CreateGrid(20, 20, 100.0f, FVector::ZeroVector);
}
```

---

### Mistake 2: Not Clearing Highlights

```cpp
// ❌ BAD - Highlights persist forever
void AKalkiCharacter::ShowMovementRange()
{
    TArray<FKalkiGridCoord> Range = GetReachableTiles();
    Visualizer->SetHighlightedTiles(Range);
    // Never cleared!
}

// ✅ GOOD - Clear when done
void AKalkiCharacter::OnSelected()
{
    ShowMovementRange();
}

void AKalkiCharacter::OnDeselected()
{
    Visualizer->ClearHighlightedTiles();
}

void AKalkiCharacter::OnTurnEnd()
{
    Visualizer->ClearHighlightedTiles();
    Visualizer->ClearSelectedTile();
}
```

---

### Mistake 3: Calling RefreshAllVisuals Too Often

```cpp
// ❌ BAD - Refresh every frame
void AMyActor::Tick(float DeltaTime)
{
    Visualizer->RefreshAllVisuals();  // Very expensive!
}

// ✅ GOOD - Only refresh when settings change
void AMyActor::UpdateVisualizerSettings()
{
    Visualizer->WalkableColor = NewColor;
    Visualizer->RefreshAllVisuals();  // Once, when needed
}

// ✅ BETTER - Update single tile
void AMyActor::OnTileChanged(const FKalkiGridCoord& Coord)
{
    Visualizer->UpdateTileVisual(Coord);  // Only affected tile
}
```

---

### Mistake 4: Not Validating Coordinates

```cpp
// ❌ BAD - No validation
void APlayerController::OnMouseClick()
{
    FVector WorldPos = GetCursorWorldPosition();
    FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(WorldPos);
    
    Visualizer->SetSelectedTile(Coord);  // May be invalid!
}

// ✅ GOOD - Validate first
void APlayerController::OnMouseClick()
{
    FVector WorldPos = GetCursorWorldPosition();
    FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(WorldPos);
    
    if (GridManager->IsValidCoord(Coord))
    {
        Visualizer->SetSelectedTile(Coord);
    }
}
```

---

### Mistake 5: Multiple Visualizers Fighting

```cpp
// ❌ BAD - Multiple visualizers for same grid
void AGameMode::BeginPlay()
{
    // Spawn visualizer 1
    Visualizer1 = GetWorld()->SpawnActor<AKalkiGridVisualizer>();
    
    // Spawn visualizer 2
    Visualizer2 = GetWorld()->SpawnActor<AKalkiGridVisualizer>();
    
    // Both try to render same tiles - Z-fighting!
}

// ✅ GOOD - Single visualizer or separate purposes
void AGameMode::BeginPlay()
{
    // Main grid visualizer
    MainVisualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>();
    MainVisualizer->TileHeightOffset = 0.0f;
    
    // Optional: Second visualizer for special overlays only
    OverlayVisualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>();
    OverlayVisualizer->TileHeightOffset = 10.0f;  // Render above main
    // Don't call CreateGridVisuals() on overlay visualizer
}
```

---

## Troubleshooting

### Grid Not Rendering

**Problem:** Grid created but tiles not visible

**Solutions:**

1. Check if visualizer spawned: `GetWorld()->GetActorOf<AKalkiGridVisualizer>()`
2. Verify `CreateGridVisuals()` called (check log)
3. Check if `TileMesh` and `TileMaterial` are set
4. Verify camera can see grid area
5. Check if instances have valid transforms: `GetInstanceCount() > 0`
6. Try `RefreshAllVisuals()` to force update

---

### Highlights Not Showing

**Problem:** SetHighlightedTiles() called but no visual change

**Solutions:**

1. Check if `OverlayInstancedMeshComponent` exists
2. Verify `HighlightColor` has alpha > 0
3. Check if overlay mesh/material supports transparency
4. Verify coordinates are valid
5. Check `OverlayHeightOffset` - may be too low (Z-fighting)
6. Try increasing alpha: `HighlightColor.A = 0.8f`

---

### Z-Fighting Issues

**Problem:** Tiles flickering or overlapping

**Solutions:**

1. Increase `TileHeightOffset`: `TileHeightOffset = 1.0f`
2. Increase `OverlayHeightOffset`: `OverlayHeightOffset = 10.0f`
3. Check for multiple visualizers at same height
4. Ensure tile mesh is thin (not too thick in Z)
5. Verify grid origin Z is reasonable

---

### Performance Issues

**Problem:** Low framerate with visualizer active

**Solutions:**

1. Check grid size - reduce if > 100×100
2. Verify using instanced meshes (not individual actors)
3. Reduce poly count of tile mesh
4. Enable LOD: Set `InstanceStartCullDistance` and `InstanceEndCullDistance`
5. Minimize `RefreshAllVisuals()` calls
6. Clear highlights when not needed
7. Check for multiple visualizers rendering unnecessarily

---

### Colors Not Applying

**Problem:** Changed color properties but no visual change

**Solutions:**

1. Call `RefreshAllVisuals()` after changing colors
2. Verify material supports color parameters
3. Check if material has correct parameter names
4. Try assigning material at runtime
5. Verify material is not overriding colors

---

## Related Documents

### Architecture

- [[03_Systems/Grid_Visualizer]] - System design and patterns
- [[03_Systems/Grid_System]] - Grid architecture
- [[04_Implementation/Common_Patterns]] - Usage patterns

### Related APIs

- [[05_API_Reference/KalkiGridManager_API]] - Grid manager API
- [[05_API_Reference/KalkiGridCoord_API]] - Coordinate API
- [[05_API_Reference/KalkiGridTile_API]] - Tile data API

### Implementation

- [[04_Implementation/Error_Handling]] - Validation patterns
- [[04_Implementation/Testing_Procedures]] - Testing visualizer

---

## Quick Reference

### Essential Functions

```cpp
// Get visualizer (assumes spawned)
AKalkiGridVisualizer* Viz = FindVisualizerInWorld();

// Hover
Viz->SetHoveredTile(Coord);
Viz->ClearHoveredTile();

// Selection
Viz->SetSelectedTile(Coord);
Viz->ClearSelectedTile();

// Highlights
Viz->SetHighlightedTiles(Coords);
Viz->AddHighlightedTile(Coord);
Viz->RemoveHighlightedTile(Coord);
Viz->ClearHighlightedTiles();

// Updates
Viz->UpdateTileVisual(Coord);
Viz->RefreshAllVisuals();

// Customize colors
Viz->HoverColor = FLinearColor(0, 0.5f, 1, 0.5f);
Viz->SelectionColor = FLinearColor(1, 1, 0, 0.6f);
Viz->HighlightColor = FLinearColor(0, 1, 0, 0.4f);
Viz->RefreshAllVisuals();
```

---

## Document Info

**File Path:** `Kalki_GDD/05_API_Reference/KalkiGridVisualizer_API.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate  
**Note:** Complete API reference for KalkiGridVisualizer actor