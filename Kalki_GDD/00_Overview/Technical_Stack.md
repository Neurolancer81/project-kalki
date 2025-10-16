---
tags:
  - kalki/overview
  - kalki/technical
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
---

# Technical Stack

## Overview
This document defines all technologies, tools, libraries, and frameworks used in the Kalki project. It serves as a reference for technical decisions and helps onboard new developers (or Claude instances).

---

## Core Engine

### Unreal Engine 5.3+
**Version:** 5.3 or higher  
**License:** Unreal Engine EULA

**Why Unreal:**
- ✅ Industry-standard for tactical games (XCOM 2, Gears Tactics)
- ✅ Excellent Blueprint + C++ workflow
- ✅ Strong multiplayer networking
- ✅ Great tooling and debugging
- ✅ Active community and marketplace
- ✅ Visual fidelity (Nanite, Lumen)
- ✅ Cross-platform support

**Key Engine Features Used:**
- Blueprint visual scripting (UI, rapid prototyping)
- C++ gameplay code (performance-critical systems)
- UMG (Unreal Motion Graphics) for UI
- Replication system for multiplayer
- WorldSubsystems for manager classes
- Instanced Static Meshes for grid rendering

---

## Programming Languages

### C++ (Primary)
**Standard:** C++17 (Unreal 5.3+ default)  
**Compiler:** MSVC 2022 (Windows), Clang (Mac/Linux)

**Usage:**
- Core gameplay systems (Grid, Pathfinding, Combat)
- Performance-critical code
- Multiplayer authority logic
- Data structures and algorithms
- Manager classes (Subsystems)

**Unreal-specific Extensions:**
- `UCLASS`, `USTRUCT`, `UENUM` macros
- `UPROPERTY`, `UFUNCTION` reflection
- TArray, TMap, TSet (Unreal containers)
- FString, FName, FText (Unreal strings)
- TObjectPtr (UE 5.1+ smart pointer)

---

### Blueprint (Secondary)
**Usage:**
- UI widgets and layout
- Rapid prototyping
- Designer-friendly logic
- Visual effects and animations
- Data-only classes (DataAssets)

**Blueprint Classes:**
- `BP_KalkiCharacter` - Character visuals and animations
- `BP_GridVisualizer` - Grid rendering configuration
- `BP_LevelManager` - Per-level settings
- `WBP_*` - All UI widgets

---

## Development Tools

### IDE
**Primary:** Rider for Unreal Engine  
**Alternative:** Visual Studio 2022

**Why Rider:**
- ✅ Better Unreal Engine integration
- ✅ Faster code navigation
- ✅ Superior refactoring tools
- ✅ Built-in Git support
- ✅ Blueprint debugging support
- ✅ Cross-platform (Windows, Mac, Linux)

**Plugins:**
- RiderLink (Unreal Engine integration)
- Git integration (built-in)
- Markdown support (for documentation)

---

### Version Control
**System:** Git  
**Hosting:** GitHub / GitLab (private repository)

**Structure:**
```
main (stable releases)
├─ develop (active development)
├─ feature/* (new features)
├─ bugfix/* (bug fixes)
└─ hotfix/* (critical fixes)
```

**What's Tracked:**
- ✅ Source code (C++ and headers)
- ✅ Config files (.ini)
- ✅ Asset references (not binaries)
- ✅ Documentation (GDD, markdown)

**What's Ignored (.gitignore):**
- ❌ Binaries (Binaries/, Intermediate/)
- ❌ Saved data (Saved/)
- ❌ Large assets (use Git LFS or separate)
- ❌ IDE files (.vs/, *.sln.DotSettings)

---

### Asset Management
**For large assets (meshes, textures, audio):**

**Option 1:** Unreal Engine Marketplace
- Pre-made assets
- Regular updates
- Community support

**Option 2:** Git LFS (Large File Storage)
- Track large files separately
- Keeps repo size manageable

**Option 3:** Cloud Storage + Local
- Store assets in cloud (Google Drive, Dropbox)
- Local working copy
- Not in Git

---

### Documentation
**Tool:** Obsidian  
**Format:** Markdown

**Why Obsidian:**
- ✅ Plain text (git-friendly)
- ✅ Graph view (visualize connections)
- ✅ Tags and links
- ✅ No vendor lock-in
- ✅ Fast and lightweight

**Vault Structure:**
- See [[01_Architecture/Project_Structure]] for GDD organization

---

## Project Structure

### Module Organization
```
Kalki/
├─ Source/
│  ├─ Kalki/             (Primary game module)
│  │  ├─ Public/         (Headers)
│  │  │  ├─ Grid/
│  │  │  ├─ Combat/
│  │  │  ├─ Character/
│  │  │  ├─ UI/
│  │  │  │  ├─ Common/
│  │  │  │  ├─ Combat/
│  │  │  │  └─ Grid/
│  │  │  ├─ Logging/
│  │  │  └─ Level/
│  │  └─ Private/        (Implementation)
│  │     ├─ Grid/
│  │     ├─ Combat/
│  │     ├─ Character/
│  │     ├─ UI/
│  │     ├─ Logging/
│  │     └─ Level/
│  └─ KalkiEditor/       (Editor-only tools)
│
├─ Content/
│  ├─ Blueprints/
│  │  ├─ Characters/
│  │  ├─ Grid/
│  │  └─ UI/
│  ├─ UI/
│  │  ├─ Widgets/
│  │  ├─ Fonts/
│  │  └─ Icons/
│  ├─ Materials/
│  ├─ Meshes/
│  ├─ Textures/
│  ├─ Audio/
│  └─ Levels/
│
├─ Config/               (Configuration files)
├─ Plugins/              (Third-party plugins)
└─ Documentation/        (Design docs, GDD)
```

**See [[01_Architecture/Project_Structure]] for detailed breakdown.**

---

## Unreal Engine Systems

### Gameplay Framework

**Subsystems (Manager Classes):**
```cpp
UWorldSubsystem - Per-world singletons
- UKalkiGridManager
- UKalkiCombatManager (future)
- UKalkiAbilityManager (future)

UGameInstanceSubsystem - Per-game singletons
- UKalkiSaveGameManager (future)
- UKalkiSettingsManager (future)
```

**Why Subsystems:**
- Automatic lifetime management
- Clean singleton pattern
- Easy to access (GetSubsystem)
- Blueprint-friendly

---

**Actor Framework:**
```cpp
AGameModeBase - Game rules and flow
- AKalkiGameMode (combat rules, turn order)

APlayerController - Input and player state
- AKalkiPlayerController (grid cursor, ability targeting)

ACharacter - Playable characters
- AKalkiCharacter (player units, NPCs)

AActor - World objects
- AKalkiLevelManager (level configuration)
- AKalkiGridVisualizer (grid rendering)
```

---

**Components:**
```cpp
UActorComponent - Reusable functionality
- UKalkiGridMovementComponent (grid-based movement)
- UKalkiAbilityComponent (ability casting)
- UKalkiStatsComponent (HP, AP, stats)
```

---

### UI Framework

**UMG (Unreal Motion Graphics):**
- Widget blueprints for visual layout
- C++ for logic and data binding
- MVVM pattern for complex UI

**Widget Hierarchy:**
```cpp
UUserWidget - Base widget class
├─ UKalkiSimpleWidget - Simple UI (no ViewModel)
│  └─ Health bars, tooltips, etc.
└─ UKalkiViewModelWidget - Complex UI (with ViewModel)
   ├─ UKalkiCombatLogWidget
   ├─ UKalkiGridInfoWidget
   └─ Character sheets, inventory, etc.
```

**ViewModel Pattern:**
```cpp
UKalkiViewModelBase - Base ViewModel
├─ UKalkiCombatLogViewModel
├─ UKalkiGridInfoViewModel
└─ Future ViewModels
```

**See [[03_Systems/MVVM_Pattern]] for details.**

---

### Networking

**Replication Model:**
- Server-authoritative
- Client-predicted (where appropriate)
- Deterministic systems (grid, pathfinding)

**Key Concepts:**
```cpp
bReplicates = true;              // Actor replicates
UPROPERTY(Replicated)            // Property replicates
UPROPERTY(ReplicatedUsing=Func)  // Replicate with callback

UFUNCTION(Server, Reliable)      // RPC to server
UFUNCTION(Client, Reliable)      // RPC to client
UFUNCTION(NetMulticast)          // RPC to all clients

HasAuthority()                   // Is this the server?
GetLocalRole()                   // ROLE_Authority or ROLE_SimulatedProxy
```

**Network Modes:**
- `NM_Standalone` - Single player
- `NM_ListenServer` - Host + clients
- `NM_DedicatedServer` - Dedicated server
- `NM_Client` - Client only

**See [[04_Implementation/Multiplayer_Guidelines]] for patterns.**

---

### Rendering

