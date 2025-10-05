---
tags: [technical, classes]
---

> [!info] Project Prefix Update
> All classes in this project use the **`Kalki`** prefix (e.g., `AKalkiGameMode`, `UKalkiTurnManager`), replacing any earlier `Kalki` naming.  
> Files and folders now follow the conventions in [[Technical/Unreal Coding Standards (Project)]].

# Class Architecture

## Framework Diagram

```
AKalkiGameMode
├─ AKalkiGameState
│ ├─ UKalkiWorldSubsystem_Combat
│ ├─ UKalkiWorldSubsystem_Exploration
│ ├─ UKalkiWorldSubsystem_Overworld
│ └─ UAttributeSet_Stats
├─ AKalkiPlayerController
│ └─ UKalkiPlayerSubsystem_UI
└─ AKalkiPlayerState
└─ AbilitySystemComponent
```

---
### Kalki Class Name Map (authoritative)
- `AKalkiGameMode`, `AKalkiGameState`, `AKalkiPlayerController`, `AKalkiPlayerState`
- `UKalkiJournalGameInstanceSubsystem`, `UKalkiCombatWorldSubsystem`, `UKalkiExplorationWorldSubsystem`, `UKalkiOverworldWorldSubsystem`, `UKalkiUIPlayerSubsystem`
- `UKalkiTurnManager`, `UKalkiActionQueue`, `UKalkiCombatStateMachine`, `UKalkiDeterministicRNG`, `UKalkiTargetingService`
- `UKalkiTacticalAIComponent`, `UKalkiTacticalPolicy` (DataAsset), `UKalkiActionEvaluator`
- `UKalkiRulesAdapter`, `UKalkiAttributeSet_Stats`
- `FKalkiLogEvent`, `UKalkiGameLogRouter`, `UKalkiEventStore`, `UKalkiSnapshotManager`, `UKalkiJournalView`
---

## Key Base Classes
| Class                    | Parent              | Purpose                               |
| ------------------------ | ------------------- | ------------------------------------- |
| `AKalkiameMode`          | `AGameModeBase`     | Governs global flow.                  |
| `AKalkiGameState`        | `AGameStateBase`    | Holds active subsystems and state.    |
| `AKalkiPlayerController` | `APlayerController` | Handles input, commands, camera.      |
| `AKalkiPlayerState`       | `APlayerState`      | Stores stats, level, inventory refs.  |
| `AKalkiCharacter`         | `ACharacter`        | Pawn base; owns ASC and AIComponent.  |
| `AKalkiCompanion`         | `AKalkiCharacter`    | AI or player-controlled party member. |
| `AKalkiEnemy`             | `AKalkiCharacter`    | NPC controlled by Unified AI.         |

---

## DataAsset Types
| DataAsset | Usage |
|------------|--------|
| `UKalkiTacticalPolicy` | AI decision weighting |
| `UKalkiRulePack` | Formula definitions |
| `UKalkiAbilityData` | Metadata for abilities |
| `UKalkiQuestData` | Quest nodes & objectives |
| `UKalkiNodeData` | Overworld node definition |

---

## UML-style Summary
```
AKalkiCharacter  
├─ AbilitySystemComponent  
├─ TacticalAIComponent  
├─ Attributes: UAttributeSet_Stats  
└─ Reference: TacticalPolicy
```


See also → [[Subsystem Architecture]], [[Gameplay Ability System Integration]].

