---
tags: [technical, subsystems, ue5.6]
---

# Subsystem Architecture

## Overview
Subsystems form the backbone of *The Fourth Age*.  
They divide responsibilities cleanly across **GameInstance**, **World**, and **Player** scopes.

---

## Hierarchy
```
UTFA_GameInstanceSubsystem_Journal  
├─ UTFA_GameLogRouter  
├─ UTFA_EventStore  
├─ UTFA_SnapshotManager  
└─ UTFA_JournalView

UTFA_WorldSubsystem_Combat  
├─ UTFA_TurnManager  
├─ UTFA_ActionQueue  
├─ UTFA_CombatStateMachine

UTFA_WorldSubsystem_Exploration  
├─ UTFA_InteractableRegistry  
└─ UTFA_PerceptionGrid

UTFA_WorldSubsystem_Overworld  
├─ UTFA_NodeGraph  
└─ UTFA_EncounterManager

UTFA_PlayerSubsystem_UI  
├─ UTFA_UIManager  
├─ UTFA_TooltipRegistry  
└─ UTFA_NotificationManager

```


---

## Subsystem Lifecycle

| Stage | Subsystem | Key Methods |
|--------|------------|-------------|
| Game start | `Journal` | `Initialize()`, load latest snapshot |
| Map load | `Combat`, `Exploration`, `Overworld` | `OnWorldBeginPlay()` |
| Player join | `UI` | `SetupHUD()`, subscribe to Journal delegates |
| Map unload | all | `Deinitialize()`, flush events |

---

## Communication Flow

[Gameplay System] → [Router] → [EventStore]  
↓  
[JournalView]  
↓  
[UI Widgets]


Events always flow *downstream*; gameplay never queries UI directly.

---

## Benefits
- Deterministic, reproducible systems.
- Clear separation of persistent (GameInstance) vs transient (World) data.
- Supports both single- and multiplayer with minimal refactor.

See also → [[Class Architecture]], [[Journal System (Technical)]].