**Instanced Static Meshes:**
```cpp
UInstancedStaticMeshComponent
- Used for grid tiles (400+ instances)
- 1-2 draw calls for entire grid
- GPU instancing for performance
```

**Custom Data:**
```cpp
NumCustomDataFloats = 4;  // Per-instance data (RGBA color)
SetCustomDataValue(Index, Slot, Value);
```

**Materials:**
```cpp
UMaterialInterface - Base material
UMaterialInstance - Material instance
UMaterialInstanceDynamic - Runtime modification
```

**See [[03_Systems/Grid_Visualization]] for implementation.**

---

## Third-Party Libraries

### Currently Used

**None yet** - Using only Unreal Engine built-ins

---

### Considered for Future

**AI/Pathfinding:**
- Unreal's built-in navigation (NavMesh)
- Custom A* implementation (current plan)

**Save System:**
- Unreal's SaveGame system
- JSON serialization

**Analytics:**
- Epic Online Services
- Google Analytics for Games

---

## Data Structures

### Unreal Containers

**TArray - Dynamic array:**
```cpp
TArray<int32> Numbers;
TArray<FKalkiGridCoord> Path;
```

**TMap - Hash map (dictionary):**
```cpp
TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;  // O(1) lookup
TMap<int32, AKalkiCharacter*> CharactersByID;
```

**TSet - Hash set (unique values):**
```cpp
TSet<FKalkiGridCoord> WalkableTiles;
TSet<AActor*> VisibleEnemies;
```

**TQueue - FIFO queue:**
```cpp
TQueue<FKalkiAbilityData> AbilityQueue;
```

---

### Smart Pointers

**TObjectPtr (UE 5.1+):**
```cpp
TObjectPtr<UKalkiGridManager> GridManager;  // For UObjects
```

**TSharedPtr/TUniquePtr (for non-UObject):**
```cpp
TSharedPtr<FMyData> SharedData;
TUniquePtr<FMyResource> UniqueResource;
```

**TWeakObjectPtr (weak reference):**
```cpp
TWeakObjectPtr<AActor> Occupant;  // Doesn't prevent GC
```

---

### Unreal String Types

**FString - Mutable string:**
```cpp
FString Message = TEXT("Hello");
Message += TEXT(" World");
```

**FName - Immutable identifier (fast comparison):**
```cpp
FName Tag = "Enemy";
if (Tag == "Enemy") { }  // Fast
```

**FText - Localized text (for UI):**
```cpp
FText UIText = LOCTEXT("HealthLabel", "Health");
FText DebugText = INVTEXT("Debug info");
```

---

## Build Configuration

### Build Targets

**Development:**
```
- Cheats enabled
- Logging enabled
- Editor tools available
- Not optimized
```

**Shipping:**
```
- Cheats disabled (#if !UE_BUILD_SHIPPING)
- Minimal logging
- Fully optimized
- Smallest binary
```

**Debug:**
```
- Full debug symbols
- No optimization
- Slowest but easiest to debug
```

**DebugGame:**
```
- Debug game code
- Optimized engine
- Good for gameplay debugging
```

---

### Platform-Specific

**Windows (Primary):**
- DirectX 12 / DirectX 11
- MSVC compiler
- Full editor support

**Mac:**
- Metal API
- Clang compiler
- Full editor support

**Linux:**
- Vulkan API
- Clang compiler
- Editor support (experimental)

---

## Performance Targets

### PC (Primary Platform)

**Minimum Spec:**
- CPU: Intel i5-8400 / AMD Ryzen 5 2600
- GPU: GTX 1060 6GB / RX 580 8GB
- RAM: 8GB
- Storage: SSD recommended
- Target: 30 FPS @ 1080p Low settings

**Recommended Spec:**
- CPU: Intel i5-10600K / AMD Ryzen 5 5600X
- GPU: RTX 3060 / RX 6700 XT
- RAM: 16GB
- Storage: NVMe SSD
- Target: 60 FPS @ 1080p High settings

**High-End Spec:**
- CPU: Intel i7-12700K / AMD Ryzen 7 5800X3D
- GPU: RTX 4070 / RX 7800 XT
- RAM: 32GB
- Storage: NVMe SSD
- Target: 60 FPS @ 1440p/4K Ultra settings

---

### Console (Future)

**PlayStation 5:**
- Target: 60 FPS @ 1080p (Performance)
- Target: 30 FPS @ 4K (Quality)

**Xbox Series X:**
- Target: 60 FPS @ 1080p (Performance)
- Target: 30 FPS @ 4K (Quality)

**Xbox Series S:**
- Target: 60 FPS @ 1080p (Performance)
- Target: 30 FPS @ 1440p (Quality)

---

## Testing Framework

### Unit Testing
**Currently:** Manual testing via console commands

**Future:**
- Unreal Automation Testing Framework
- C++ unit tests for algorithms
- Blueprint functional tests

---

### Performance Testing

**Tools:**
```
stat FPS          - Frame rate
stat RHI          - Draw calls, triangles
stat SceneRendering - Rendering stats
stat Memory       - Memory usage
stat Game         - Game thread time
```

**Profiling:**
- Unreal Insights (CPU profiling)
- RenderDoc (GPU profiling)
- Visual Studio Profiler

---

## Asset Pipeline

### 3D Models

**Format:** FBX (preferred)  
**Alternative:** OBJ, glTF

**Guidelines:**
- Quads preferred (converts to tris on import)
- Clean topology
- Proper scale (Unreal units: 1 unit = 1 cm)
- UV mapping for textures

---

### Textures

**Format:** 
- TGA (uncompressed, best quality)
- PNG (compressed, smaller)
- PSD (working files)

**Resolution:**
- Characters: 2048×2048
- Props: 1024×1024 or 512×512
- UI: Varies (icons 256×256)

**Maps:**
- Albedo (Base Color)
- Normal
- Roughness
- Metallic
- Ambient Occlusion

---

### Audio

**Format:**
- WAV (lossless, for import)
- OGG Vorbis (compressed, runtime)

**Categories:**
- SFX (sound effects)
- Music (background, combat)
- VO (voice over) - future
- UI (button clicks, notifications)

---

## Localization

**Currently:** English only

**Future:**
- EFIGS (English, French, Italian, German, Spanish)
- Text externalization (FText + String Tables)
- Font support for additional languages

---

## Analytics and Telemetry

**Currently:** None

**Future:**
- Playtime tracking
- Encounter win/loss rates
- Ability usage stats
- Player builds
- Crash reporting

---

## Deployment

### Distribution Platform

**Primary:** Steam
- Steamworks SDK
- Achievements
- Cloud saves
- Multiplayer (Steam networking)

**Future:**
- Epic Games Store
- GOG (DRM-free)
- Console stores (PlayStation, Xbox)

---

### Update Strategy

**Versioning:** Semantic Versioning (MAJOR.MINOR.PATCH)
- MAJOR: Breaking changes
- MINOR: New features
- PATCH: Bug fixes

**Example:**
- v0.1.0 - MVP
- v0.2.0 - Added pathfinding
- v0.2.1 - Fixed pathfinding bug
- v1.0.0 - Release

---

## Development Environment Setup

### Required Software

1. **Unreal Engine 5.5+** (Epic Games Launcher)
2. **Rider for Unreal Engine** (JetBrains)
   - RiderLink plugin
   - Unreal Engine support
3. **Visual Studio 2022** (for MSVC compiler on Windows)
   - Desktop Development with C++
   - Game Development with C++
   - .NET Desktop Development (for Unreal Build Tool)
4. **Git** (version control)
5. **Obsidian** (documentation)

**Note:** Visual Studio is still required on Windows as it provides the MSVC compiler that Unreal uses. Rider uses this compiler but provides the IDE interface.

---

### Optional Tools

- **Visual Studio 2022** (as primary IDE instead of Rider)
- **Perforce Helix** (Large teams, binary assets)
- **Substance Painter** (Texture authoring)
- **Blender** (3D modeling)
- **Audacity** (Audio editing)

---

## Related Documents

### Foundation
- [[00_Overview/Project_Vision]] - What we're building
- [[00_Overview/Current_Status]] - Current progress

### Architecture
- [[01_Architecture/Project_Structure]] - Folder organization
- [[01_Architecture/Module_Overview]] - Code modules
- [[01_Architecture/Class_Hierarchy]] - Class relationships

### Implementation
- [[04_Implementation/Multiplayer_Guidelines]] - Networking patterns
- [[04_Implementation/Performance_Guidelines]] - Optimization tips
- [[04_Implementation/Class_Templates]] - Code templates

### Systems
- [[03_Systems/Grid_System]] - Example system using this stack
- [[03_Systems/MVVM_Pattern]] - UI architecture

---

## Document Info
**File Path:** `Kalki_GDD/00_Overview/Technical_Stack.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate