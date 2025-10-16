---
tags:
  - kalki/system/grid-visualization
  - kalki/rendering
  - kalki/phase-2
  - type: reference
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Grid Visualization

## Overview
The Grid Visualization System renders the tactical grid using Unreal Engine's Instanced Static Mesh Component for performance. It provides visual feedback for hover, selection, movement range, and tile states with dynamic coloring and efficient rendering.

---

## Purpose

### Problems Solved
1. **Performance** - Render 400+ tiles efficiently (1-2 draw calls)
2. **Visual Feedback** - Show hover, selection, range highlights
3. **State Indication** - Visual distinction between walkable/unwalkable tiles
4. **Dynamic Updates** - Real-time response to grid changes
5. **Mouse Interaction** - Raycast to determine hovered tile

### Design Goals
- ✅ **High Performance** - GPU instancing for minimal draw calls
- ✅ **Clear Visual Language** - Intuitive color coding
- ✅ **Real-time Updates** - Instant feedback on state changes
- ✅ **Configurable** - Designer-friendly settings in Blueprint
- ✅ **Extensible** - Easy to add new visual states

---

## Architecture

### Class Hierarchy
```
AActor (Unreal)
    ↓
AKalkiGridVisualizer
    ├─ UInstancedStaticMeshComponent (TileMeshComponent)
    ├─ Configuration (colors, materials, meshes)
    └─ Mouse interaction (raycasting)
```

---

### Rendering Pipeline
```
┌──────────────────────────────────────────┐
│  GridManager (Data)                      │
│  - Tile positions                        │
│  - Tile states (walkable, elevation)     │
└──────────────┬───────────────────────────┘
               │
               ↓
┌──────────────────────────────────────────┐
│  GridVisualizer (Rendering)              │
│  - Create mesh instances                 │
│  - Set custom data (color)               │
│  - Update on tile changes                │
└──────────────┬───────────────────────────┘
               │
               ↓
┌──────────────────────────────────────────┐
│  InstancedStaticMeshComponent            │
│  - 1-2 draw calls for entire grid        │
│  - Per-instance custom data (RGBA)       │
│  - GPU instancing                        │
└──────────────────────────────────────────┘
```

---

## Key Class: AKalkiGridVisualizer

**Type:** Actor  
**Path:** `Source/Kalki/Public/Grid/KalkiGridVisualizer.h`  
**Lifetime:** Spawned by LevelManager, one per level

**Purpose:**
- Render all grid tiles using instanced meshes
- Provide visual feedback (hover, selection, range)
- Handle mouse interaction (raycasting)
- Update visuals on grid changes
- Apply color coding for tile states

---

### Key Components

**TileMeshComponent:**
```cpp
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid")
TObjectPtr<UInstancedStaticMeshComponent> TileMeshComponent;
```

**Purpose:** Renders all tiles as instances of a single mesh

**Benefits:**
- 1 draw call for entire grid (assuming same material)
- GPU instancing (fast)
- Per-instance custom data (RGBA color)

---

### Configuration Properties

**Mesh and Material:**
```cpp
// Tile mesh (1x1 square plane by default)
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
TObjectPtr<UStaticMesh> TileMesh;

// Base material for tiles
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
TObjectPtr<UMaterialInterface> TileMaterial;
```

---

**Color Configuration:**
```cpp
// Default tile color (walkable, normal state)
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
FLinearColor DefaultTileColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

// Unwalkable tile color
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
FLinearColor UnwalkableTileColor = FLinearColor(0.5f, 0.1f, 0.1f, 1.0f);

// Hover highlight color
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
FLinearColor HoverTileColor = FLinearColor(0.3f, 0.6f, 1.0f, 1.0f);

// Selection color
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
FLinearColor SelectedTileColor = FLinearColor(0.3f, 1.0f, 0.3f, 1.0f);

// Movement range color
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
FLinearColor RangeTileColor = FLinearColor(1.0f, 1.0f, 0.3f, 0.5f);

// Elevated tile color modifier
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
float ElevationColorMultiplier = 1.2f;
```

---

**Visibility:**
```cpp
// Show/hide grid
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
bool bShowGrid = true;

// Show elevation differences with color intensity
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
bool bShowElevation = true;
```

---

### Key Functions

**Initialization:**
```cpp
// Create all tile instances
void CreateTileInstances();

// Update a single tile's visual
void UpdateTileVisual(const FKalkiGridCoord& Coord);

// Refresh entire grid
void RefreshGridVisuals();
```

---

**Mouse Interaction:**
```cpp
// Get tile under mouse cursor
FKalkiGridCoord GetTileUnderCursor() const;

// Raycast to grid
bool RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord) const;
```

---

**Visual Feedback:**
```cpp
// Highlight tile on hover
void SetHoveredTile(const FKalkiGridCoord& Coord);
void ClearHoveredTile();

// Select tile
void SetSelectedTile(const FKalkiGridCoord& Coord);
void ClearSelectedTile();

// Show movement range
void ShowMovementRange(const TArray<FKalkiGridCoord>& Tiles);
void ClearMovementRange();
```

---

**Color Application:**
```cpp
// Get color for tile state
FLinearColor GetTileColor(const FKalkiGridCoord& Coord) const;

// Apply color to instance
void SetInstanceColor(int32 InstanceIndex, const FLinearColor& Color);
```

---

## Instanced Static Mesh Component

### What is Instancing?

**Traditional Rendering (Bad for grids):**
```
Draw Tile 1 (1 draw call)
Draw Tile 2 (1 draw call)
Draw Tile 3 (1 draw call)
...
Draw Tile 400 (1 draw call)
---
Total: 400 draw calls (very slow!)
```

**Instanced Rendering (Good for grids):**
```
Draw All 400 Tiles (1 draw call)
---
Total: 1 draw call (fast!)
```

---

### How It Works

**GPU Instancing:**
- Upload mesh data once
- GPU renders same mesh multiple times
- Each instance has unique transform + custom data
- All instances rendered in single draw call

**Custom Data:**
- 4 floats per instance (RGBA)
- Used for per-instance color
- Set via `SetCustomDataValue(InstanceIndex, Slot, Value)`

---

### Setup in C++
```cpp
void AKalkiGridVisualizer::BeginPlay()
{
    Super::BeginPlay();

    // Create instanced mesh component
    TileMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TileMeshComponent"));
    RootComponent = TileMeshComponent;

    // Set mesh
    if (TileMesh)
    {
        TileMeshComponent->SetStaticMesh(TileMesh);
    }

    // Set material
    if (TileMaterial)
    {
        TileMeshComponent->SetMaterial(0, TileMaterial);
    }

    // Configure for custom data
    TileMeshComponent->NumCustomDataFloats = 4; // RGBA

    // Get GridManager
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::Grid(TEXT("GridVisualizer - GridManager not found"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Bind to grid events
    GridManager->OnGridCreated.AddDynamic(this, &AKalkiGridVisualizer::OnGridCreated);
    GridManager->OnTileChanged.AddDynamic(this, &AKalkiGridVisualizer::OnTileChanged);

    // Create tile instances if grid exists
    if (GridManager->IsGridCreated())
    {
        CreateTileInstances();
    }
}
```

---

### Creating Instances
```cpp
void AKalkiGridVisualizer::CreateTileInstances()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager || !GridManager->IsGridCreated())
    {
        return;
    }

    // Clear existing instances
    TileMeshComponent->ClearInstances();

    // Create instance for each tile
    for (int32 Y = 0; Y < GridManager->GetGridSizeY(); ++Y)
    {
        for (int32 X = 0; X < GridManager->GetGridSizeX(); ++X)
        {
            FKalkiGridCoord Coord(X, Y);
            
            // Get tile data
            FKalkiGridTile Tile = GridManager->GetTile(Coord);
            
            // Calculate transform
            FVector Location = Tile.WorldPosition;
            FRotator Rotation = FRotator::ZeroRotator;
            FVector Scale = FVector(1.0f, 1.0f, 1.0f);
            FTransform Transform(Rotation, Location, Scale);
            
            // Add instance
            int32 InstanceIndex = TileMeshComponent->AddInstance(Transform);
            
            // Set color via custom data
            FLinearColor TileColor = GetTileColor(Coord);
            SetInstanceColor(InstanceIndex, TileColor);
        }
    }

    KalkiLog::Grid(
        FString::Printf(TEXT("GridVisualizer - Created %d tile instances"), 
            TileMeshComponent->GetInstanceCount())
    );
}
```

---

### Setting Instance Color
```cpp
void AKalkiGridVisualizer::SetInstanceColor(int32 InstanceIndex, const FLinearColor& Color)
{
    if (!TileMeshComponent)
    {
        return;
    }

    // Custom data slots: 0=R, 1=G, 2=B, 3=A
    TileMeshComponent->SetCustomDataValue(InstanceIndex, 0, Color.R);
    TileMeshComponent->SetCustomDataValue(InstanceIndex, 1, Color.G);
    TileMeshComponent->SetCustomDataValue(InstanceIndex, 2, Color.B);
    TileMeshComponent->SetCustomDataValue(InstanceIndex, 3, Color.A);

    // Mark render state dirty to update visuals
    TileMeshComponent->MarkRenderStateDirty();
}
```

---

## Material Setup

### Material Graph

**Material must read custom data for per-instance color:**
```
Material Graph (M_GridTile):

[PerInstanceCustomData] → [R, G, B, A]
         ↓
[Multiply] ← [BaseColor Texture] (optional)
         ↓
[Base Color Output]

[PerInstanceCustomData.A] → [Opacity Output]
```

**Nodes:**
1. **PerInstanceCustomData** - Reads custom data from instance
2. **Multiply** - Combine with texture (optional)
3. **Base Color** - Output final color
4. **Opacity** - Use alpha for transparency (range highlights)

---

### Material Properties

**Material Settings:**
- Blend Mode: `Translucent` (for alpha blending)
- Shading Model: `Unlit` or `Default Lit`
- Two Sided: `True` (optional, depends on mesh)

**Why Translucent:**
- Allows alpha-based transparency
- Used for range highlights (semi-transparent yellow)
- Used for hover effects

---

## Color Coding System

### Default Color Scheme
```cpp
FLinearColor AKalkiGridVisualizer::GetTileColor(const FKalkiGridCoord& Coord) const
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager || !GridManager->IsValidCoord(Coord))
    {
        return FLinearColor::White;
    }

    FKalkiGridTile Tile = GridManager->GetTile(Coord);
    FLinearColor BaseColor;

    // 1. Check special states first
    
    // Selected tile (highest priority)
    if (Coord == SelectedTileCoord && bHasSelection)
    {
        return SelectedTileColor;
    }

    // Hovered tile
    if (Coord == HoveredTileCoord && bHasHover)
    {
        return HoverTileColor;
    }

    // In movement range
    if (RangeTiles.Contains(Coord))
    {
        return RangeTileColor;
    }

    // 2. Base color from walkability
    if (Tile.bWalkable)
    {
        BaseColor = DefaultTileColor;
    }
    else
    {
        BaseColor = UnwalkableTileColor;
    }

    // 3. Modify for elevation (if enabled)
    if (bShowElevation && Tile.Elevation > 0.0f)
    {
        // Brighter tiles = higher elevation
        float ElevationFactor = Tile.Elevation / 500.0f; // Normalize to ~0-1 range
        BaseColor *= (1.0f + ElevationFactor * ElevationColorMultiplier);
    }

    return BaseColor;
}
```

---

### Visual States Priority

**Priority (highest to lowest):**
1. **Selected** - Bright green (user explicitly selected)
2. **Hovered** - Bright blue (mouse is over tile)
3. **In Range** - Semi-transparent yellow (reachable tiles)
4. **Unwalkable** - Dark red (blocked tiles)
5. **Default** - Dark gray (normal walkable tiles)
6. **Elevated** - Brighter than base (height indicator)

---

## Mouse Interaction

### Raycasting to Grid
```cpp
FKalkiGridCoord AKalkiGridVisualizer::GetTileUnderCursor() const
{
    FVector HitLocation;
    FKalkiGridCoord Coord;
    
    if (RaycastToGrid(HitLocation, Coord))
    {
        return Coord;
    }
    
    return FKalkiGridCoord(-1, -1); // Invalid coord
}

bool AKalkiGridVisualizer::RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord) const
{
    // Get player controller
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        return false;
    }

    // Get mouse position
    FVector2D MousePosition;
    if (!PC->GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        return false;
    }

    // Convert to world ray
    FVector WorldLocation, WorldDirection;
    if (!PC->DeprojectScreenPositionToWorld(
        MousePosition.X, 
        MousePosition.Y, 
        WorldLocation, 
        WorldDirection))
    {
        return false;
    }

    // Raycast parameters
    FHitResult HitResult;
    FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    // Perform raycast
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        WorldLocation,
        TraceEnd,
        ECC_Visibility,
        QueryParams
    );

    if (bHit)
    {
        OutHitLocation = HitResult.Location;
        
        // Convert world position to grid coordinate
        UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
        if (GridManager)
        {
            OutCoord = GridManager->WorldPositionToCoord(OutHitLocation);
            return GridManager->IsValidCoord(OutCoord);
        }
    }

    return false;
}
```

---

### Tick for Hover Updates
```cpp
void AKalkiGridVisualizer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bShowGrid)
    {
        return;
    }

    // Get tile under cursor
    FKalkiGridCoord NewHoveredCoord = GetTileUnderCursor();

    // Check if hover changed
    if (NewHoveredCoord != HoveredTileCoord)
    {
        // Clear old hover
        if (bHasHover)
        {
            UpdateTileVisual(HoveredTileCoord);
        }

        // Set new hover
        HoveredTileCoord = NewHoveredCoord;
        bHasHover = (HoveredTileCoord.X >= 0 && HoveredTileCoord.Y >= 0);

        if (bHasHover)
        {
            UpdateTileVisual(HoveredTileCoord);
            
            // Notify other systems (e.g., GridInfoWidget)
            OnTileHovered.Broadcast(HoveredTileCoord);
        }
    }
}
```

---

## Event Handling

### Grid Events
```cpp
// Bind to GridManager events in BeginPlay:
GridManager->OnGridCreated.AddDynamic(this, &AKalkiGridVisualizer::OnGridCreated);
GridManager->OnTileChanged.AddDynamic(this, &AKalkiGridVisualizer::OnTileChanged);

// Event handlers:
void AKalkiGridVisualizer::OnGridCreated()
{
    KalkiLog::Grid(TEXT("GridVisualizer - Grid created, creating instances"));
    CreateTileInstances();
}

void AKalkiGridVisualizer::OnTileChanged(const FKalkiGridCoord& Coord)
{
    KalkiLog::Grid(
        FString::Printf(TEXT("GridVisualizer - Tile changed: (%d, %d)"), Coord.X, Coord.Y)
    );
    UpdateTileVisual(Coord);
}
```

---

### Custom Events
```cpp
// Declare in header:
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileHovered, const FKalkiGridCoord&, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileClicked, const FKalkiGridCoord&, Coord);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnTileHovered OnTileHovered;

UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
FOnTileClicked OnTileClicked;

// Broadcast when appropriate:
void AKalkiGridVisualizer::Tick(float DeltaTime)
{
    // ... hover detection ...
    
    if (bHasHover)
    {
        OnTileHovered.Broadcast(HoveredTileCoord);
    }
}

void AKalkiGridVisualizer::HandleMouseClick()
{
    if (bHasHover)
    {
        OnTileClicked.Broadcast(HoveredTileCoord);
    }
}
```

---

## Visual Feedback Examples

### Movement Range Preview
```cpp
void AKalkiGridVisualizer::ShowMovementRange(const TArray<FKalkiGridCoord>& Tiles)
{
    // Clear old range
    ClearMovementRange();

    // Store new range tiles
    RangeTiles = TSet<FKalkiGridCoord>(Tiles);

    // Update visuals for all range tiles
    for (const FKalkiGridCoord& Coord : RangeTiles)
    {
        UpdateTileVisual(Coord);
    }

    KalkiLog::Grid(
        FString::Printf(TEXT("GridVisualizer - Showing range: %d tiles"), RangeTiles.Num())
    );
}

void AKalkiGridVisualizer::ClearMovementRange()
{
    if (RangeTiles.Num() == 0)
    {
        return;
    }

    // Update visuals for all tiles that were in range
    for (const FKalkiGridCoord& Coord : RangeTiles)
    {
        UpdateTileVisual(Coord);
    }

    RangeTiles.Empty();
    
    KalkiLog::Grid(TEXT("GridVisualizer - Range cleared"));
}
```

**Usage:**
```cpp
// From character movement system:
TArray<FKalkiGridCoord> ReachableTiles = GridManager->GetTilesInRange(CharacterPos, MovementRange, true);
GridVisualizer->ShowMovementRange(ReachableTiles);
```

---

### Selection Feedback
```cpp
void AKalkiGridVisualizer::SetSelectedTile(const FKalkiGridCoord& Coord)
{
    // Clear old selection
    if (bHasSelection)
    {
        UpdateTileVisual(SelectedTileCoord);
    }

    // Set new selection
    SelectedTileCoord = Coord;
    bHasSelection = true;

    // Update visual
    UpdateTileVisual(SelectedTileCoord);

    KalkiLog::Grid(
        FString::Printf(TEXT("GridVisualizer - Tile selected: (%d, %d)"), Coord.X, Coord.Y)
    );
}

void AKalkiGridVisualizer::ClearSelectedTile()
{
    if (!bHasSelection)
    {
        return;
    }

    // Update visual
    UpdateTileVisual(SelectedTileCoord);

    bHasSelection = false;
    SelectedTileCoord = FKalkiGridCoord(-1, -1);

    KalkiLog::Grid(TEXT("GridVisualizer - Selection cleared"));
}
```

---

## Performance Optimization

### Draw Call Reduction

**Before (Naive Approach):**
```cpp
// 400 tiles × 1 draw call each = 400 draw calls
for (int32 i = 0; i < 400; ++i)
{
    DrawTile(i); // 1 draw call
}
```

**After (Instanced Rendering):**
```cpp
// 400 tiles × 1 draw call total = 1 draw call
DrawAllInstances(); // 1 draw call
```

**Performance Gain:** ~400× fewer draw calls!

---

### Update Optimization

**Only update changed tiles:**
```cpp
void AKalkiGridVisualizer::UpdateTileVisual(const FKalkiGridCoord& Coord)
{
    // Calculate instance index
    int32 InstanceIndex = CoordToInstanceIndex(Coord);
    
    if (InstanceIndex < 0 || InstanceIndex >= TileMeshComponent->GetInstanceCount())
    {
        return;
    }

    // Get new color
    FLinearColor NewColor = GetTileColor(Coord);
    
    // Update only this instance
    SetInstanceColor(InstanceIndex, NewColor);
}

int32 AKalkiGridVisualizer::CoordToInstanceIndex(const FKalkiGridCoord& Coord) const
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        return -1;
    }

    // Instances are created in row-major order (Y outer, X inner)
    return Coord.Y * GridManager->GetGridSizeX() + Coord.X;
}
```

---

### Batch Updates

**For multiple tile changes:**
```cpp
void AKalkiGridVisualizer::UpdateMultipleTiles(const TArray<FKalkiGridCoord>& Coords)
{
    // Suppress render state updates
    TileMeshComponent->SetComponentTickEnabled(false);

    // Update all tiles
    for (const FKalkiGridCoord& Coord : Coords)
    {
        UpdateTileVisual(Coord);
    }

    // Re-enable and mark dirty once
    TileMeshComponent->SetComponentTickEnabled(true);
    TileMeshComponent->MarkRenderStateDirty();
}
```

---

## Blueprint Configuration

### BP_GridVisualizer Setup

**Class Settings:**
- Parent Class: `KalkiGridVisualizer`
- Spawned by: `AKalkiLevelManager`

**Editable Properties (Designer-Friendly):**

**Visuals:**
- Tile Mesh: `SM_GridTile_1x1`
- Tile Material: `M_GridTile`
- Show Grid: `True`
- Show Elevation: `True`

**Colors:**
- Default Tile Color: `RGB(51, 51, 51)` - Dark gray
- Unwalkable Tile Color: `RGB(128, 25, 25)` - Dark red
- Hover Tile Color: `RGB(76, 153, 255)` - Bright blue
- Selected Tile Color: `RGB(76, 255, 76)` - Bright green
- Range Tile Color: `RGB(255, 255, 76, 128)` - Semi-transparent yellow
- Elevation Color Multiplier: `1.2`

---

### Testing in Editor

**PIE Testing:**
1. Play in editor
2. Hover mouse over grid → tiles highlight blue
3. Click tile → tile turns green (selection)
4. Check console for logs
5. Press `~` and type `ShowGridVisualizer false` → grid hides

**Console Commands:**
```
ShowGridVisualizer true/false
SetHoverColor 0.5 0.5 1.0 1.0
RefreshGridVisuals
```

---

## Common Patterns

### Pattern 1: Show Action Range
```cpp
// Character wants to show movement range
void ShowCharacterMovementRange(AKalkiCharacter* Character)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Get character position
    FKalkiGridCoord CharPos = Character->GetGridPosition();

    // Get tiles in movement range
    int32 MovementRange = Character->GetMovementRange();
    TArray<FKalkiGridCoord> ReachableTiles = GridManager->GetTilesInRange(
        CharPos, 
        MovementRange, 
        true // Only walkable
    );

    // Show range
    Visualizer->ShowMovementRange(ReachableTiles);
}
```

---

### Pattern 2: Show Ability Target Area
```cpp
// Show valid ability targets
void ShowAbilityTargetArea(const FKalkiAbility& Ability, const FKalkiGridCoord& CasterPos)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Get tiles in ability range
    TArray<FKalkiGridCoord> ValidTargets = GridManager->GetTilesInRange(
        CasterPos,
        Ability.Range
    );

    // Filter by line of sight (if needed)
    // ... filtering logic ...

    // Show targets
    Visualizer->ShowMovementRange(ValidTargets); // Reuse range display
}
```

---

### Pattern 3: Highlight Path
```cpp
// Show pathfinding result
void ShowPath(const TArray<FKalkiGridCoord>& Path)
{
    AKalkiGridVisualizer* Visualizer = GetGridVisualizer();

    // Clear any existing range display
    Visualizer->ClearMovementRange();

    // Show path as range (yellow highlight)
    Visualizer->ShowMovementRange(Path);
}
```

---

## Troubleshooting

### Grid Not Visible

**Check:**
1. ✅ GridVisualizer spawned? (Check World Outliner)
2. ✅ `bShowGrid = true`?
3. ✅ Tile mesh assigned?
4. ✅ Tile material assigned?
5. ✅ Instances created? (Check `GetInstanceCount()`)
6. ✅ Camera can see grid? (Check camera position/angle)

**Debug Commands:**
```cpp
PrintGridVisualizerInfo  // Shows state, instance count, etc.
```

---

### Colors Not Updating

**Check:**
1. ✅ `MarkRenderStateDirty()` called after color change?
2. ✅ Material reads `PerInstanceCustomData`?
3. ✅ `NumCustomDataFloats = 4` set?
4. ✅ Instance index valid?

**Debug:**
```cpp
KalkiLog::Grid(
    FString::Printf(TEXT("Setting color for instance %d: R=%.2f G=%.2f B=%.2f"), 
        InstanceIndex, Color.R, Color.G, Color.B)
);
```

---

### Hover Not Working

**Check:**
1. ✅ Tick enabled? (`PrimaryActorTick.bCanEverTick = true`)
2. ✅ Raycast hitting grid? (Debug draw line trace)
3. ✅ Collision enabled on tile mesh?
4. ✅ Player controller exists?

**Debug:**
```cpp
// In Tick:
DrawDebugLine(
    GetWorld(),
    WorldLocation,
    TraceEnd,
    FColor::Red,
    false,
    0.1f
);
```

---

## Future Enhancements

### Planned Features

**1. Multiple Visual Layers**
```cpp
// Different meshes for different states
UInstancedStaticMeshComponent* WalkableTileMesh;
UInstancedStaticMeshComponent* UnwalkableTileMesh;
UInstancedStaticMeshComponent* ElevatedTileMesh;
```

**2. Tile Animations**
```cpp
// Pulse effect on selection
void AnimateSelectedTile(float DeltaTime);
```

**3. Decals/Overlays**
```cpp
// Show icons on tiles (objective markers, hazards)
void AddTileDecal(const FKalkiGridCoord& Coord, UTexture2D* Icon);
```

**4.**4. Line of Sight Visualization**
```cpp
// Show line of sight from character
void ShowLineOfSight(const FKalkiGridCoord& Origin, int32 Range);

// Highlight visible vs hidden tiles
FLinearColor VisibleTileColor = FLinearColor(0.3f, 1.0f, 0.3f, 0.5f);
FLinearColor HiddenTileColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.3f);
```

**5. Cover Indicators**
```cpp
// Show cover rating on tiles
enum class EKalkiCoverType : uint8
{
    None,
    Half,
    Full
};

void ShowCoverIndicators(const FKalkiGridCoord& AttackerPos);
```

**6. Area of Effect Preview**
```cpp
// Show AoE spell radius
void ShowAoEArea(const FKalkiGridCoord& Center, int32 Radius, EKalkiAoEShape Shape);

enum class EKalkiAoEShape : uint8
{
    Circle,      // Fireball
    Cone,        // Burning Hands
    Line,        // Lightning Bolt
    Square       // Cloud spells
};
```

**7. Height Indicators**
```cpp
// Visual pillars showing elevation difference
void ShowElevationPillars(bool bShow);

// Different tile heights (actual Z positioning)
void SetTileHeight(const FKalkiGridCoord& Coord, float Height);
```

**8. Particle Effects**
```cpp
// Spawn particles on tiles
void SpawnTileEffect(const FKalkiGridCoord& Coord, UParticleSystem* Effect);

// Examples:
// - Magic circle on cast target
// - Fire on hazard tiles
// - Sparkles on loot tiles
```

---

## Related Documents

### Core Systems
- [[03_Systems/Grid_System]] - Grid data and logic
- [[05_API_Reference/KalkiGridManager_API]] - Grid Manager API
- [[05_API_Reference/KalkiGridVisualizer_API]] - Visualizer API (future)

### Architecture
- [[01_Architecture/Module_Overview]] - Grid subsystem
- [[01_Architecture/Project_Structure]] - File locations

### Implementation
- [[04_Implementation/Common_Patterns]] - Rendering patterns
- [[04_Implementation/Performance_Guidelines]] - Optimization tips

### Technical
- [[00_Overview/Technical_Stack]] - Rendering pipeline details

---

## Quick Reference

### Common Tasks

**Show/Hide Grid:**
```cpp
GridVisualizer->SetShowGrid(true/false);
```

**Get Hovered Tile:**
```cpp
FKalkiGridCoord HoveredCoord = GridVisualizer->GetTileUnderCursor();
```

**Show Movement Range:**
```cpp
TArray<FKalkiGridCoord> Range = GridManager->GetTilesInRange(Origin, 5, true);
GridVisualizer->ShowMovementRange(Range);
```

**Clear Range:**
```cpp
GridVisualizer->ClearMovementRange();
```

**Select Tile:**
```cpp
GridVisualizer->SetSelectedTile(Coord);
```

**Update Single Tile:**
```cpp
GridVisualizer->UpdateTileVisual(Coord);
```

**Refresh Entire Grid:**
```cpp
GridVisualizer->RefreshGridVisuals();
```

---

### Color Customization

**Change colors in Blueprint:**
1. Select `BP_GridVisualizer` in World Outliner
2. In Details panel, find "Colors" section
3. Modify color properties
4. Colors update in real-time (PIE)

**Change colors at runtime:**
```cpp
GridVisualizer->DefaultTileColor = FLinearColor(0.1f, 0.1f, 0.1f);
GridVisualizer->HoverTileColor = FLinearColor(1.0f, 0.5f, 0.0f); // Orange hover
GridVisualizer->RefreshGridVisuals(); // Apply changes
```

---

### Material Customization

**Create custom tile material:**

1. **Duplicate** `M_GridTile`
2. **Add** texture parameters (optional)
3. **Modify** PerInstanceCustomData usage
4. **Assign** to `BP_GridVisualizer`

**Example: Add grid lines**
```
Material Graph:

[PerInstanceCustomData] → [Color]
         ↓
[Multiply] ← [GridLines Texture]
         ↓
[Base Color]
```

---

### Debug Commands

**Console commands for testing:**
```cpp
// Show/hide grid
ShowGridVisualizer true
ShowGridVisualizer false

// Refresh visuals
RefreshGridVisuals

// Print debug info
PrintGridVisualizerInfo

// Test hover
TestGridHover 10 10

// Test selection
TestGridSelection 5 7

// Test range display
TestMovementRange 10 10 5
```

---

## Performance Metrics

### Target Performance

**Grid Size: 20×20 (400 tiles)**
- Draw Calls: 1-2
- Frame Time: <0.5ms
- Memory: ~2MB (mesh + instances)

**Grid Size: 50×50 (2500 tiles)**
- Draw Calls: 1-2
- Frame Time: <1.0ms
- Memory: ~8MB

---

### Bottlenecks to Avoid

**❌ Don't:**
```cpp
// Update all tiles every frame
void Tick(float DeltaTime)
{
    RefreshGridVisuals(); // BAD! Too expensive
}

// Create new instances frequently
void UpdateGrid()
{
    TileMeshComponent->ClearInstances();
    CreateTileInstances(); // BAD! Reuses existing instances instead
}
```

**✅ Do:**
```cpp
// Update only changed tiles
void OnTileChanged(const FKalkiGridCoord& Coord)
{
    UpdateTileVisual(Coord); // GOOD! Only updates one tile
}

// Batch updates
void UpdateMultipleTiles(const TArray<FKalkiGridCoord>& Coords)
{
    // Update all, then mark dirty once
}
```

---

## Material Reference

### Minimal Working Material

**M_GridTile (simplified):**

**Nodes:**
1. `PerInstanceCustomData` node
2. Output RGBA to `Base Color` and `Opacity`

**Settings:**
- Blend Mode: `Translucent`
- Shading Model: `Unlit`

**Graph:**
```
[PerInstanceCustomData] (outputs: R, G, B, A)
    ↓ (RGB)
[Base Color]

    ↓ (A)
[Opacity]
```

---

### Advanced Material

**M_GridTile_Advanced:**

**Features:**
- Base color tint
- Grid lines overlay
- Emission glow
- Normal mapping

**Graph:**
```
[PerInstanceCustomData (RGB)] → [Multiply] ← [Base Texture]
                                    ↓
                           [Add] ← [Grid Lines Texture]
                                    ↓
                              [Base Color]

[PerInstanceCustomData (A)] → [Opacity]

[PerInstanceCustomData (RGB)] → [Multiply 0.5] → [Emissive Color]
```

---

## Code Complete Example

### Full Visualizer Implementation

**KalkiGridVisualizer.h:**
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridVisualizer.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/KalkiGridTypes.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "KalkiGridVisualizer.generated.h"

// Forward declarations
class UKalkiGridManager;

// Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileHovered, const FKalkiGridCoord&, Coord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileClicked, const FKalkiGridCoord&, Coord);

/**
 * Grid Visualizer
 * Renders the tactical grid using instanced meshes
 */
UCLASS()
class KALKI_API AKalkiGridVisualizer : public AActor
{
    GENERATED_BODY()

public:
    AKalkiGridVisualizer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ========================================
    // COMPONENTS
    // ========================================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid")
    TObjectPtr<UInstancedStaticMeshComponent> TileMeshComponent;

    // ========================================
    // CONFIGURATION
    // ========================================

    // Tile mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
    TObjectPtr<UStaticMesh> TileMesh;

    // Tile material
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
    TObjectPtr<UMaterialInterface> TileMaterial;

    // Show/hide grid
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
    bool bShowGrid = true;

    // Show elevation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Visuals")
    bool bShowElevation = true;

    // ========================================
    // COLORS
    // ========================================

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
    FLinearColor DefaultTileColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
    FLinearColor UnwalkableTileColor = FLinearColor(0.5f, 0.1f, 0.1f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
    FLinearColor HoverTileColor = FLinearColor(0.3f, 0.6f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
    FLinearColor SelectedTileColor = FLinearColor(0.3f, 1.0f, 0.3f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
    FLinearColor RangeTileColor = FLinearColor(1.0f, 1.0f, 0.3f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid|Colors")
    float ElevationColorMultiplier = 1.2f;

    // ========================================
    // EVENTS
    // ========================================

    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
    FOnTileHovered OnTileHovered;

    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
    FOnTileClicked OnTileClicked;

    // ========================================
    // PUBLIC FUNCTIONS
    // ========================================

    // Create/refresh instances
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void CreateTileInstances();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void RefreshGridVisuals();

    // Update single tile
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void UpdateTileVisual(const FKalkiGridCoord& Coord);

    // Show/hide grid
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetShowGrid(bool bShow);

    // Mouse interaction
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    FKalkiGridCoord GetTileUnderCursor() const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord) const;

    // Visual feedback
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetHoveredTile(const FKalkiGridCoord& Coord);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ClearHoveredTile();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetSelectedTile(const FKalkiGridCoord& Coord);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ClearSelectedTile();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ShowMovementRange(const TArray<FKalkiGridCoord>& Tiles);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ClearMovementRange();

private:
    // ========================================
    // INTERNAL STATE
    // ========================================

    FKalkiGridCoord HoveredTileCoord;
    bool bHasHover = false;

    FKalkiGridCoord SelectedTileCoord;
    bool bHasSelection = false;

    TSet<FKalkiGridCoord> RangeTiles;

    // ========================================
    // INTERNAL FUNCTIONS
    // ========================================

    // Event handlers
    UFUNCTION()
    void OnGridCreated();

    UFUNCTION()
    void OnTileChanged(const FKalkiGridCoord& Coord);

    // Helpers
    FLinearColor GetTileColor(const FKalkiGridCoord& Coord) const;
    void SetInstanceColor(int32 InstanceIndex, const FLinearColor& Color);
    int32 CoordToInstanceIndex(const FKalkiGridCoord& Coord) const;
};
```

---

## Summary

### Key Takeaways

1. **Instanced Rendering** - 400+ tiles in 1-2 draw calls
2. **Per-Instance Colors** - Custom data for dynamic coloring
3. **Real-time Feedback** - Hover, selection, range highlights
4. **Event-Driven** - Updates only when needed
5. **Designer-Friendly** - Blueprint configuration

### Integration Points

**Used By:**
- Character movement (range preview)
- Ability targeting (valid targets)
- Pathfinding (path visualization)
- UI (tile info tooltips)

**Depends On:**
- GridManager (tile data)
- InstancedStaticMeshComponent (rendering)
- Materials (per-instance coloring)

---

## Document Info
**File Path:** `Kalki_GDD/03_Systems/Grid_Visualization.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate