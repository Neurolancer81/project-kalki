---
tags:
  - kalki/architecture
  - kalki/structure
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
---

# Project Structure

## Overview
This document defines the complete folder structure and file organization for the Kalki project. Understanding this structure is essential for navigating the codebase and knowing where to place new files.

---

## Root Directory Structure
```
Kalki/
├── Source/                 # C++ source code
├── Content/                # Unreal assets (Blueprints, materials, etc.)
├── Config/                 # Configuration files
├── Plugins/                # Third-party and custom plugins
├── Documentation/          # Design documents, GDD
├── Build/                  # Build artifacts (gitignored)
├── Binaries/               # Compiled binaries (gitignored)
├── Intermediate/           # Intermediate build files (gitignored)
├── Saved/                  # Saved data, logs (gitignored)
├── .git/                   # Git repository
├── .gitignore              # Git ignore rules
├── .gitattributes          # Git LFS configuration
├── Kalki.uproject          # Unreal project file
└── README.md               # Project readme
```

---

## Source/ Directory

### Module Structure
```
Source/
├── Kalki/                  # Primary game module
│   ├── Kalki.Build.cs      # Build configuration
│   ├── Public/             # Public headers (.h)
│   └── Private/            # Implementation files (.cpp)
│
├── KalkiEditor/            # Editor-only module (future)
│   ├── KalkiEditor.Build.cs
│   ├── Public/
│   └── Private/
│
└── ThirdParty/             # Third-party libraries (if any)
```

---

### Kalki/Public/ Structure

**Public headers - exposed to other modules**
```
Public/
├── Grid/
│   ├── KalkiGridManager.h          # Grid subsystem
│   ├── KalkiGridTypes.h            # Grid structs (Coord, Tile)
│   └── KalkiGridVisualizer.h       # Grid rendering
│
├── Level/
│   └── KalkiLevelManager.h         # Per-level configuration
│
├── Logging/
│   └── KalkiLog.h                  # Static logging utility
│
├── UI/
│   ├── Common/                     # Shared UI classes
│   │   ├── KalkiViewModelBase.h    # Base ViewModel
│   │   ├── KalkiViewModelWidget.h  # Base widget with ViewModel
│   │   └── KalkiSimpleWidget.h     # Simple widget (no ViewModel)
│   │
│   ├── Combat/                     # Combat UI
│   │   ├── KalkiCombatLogViewModel.h
│   │   └── KalkiCombatLogWidget.h
│   │
│   └── Grid/                       # Grid UI
│       ├── KalkiGridInfoViewModel.h
│       └── KalkiGridInfoWidget.h
│
├── Character/                      # Character classes (future)
│   ├── KalkiCharacter.h
│   ├── KalkiPlayerController.h
│   └── Components/
│       ├── KalkiStatsComponent.h
│       └── KalkiAbilityComponent.h
│
├── Combat/                         # Combat system (future)
│   ├── KalkiCombatManager.h
│   ├── KalkiAbility.h
│   └── KalkiDamageTypes.h
│
├── AI/                             # AI system (future)
│   ├── KalkiAIController.h
│   └── KalkiBehaviorTree.h
│
├── Game/
│   ├── KalkiGameMode.h
│   ├── KalkiGameState.h
│   └── KalkiPlayerState.h
│
└── Utilities/                      # Helper classes
    ├── KalkiMathLibrary.h
    └── KalkiCheatManager.h
```

---

### Kalki/Private/ Structure

**Implementation files - internal to module**
```
Private/
├── Grid/
│   ├── KalkiGridManager.cpp
│   └── KalkiGridVisualizer.cpp
│
├── Level/
│   └── KalkiLevelManager.cpp
│
├── Logging/
│   └── KalkiLog.cpp
│
├── UI/
│   ├── Common/
│   │   ├── KalkiViewModelBase.cpp
│   │   ├── KalkiViewModelWidget.cpp
│   │   └── KalkiSimpleWidget.cpp
│   │
│   ├── Combat/
│   │   ├── KalkiCombatLogViewModel.cpp
│   │   └── KalkiCombatLogWidget.cpp
│   │
│   └── Grid/
│       ├── KalkiGridInfoViewModel.cpp
│       └── KalkiGridInfoWidget.cpp
│
├── Character/
│   ├── KalkiCharacter.cpp
│   ├── KalkiPlayerController.cpp
│   └── Components/
│       ├── KalkiStatsComponent.cpp
│       └── KalkiAbilityComponent.cpp
│
├── Combat/
│   ├── KalkiCombatManager.cpp
│   └── KalkiAbility.cpp
│
├── AI/
│   └── KalkiAIController.cpp
│
├── Game/
│   ├── KalkiGameMode.cpp
│   ├── KalkiGameState.cpp
│   └── KalkiPlayerState.cpp
│
└── Utilities/
    ├── KalkiMathLibrary.cpp
    └── KalkiCheatManager.cpp
```

---

## Content/ Directory

### Asset Organization
```
Content/
├── Blueprints/
│   ├── Characters/
│   │   ├── BP_KalkiCharacter          # Base character
│   │   ├── BP_Fighter                 # Fighter class
│   │   ├── BP_Wizard                  # Wizard class
│   │   └── BP_Rogue                   # Rogue class
│   │
│   ├── Grid/
│   │   ├── BP_GridVisualizer          # Grid rendering config
│   │   └── BP_LevelManager            # Level configuration
│   │
│   ├── AI/
│   │   └── BP_EnemyController         # Enemy AI
│   │
│   └── Game/
│       ├── BP_KalkiGameMode
│       └── BP_KalkiPlayerController
│
├── UI/
│   ├── Widgets/
│   │   ├── Combat/
│   │   │   └── WBP_CombatLog          # Combat log widget
│   │   │
│   │   ├── Grid/
│   │   │   └── WBP_GridInfo           # Grid info tooltip
│   │   │
│   │   └── HUD/
│   │       ├── WBP_MainHUD            # Main HUD
│   │       ├── WBP_CombatHUD          # Combat mode HUD
│   │       └── WBP_StrategyHUD        # Strategy mode HUD
│   │
│   ├── Fonts/
│   │   └── F_MainFont.uasset
│   │
│   ├── Icons/
│   │   └── T_Icon_*.uasset
│   │
│   └── Styles/
│       └── WS_MainStyle               # Widget style
│
├── Materials/
│   ├── Grid/
│   │   ├── M_GridTile_Base            # Base tile material
│   │   ├── MI_GridTile_Walkable       # Walkable variant
│   │   ├── MI_GridTile_Unwalkable     # Unwalkable variant
│   │   └── MI_GridTile_Hover          # Hover highlight
│   │
│   ├── Characters/
│   │   └── M_Character_Base           # Character material
│   │
│   └── FX/
│       └── M_VFX_*                    # Visual effects
│
├── Meshes/
│   ├── Grid/
│   │   ├── SM_GridTile_1x1            # Single tile mesh
│   │   ├── SM_GridTile_2x2            # 2x2 platform
│   │   └── SM_GridTile_5x5            # 5x5 platform
│   │
│   ├── Characters/
│   │   └── SK_Character_*             # Character meshes
│   │
│   └── Props/
│       └── SM_Prop_*                  # Environment props
│
├── Textures/
│   ├── Grid/
│   │   ├── T_GridTile_Albedo
│   │   ├── T_GridTile_Normal
│   │   ├── T_GridTile_Roughness
│   │   └── T_GridTile_AO
│   │
│   ├── Characters/
│   │   └── T_Character_*
│   │
│   └── UI/
│       └── T_UI_*
│
├── Audio/
│   ├── SFX/
│   │   ├── Combat/
│   │   │   └── A_SFX_*                # Combat sounds
│   │   │
│   │   ├── UI/
│   │   │   └── A_UI_*                 # UI sounds
│   │   │
│   │   └── Ambient/
│   │       └── A_Ambient_*            # Ambient sounds
│   │
│   └── Music/
│       ├── A_Music_Combat             # Combat music
│       └── A_Music_Strategy           # Strategy music
│
├── Animations/
│   ├── Characters/
│   │   ├── ABP_Character              # Animation Blueprint
│   │   └── AS_Character_*             # Animation sequences
│   │
│   └── UI/
│       └── UMG animations
│
├── FX/
│   ├── Niagara/                       # Particle systems
│   │   └── NS_*
│   │
│   └── Materials/
│       └── M_VFX_*
│
├── Levels/
│   ├── L_MainMenu                     # Main menu
│   ├── L_LoadLevel                    # Loading screen
│   ├── L_TestMap                      # Test/development map
│   │
│   └── Campaign/
│       ├── L_Chapter1_01              # Campaign levels
│       ├── L_Chapter1_02
│       └── ...
│
└── Data/
    ├── DataTables/
    │   ├── DT_Characters              # Character data
    │   ├── DT_Abilities               # Ability data
    │   └── DT_Items                   # Item data
    │
    └── DataAssets/
        ├── DA_Classes                 # Class definitions
        └── DA_Enemies                 # Enemy definitions
```

---

## Config/ Directory

### Configuration Files
```
Config/
├── DefaultEngine.ini         # Engine configuration
├── DefaultGame.ini           # Game configuration
├── DefaultInput.ini          # Input bindings
├── DefaultEditor.ini         # Editor settings
├── DefaultScalability.ini    # Graphics scalability
└── DefaultRHI.ini            # Rendering settings (RHI)
```

**Key Settings:**

**DefaultEngine.ini:**
```ini
[/Script/Engine.Engine]
+ActiveGameNameRedirects=(OldGameName="Kalki",NewGameName="/Script/Kalki")

[/Script/EngineSettings.GameMapsSettings]
GameDefaultMap=/Game/Levels/L_MainMenu
EditorStartupMap=/Game/Levels/L_TestMap
```

**DefaultGame.ini:**
```ini
[/Script/EngineSettings.GeneralProjectSettings]
ProjectID=...
ProjectName=Kalki
CompanyName=V.S. Puranam
CopyrightNotice=Copyright of V.S. Puranam and no one else
```

---

## Plugins/ Directory

### Plugin Structure
```
Plugins/
├── KalkiCore/                # Custom core plugin (future)
│   ├── KalkiCore.uplugin
│   ├── Source/
│   ├── Content/
│   └── Resources/
│
└── ThirdParty/               # Third-party plugins
    └── (Empty for now)
```

---

## Documentation/ Directory

### GDD Structure
```
Documentation/
└── Kalki_GDD/
    ├── 00_Overview/
    │   ├── Current_Status.md
    │   ├── Project_Vision.md
    │   └── Technical_Stack.md
    │
    ├── 01_Architecture/
    │   ├── Project_Structure.md         # This document
    │   ├── Module_Overview.md
    │   ├── Class_Hierarchy.md
    │   └── System_Dependencies.md
    │
    ├── 02_Conventions/
    │   ├── Naming_Conventions.md
    │   ├── Coding_Standards.md
    │   ├── File_Organization.md
    │   └── Blueprint_Guidelines.md
    │
    ├── 03_Systems/
    │   ├── Logging_System.md
    │   ├── Grid_System.md
    │   ├── Grid_Visualization.md
    │   ├── Combat_Log_System.md
    │   └── MVVM_Pattern.md
    │
    ├── 04_Implementation/
    │   ├── Class_Templates.md
    │   ├── Common_Patterns.md
    │   ├── Testing_Procedures.md
    │   ├── Multiplayer_Guidelines.md
    │   ├── Error_Handling.md
    │   └── Performance_Guidelines.md
    │
    ├── 05_API_Reference/
    │   ├── KalkiLog_API.md
    │   ├── KalkiGridManager_API.md
    │   ├── KalkiGridVisualizer_API.md
    │   └── Widget_System_API.md
    │
    ├── 06_How_To_Work_With_Claude/
    │   ├── Context_Guidelines.md
    │   └── Code_Generation_Rules.md
    │
    └── 07_Roadmap/
        ├── Phase_3_Pathfinding.md
        ├── Phase_4_Character_Movement.md
        └── Future_Systems.md
```

---

## File Naming Conventions

### C++ Files

**Headers (.h):**
```
KalkiClassName.h
KalkiGridManager.h
KalkiCharacter.h
```

**Implementation (.cpp):**
```
KalkiClassName.cpp
KalkiGridManager.cpp
KalkiCharacter.cpp
```

**See [[02_Conventions/Naming_Conventions]] for complete rules.**

---

### Blueprint Files

**Actors:**
```
BP_ClassName
BP_KalkiCharacter
BP_GridVisualizer
```

**Widgets:**
```
WBP_WidgetName
WBP_CombatLog
WBP_GridInfo
```

**Materials:**
```
M_MaterialName        (Base material)
MI_MaterialName       (Material instance)
```

**Meshes:**
```
SM_MeshName          (Static mesh)
SK_MeshName          (Skeletal mesh)
```

**Textures:**
```
T_TextureName_Type
T_GridTile_Albedo
T_Character_Normal
```

---

## Module Dependencies

### Kalki Module Dependencies
```
Kalki.Build.cs:

PublicDependencyModuleNames:
- Core
- CoreUObject
- Engine
- InputCore
- UMG                 (UI widgets)
- Slate               (UI framework)
- SlateCore           (UI framework)

PrivateDependencyModuleNames:
- RenderCore          (Rendering)
- RHI                 (Render Hardware Interface)
- NavigationSystem    (Pathfinding - future)
```

---

### Module Relationships
```
┌──────────────────────────────────────┐
│           Kalki (Game Module)        │
│  - Grid                              │
│  - Combat                            │
│  - Character                         │
│  - UI                                │
└──────────────┬───────────────────────┘
               │ depends on
               ↓
┌──────────────────────────────────────┐
│       Engine Modules                 │
│  - Core, CoreUObject, Engine         │
│  - UMG, Slate, SlateCore             │
│  - RenderCore, RHI                   │
└──────────────────────────────────────┘
```

---

## Folder Organization Principles

### By Feature, Not Type

**✅ Good - Organized by feature:**
```
Grid/
├── KalkiGridManager.h
├── KalkiGridTypes.h
└── KalkiGridVisualizer.h
```

**❌ Bad - Organized by type:**
```
Managers/
├── KalkiGridManager.h
├── KalkiCombatManager.h
└── ...

Types/
├── KalkiGridTypes.h
├── KalkiCombatTypes.h
└── ...
```

**Why:**
- Easier to find related code
- Clear ownership
- Easier to refactor
- Better encapsulation

---

### Public vs Private

**Public/ (Headers only):**
- Interface exposed to other modules
- Blueprint-accessible classes
- Shared data structures

**Private/ (Implementation):**
- Implementation details
- Helper functions
- Internal state

---

### UI Organization

**By Function, Not Widget Type:**

**✅ Good:**
```
UI/
├── Combat/
│   ├── KalkiCombatLogWidget.h
│   └── KalkiAbilityBarWidget.h
│
└── Grid/
    └── KalkiGridInfoWidget.h
```

**❌ Bad:**
```
UI/
├── Widgets/
│   ├── KalkiCombatLogWidget.h
│   ├── KalkiGridInfoWidget.h
│   └── ...
│
└── ViewModels/
    ├── KalkiCombatLogViewModel.h
    └── ...
```

---

## Special Files

### Kalki.Build.cs
**Purpose:** Build configuration for Kalki module
```csharp
// Source/Kalki/Kalki.Build.cs

using UnrealBuildTool;

public class Kalki : ModuleRules
{
    public Kalki(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "RenderCore",
            "RHI"
        });
    }
}
```

---

### Kalki.uproject
**Purpose:** Unreal project definition
```json
{
    "FileVersion": 3,
    "EngineAssociation": "5.5",
    "Category": "",
    "Description": "D&D 5e Tactical RPG",
    "Modules": [
        {
            "Name": "Kalki",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "AdditionalDependencies": [
                "Engine",
                "UMG"
            ]
        }
    ],
    "Plugins": [],
    "TargetPlatforms": [
        "Windows",
        "Mac",
        "Linux"
    ]
}
```

---

## .gitignore Configuration

### What to Ignore
```gitignore
# Unreal Engine
Binaries/
Build/
Intermediate/
Saved/
DerivedDataCache/

# Visual Studio / Rider
.vs/
.idea/
*.sln
*.suo
*.user
*.sdf
*.opensdf
*.VC.db
*.VC.opendb

# OS Files
.DS_Store
Thumbs.db

# Temporary Files
*.tmp
*.log
*.swp
```

---

## Git LFS Configuration

### Large File Tracking
```gitattributes
# Unreal Engine Large Files
*.uasset filter=lfs diff=lfs merge=lfs -text
*.umap filter=lfs diff=lfs merge=lfs -text

# 3D Models
*.fbx filter=lfs diff=lfs merge=lfs -text
*.obj filter=lfs diff=lfs merge=lfs -text

# Textures
*.tga filter=lfs diff=lfs merge=lfs -text
*.png filter=lfs diff=lfs merge=lfs -text
*.psd filter=lfs diff=lfs merge=lfs -text

# Audio
*.wav filter=lfs diff=lfs merge=lfs -text
*.ogg filter=lfs diff=lfs merge=lfs -text
```

---

## Quick Reference

### Finding Files

**"Where do I put...?"**

**New C++ class:**
- Header: `Source/Kalki/Public/[Category]/`
- Implementation: `Source/Kalki/Private/[Category]/`

**New Blueprint:**
- `Content/Blueprints/[Category]/`

**New Widget:**
- Code: `Source/Kalki/Public/UI/[Category]/`
- Blueprint: `Content/UI/Widgets/[Category]/`

**New Material:**
- `Content/Materials/[Category]/`

**New Mesh:**
- `Content/Meshes/[Category]/`

**New Texture:**
- `Content/Textures/[Category]/`

**New Level:**
- `Content/Levels/` or `Content/Levels/Campaign/`

---

## Related Documents

### Foundation
- [[00_Overview/Technical_Stack]] - Technologies used
- [[02_Conventions/Naming_Conventions]] - File naming rules
- [[02_Conventions/File_Organization]] - Organization principles

### Architecture
- [[01_Architecture/Module_Overview]] - Module details
- [[01_Architecture/Class_Hierarchy]] - Class relationships
- [[01_Architecture/System_Dependencies]] - System connections

### Implementation
- [[04_Implementation/Class_Templates]] - Where to put new classes
- [[04_Implementation/Common_Patterns]] - Common file structures

### For Claude
- [[06_How_To_Work_With_Claude/Context_Guidelines]] - Navigating the project

---

## Document Info
**File Path:** `Kalki_GDD/01_Architecture/Project_Structure.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate