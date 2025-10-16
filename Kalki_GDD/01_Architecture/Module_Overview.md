---
tags:
  - kalki/architecture
  - kalki/modules
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
---

# Module Overview

## Overview
This document provides a detailed overview of all modules in the Kalki project, their responsibilities, dependencies, and key classes. Understanding modules is essential for maintaining clean architecture and avoiding circular dependencies.

---

## What is a Module?

**Unreal Engine Modules:**
- Self-contained units of code
- Defined by `.Build.cs` files
- Can depend on other modules
- Compiled into separate libraries

**Benefits:**
- Clear boundaries between systems
- Reusable across projects
- Faster incremental compilation
- Better encapsulation

---

## Module Structure

### Primary Module: Kalki

**Type:** Runtime Game Module  
**Path:** `Source/Kalki/`  
**Purpose:** Core gameplay functionality

**Build File:** `Kalki.Build.cs`
```csharp
public class Kalki : ModuleRules
{
    public Kalki(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core",           // Core Unreal types
            "CoreUObject",    // UObject system
            "Engine",         // Engine classes
            "InputCore",      // Input handling
            "UMG",            // UI widgets
            "Slate",          // UI framework
            "SlateCore"       // UI framework core
        });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "RenderCore",     // Rendering utilities
            "RHI"             // Render Hardware Interface
        });
    }
}
```

---

## Kalki Module Subsystems

The Kalki module is organized into logical subsystems (folders), each with specific responsibilities.

---

### 1. Grid Subsystem

**Path:** `Source/Kalki/[Public|Private]/Grid/`  
**Status:** ✅ Complete (Phase 2)

**Purpose:**
- Tactical grid management
- Tile-based spatial queries
- Elevation handling
- Coordinate conversion

**Key Classes:**

**UKalkiGridManager** (WorldSubsystem)
```cpp
// Responsibilities:
- Create and manage grid
- Store tile data (TMap)
- Validate coordinates
- Spatial queries (neighbors, range)
- Server authority for modifications
- Event broadcasting (OnGridCreated, OnTileChanged)
```

**AKalkiLevelManager** (Actor)
```cpp
// Responsibilities:
- Per-level grid configuration
- Replicate grid config to clients
- Spawn GridVisualizer
- Designer-friendly settings
```

**AKalkiGridVisualizer** (Actor)
```cpp
// Responsibilities:
- Render grid using instanced meshes
- Visual feedback (hover, selection, range)
- Update on tile changes
- Mouse interaction
```

**FKalkiGridCoord** (Struct)
```cpp
// Purpose: Grid coordinate (X, Y)
// Features: Hashable, comparable
```

**FKalkiGridTile** (Struct)
```cpp
// Purpose: Tile properties
// Data: Coordinate, WorldPosition, Elevation, Walkability, Occupant
```

**Dependencies:**
- Logging (KalkiLog)
- Engine (WorldSubsystem, Actor)
- Rendering (InstancedStaticMeshComponent)

**API Reference:** [[05_API_Reference/KalkiGridManager_API]]

---

### 2. UI Subsystem

**Path:** `Source/Kalki/[Public|Private]/UI/`  
**Status:** ✅ Partially Complete (Phase 2)

**Purpose:**
- User interface widgets
- MVVM architecture
- HUD management
- Input handling

**Organization:**
```
UI/
├── Common/           # Shared UI classes
├── Combat/           # Combat-specific UI
├── Grid/             # Grid-specific UI
└── HUD/              # Main HUD (future)
```

---

#### UI/Common - Base Classes

**UKalkiViewModelBase** (UObject)
```cpp
// Purpose: Base class for all ViewModels
// Pattern: MVVM data layer
// Usage: Inherit for complex UI with state
```

**UKalkiViewModelWidget** (UUserWidget)
```cpp
// Purpose: Base widget with ViewModel support
// Features: Automatic binding, data updates
// Usage: Inherit for MVVM widgets
```

**UKalkiSimpleWidget** (UUserWidget)
```cpp
// Purpose: Simple widget without ViewModel
// Usage: Tooltips, health bars, simple displays
```

**Dependencies:**
- UMG (Widget system)
- Slate (UI framework)

**See:** [[03_Systems/MVVM_Pattern]]

---

#### UI/Combat - Combat UI

**UKalkiCombatLogViewModel** (ViewModel)
```cpp
// Purpose: Combat log data management
// Data: Messages, formatting, filtering
// Features: Session-based file logging
```

**UKalkiCombatLogWidget** (Widget)
```cpp
// Purpose: Display combat log messages
// Features: Scroll view, auto-scroll, styling
```

**Status:** ✅ Complete

**See:** [[03_Systems/Combat_Log_System]]

---

#### UI/Grid - Grid UI

**UKalkiGridInfoViewModel** (ViewModel)
```cpp
// Purpose: Grid tile information
// Data: Coordinate, elevation, walkability, movement cost
// Features: Extensible for combat info
```

**UKalkiGridInfoWidget** (Widget)
```cpp
// Purpose: Display tile info tooltip
// Features: Mouse-following, dynamic updates
```

**Status:** ✅ Complete

---

### 3. Logging Subsystem

**Path:** `Source/Kalki/[Public|Private]/Logging/`  
**Status:** ✅ Complete (Phase 1)

**Purpose:**
- Centralized logging
- Multi-channel support
- Severity levels
- Context tracking

**Key Classes:**

**KalkiLog** (Static Utility)
```cpp
// Purpose: Static logging utility
// Usage: KalkiLog::Grid(TEXT("Message"));

// Channels:
- General
- Combat
- Grid
- System
- UI
- Ability
- Character
- AI
- Network

// Severity:
- Log (normal)
- Warning (caution)
- Error (critical)
```

**Dependencies:**
- Engine (UE_LOG)
- CoreMinimal

**API Reference:** [[05_API_Reference/KalkiLog_API]]

---

### 4. Level Subsystem

**Path:** `Source/Kalki/[Public|Private]/Level/`  
**Status:** ✅ Complete (Phase 2)

**Purpose:**
- Per-level configuration
- Grid initialization
- Level-specific settings

**Key Classes:**

**AKalkiLevelManager** (Actor)
```cpp
// Purpose: Level-specific configuration
// Responsibilities:
- Configure grid (size, tile size)
- Spawn visualizers
- Replicate to clients
- Designer-facing properties
```

**Dependencies:**
- Grid (GridManager)
- Logging (KalkiLog)
- Replication

---

### 5. Character Subsystem (Future)

**Path:** `Source/Kalki/[Public|Private]/Character/`  
**Status:** ⏳ Planned (Phase 4)

**Purpose:**
- Character classes
- Stats and attributes
- Character progression
- D&D 5e character system

**Planned Classes:**

**AKalkiCharacter** (ACharacter)
```cpp
// Purpose: Base character class
// Features:
- D&D 5e stats (STR, DEX, CON, INT, WIS, CHA)
- HP, AC, Speed
- Class and level
- Ability slots
```

**AKalkiPlayerController** (APlayerController)
```cpp
// Purpose: Player input and control
// Features:
- Grid cursor
- Ability targeting
- Character selection
```

**Components:**

**UKalkiStatsComponent** (ActorComponent)
```cpp
// Purpose: Character statistics
// Data: HP, AC, Saves, Skills
```

**UKalkiAbilityComponent** (ActorComponent)
```cpp
// Purpose: Ability management
// Data: Known abilities, slots, cooldowns
```

**UKalkiGridMovementComponent** (ActorComponent)
```cpp
// Purpose: Grid-based movement
// Features: Pathfinding integration, smooth movement
```

**Dependencies:**
- Grid (GridManager, pathfinding)
- Combat (Abilities)
- UI (Character sheets)

---

### 6. Combat Subsystem (Future)

**Path:** `Source/Kalki/[Public|Private]/Combat/`  
**Status:** ⏳ Planned (Phase 5)

**Purpose:**
- Combat mechanics
- Ability system
- Damage calculations
- Turn management

**Planned Classes:**

**UKalkiCombatManager** (WorldSubsystem)
```cpp
// Purpose: Combat flow and rules
// Responsibilities:
- Turn order
- Action resolution
- Combat state
```

**UKalkiAbility** (UObject)
```cpp
// Purpose: Base ability class
// Features:
- Targeting
- Casting
- Effects
- D&D 5e rules
```

**Structs:**

**FKalkiDamageInfo**
```cpp
// Purpose: Damage data
// Data: Amount, type, source, target
```

**FKalkiAbilityData**
```cpp
// Purpose: Ability definition
// Data: Name, description, cost, range, effects
```

**Dependencies:**
- Grid (Targeting, range)
- Character (Stats, HP)
- UI (Combat log, feedback)

---

### 7. AI Subsystem (Future)

**Path:** `Source/Kalki/[Public|Private]/AI/`  
**Status:** ⏳ Planned (Phase 6)

**Purpose:**
- Enemy AI behavior
- Tactical decision-making
- Behavior trees

**Planned Classes:**

**AKalkiAIController** (AAIController)
```cpp
// Purpose: AI control for enemies
// Features:
- Tactical positioning
- Ability selection
- Target selection
```

**UKalkiBehaviorTree** (UBehaviorTree)
```cpp
// Purpose: AI behavior logic
// Features:
- Movement decisions
- Combat tactics
- Retreat logic
```

**Dependencies:**
- Grid (Pathfinding, positioning)
- Combat (Abilities, targeting)
- Character (Enemy stats)

---

### 8. Game Subsystem (Future)

**Path:** `Source/Kalki/[Public|Private]/Game/`  
**Status:** ⏳ Planned (Phase 5)

**Purpose:**
- Game rules and flow
- Game state management
- Player state tracking

**Planned Classes:**

**AKalkiGameMode** (AGameModeBase)
```cpp
// Purpose: Game rules and flow
// Features:
- Combat initialization
- Victory/defeat conditions
- Turn management
```

**AKalkiGameState** (AGameStateBase)
```cpp
// Purpose: Replicated game state
// Data:
- Current turn
- Combat phase
- Team scores
```

**AKalkiPlayerState** (APlayerState)
```cpp
// Purpose: Per-player state
// Data:
- Character roster
- Resources
- Progression
```

**Dependencies:**
- All subsystems (orchestrates game flow)

---

### 9. Utilities Subsystem

**Path:** `Source/Kalki/[Public|Private]/Utilities/`  
**Status:** ✅ Partially Complete

**Purpose:**
- Helper functions
- Math utilities
- Cheat commands

**Key Classes:**

**UKalkiMathLibrary** (UBlueprintFunctionLibrary)
```cpp
// Purpose: Math helper functions
// Functions:
- Distance calculations
- Grid math
- Probability utilities
```

**UKalkiCheatManager** (UCheatManager)
```cpp
// Purpose: Console commands for testing
// Commands:
- CreateTestGrid
- SetTileElevation
- ShowGridVisualizer
- (Many more)
```

**Status:** ✅ CheatManager complete, MathLibrary future

---

## Module Dependencies Graph
```
┌─────────────────────────────────────────────────┐
│                  Kalki Module                   │
└──────┬────────┬──────────┬──────────┬───────────┘
       │        │          │          │
       ↓        ↓          ↓          ↓
   ┌───────┐ ┌─────┐  ┌────────┐ ┌──────────┐
   │ Grid  │ │ UI  │  │Logging │ │  Level   │
   └───┬───┘ └──┬──┘  └───┬────┘ └────┬─────┘
       │        │         │            │
       └────────┴─────────┴────────────┘
                     │
                     ↓
       ┌──────────────────────────────┐
       │     Engine Modules           │
       │  - Core, CoreUObject         │
       │  - Engine                    │
       │  - UMG, Slate, SlateCore     │
       │  - RenderCore, RHI           │
       └──────────────────────────────┘
```

---

## Subsystem Interaction Examples

### Example 1: Grid Visualization Update
```
User hovers over tile
        ↓
GridVisualizer (raycast)
        ↓
GridManager (query tile data)
        ↓
GridInfoViewModel (update data)
        ↓
GridInfoWidget (display update)
        ↓
User sees tile info
```

---

### Example 2: Character Movement (Future)
```
Player clicks tile
        ↓
PlayerController (input)
        ↓
GridManager (FindPath)
        ↓
GridMovementComponent (move along path)
        ↓
Character moves
        ↓
GridManager (update occupancy)
        ↓
GridVisualizer (visual update)
        ↓
CombatLog (log movement)
```

---

### Example 3: Ability Cast (Future)
```
Player selects ability
        ↓
AbilityComponent (check valid)
        ↓
GridManager (get tiles in range)
        ↓
GridVisualizer (show range)
        ↓
Player selects target
        ↓
CombatManager (resolve ability)
        ↓
DamageSystem (apply damage)
        ↓
StatsComponent (update HP)
        ↓
CombatLogWidget (show result)
```

---

## Compilation Order

**Module Compilation Order:**
1. Engine modules (Core, CoreUObject, Engine)
2. Kalki module
   - Logging (no dependencies)
   - Grid (depends on Logging)
   - UI (depends on Logging)
   - Level (depends on Grid, Logging)
   - Others (depend on previous)

**PCH (Precompiled Headers):**
- `Kalki.h` - Module header (includes common headers)
- Speeds up compilation
- Shared across all .cpp files in module

---

## Module Size and Complexity

### Current Size (Phase 2)
```
Kalki Module:
├── Grid:      ~1500 LOC (3 classes, 2 structs)
├── UI:        ~1200 LOC (6 classes)
├── Logging:   ~200 LOC (1 class)
├── Level:     ~400 LOC (1 class)
└── Utilities: ~300 LOC (1 class)

Total: ~3600 LOC
```

### Projected Size (Full Game)
```
Kalki Module:
├── Grid:      ~2000 LOC
├── UI:        ~3000 LOC
├── Character: ~2500 LOC
├── Combat:    ~3000 LOC
├── AI:        ~2000 LOC
├── Game:      ~1500 LOC
├── Logging:   ~200 LOC
├── Level:     ~500 LOC
└── Utilities: ~800 LOC

Total: ~15,500 LOC
```

---

## Module Extension Guidelines

### When to Add a New Subsystem

**Add a new subsystem (folder) when:**
- ✅ Logical grouping of related classes (5+ classes)
- ✅ Clear boundary and responsibility
- ✅ Potentially reusable across projects
- ✅ Minimal dependencies on other subsystems

**Don't add a new subsystem for:**
- ❌ Single class with no related code
- ❌ Tightly coupled to existing subsystem
- ❌ Organizational convenience only

---

### When to Create a New Module

**Create a separate module when:**
- ✅ Code is fully independent
- ✅ Intended for reuse across projects
- ✅ Large enough to justify overhead (1000+ LOC)
- ✅ Has specific plugin requirements

**Example Future Modules:**
- `KalkiCore` - Reusable utilities
- `KalkiEditor` - Editor-only tools
- `KalkiAbilities` - Ability system (if very large)

---

## Testing Strategy per Subsystem

### Grid Subsystem
**Test via:**
- Console commands (CreateTestGrid, etc.)
- PIE multiplayer testing
- Unit tests for coordinate math (future)

### UI Subsystem
**Test via:**
- Visual inspection in PIE
- ViewModel unit tests (future)
- Widget functional tests (future)

### Logging Subsystem
**Test via:**
- Manual log inspection
- Automated log parsing (future)

---

## Performance Considerations

### Subsystem Performance Targets

**Grid:**
- Tile queries: O(1) - TMap lookup
- Range queries: O(R²) - acceptable for R<10
- Path finding: O(N log N) - A* (future)

**UI:**
- Widget updates: <1ms per frame
- ViewModel updates: Immediate (event-driven)
- MVVM overhead: Negligible

**Logging:**
- Log call overhead: <0.1ms
- File I/O: Async (doesn't block)

---

## Memory Budget per Subsystem

**Approximate memory usage:**
```
Grid (20×20):     ~14 KB
UI (all widgets): ~2 MB
Logging (buffer): ~100 KB
Level:            ~1 KB
Character (each): ~50 KB
Combat:           ~500 KB
AI (per enemy):   ~20 KB
```

**Total (estimated): ~5-10 MB for systems**

**Note:** Majority of memory is assets (meshes, textures, audio)

---

## Related Documents

### Foundation
- [[01_Architecture/Project_Structure]] - Where modules live
- [[01_Architecture/Class_Hierarchy]] - Class relationships
- [[01_Architecture/System_Dependencies]] - Dependency graph

### Systems
- [[03_Systems/Grid_System]] - Grid subsystem details
- [[03_Systems/MVVM_Pattern]] - UI subsystem architecture
- [[03_Systems/Logging_System]] - Logging subsystem details

### Implementation
- [[04_Implementation/Common_Patterns]] - Subsystem patterns
- [[04_Implementation/Multiplayer_Guidelines]] - Networking per subsystem

### Roadmap
- [[07_Roadmap/Phase_3_Pathfinding]] - Grid subsystem extension
- [[07_Roadmap/Phase_4_Character_Movement]] - Character subsystem
- [[07_Roadmap/Future_Systems]] - New subsystems

---

## Quick Reference

### Finding Subsystem Code

**"Which subsystem handles X?"**

- Grid operations → Grid subsystem
- UI display → UI subsystem
- Logging → Logging subsystem
- Level config → Level subsystem
- Character stats → Character subsystem (future)
- Combat rules → Combat subsystem (future)
- Enemy AI → AI subsystem (future)
- Game flow → Game subsystem (future)

---

## Document Info
**File Path:** `Kalki_GDD/01_Architecture/Module_Overview.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate