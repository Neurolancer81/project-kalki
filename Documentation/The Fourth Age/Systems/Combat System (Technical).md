---
tags: [systems, combat, technical]
---

# Combat System (Technical)

## Overview
All combat in *The Fourth Age* runs in **deterministic Turn-Based (TB)** mode.  
RTwP mode is a **speed overlay** that automates player turns using the same logic.  

The combat layer sits between the **Gameplay Ability System (GAS)** and the **Journal (JaSoT)**, coordinating initiative, actions, resolution, and event emission.

---

## Unreal Subsystems

| Subsystem | Role |
|------------|------|
| **UTFA_WorldSubsystem_Combat** | Central combat manager; owns TurnManager, ActionQueue, and state machine. |
| **UTFA_GameInstanceSubsystem_Journal** | Logs combat events, seeds, and outcomes. |
| **UTFA_PlayerSubsystem_UI** | Feeds combat data to the HUD and logs. |

---

## Core Classes

```
UTFA_WorldSubsystem_Combat  
├─ UTFA_TurnManager  
├─ UTFA_ActionQueue  
├─ UTFA_CombatStateMachine  
├─ UTFA_TargetingService  
├─ UTFA_DeterministicRNG  
└─ UTFA_CombatVisualizer (optional)

```

### `UTFA_TurnManager`
Manages turn order, active actor, and initiative.

**Responsibilities:**
- Build initiative queue at combat start.
- Advance turns deterministically.
- Broadcast delegates (`OnTurnStarted`, `OnTurnEnded`).
- Interface with Journal for event emission.

**Pseudo-code:**
```cpp
void UTFA_TurnManager::AdvanceTurn()
{
    CurrentActor = InitiativeQueue.Next();
    OnTurnStarted.Broadcast(CurrentActor);
    CurrentActor->TakeTurn();
    Journal->LogEvent(EEventType::TurnEnded, CurrentActor);
}
```

### `UTFA_ActionQueue`

Buffers actions (Abilities, Reactions, Delays) until resolved.

- Populated by player input or AI policy.
    
- Each Action references a **GAS Ability**.
    
- Supports nested reaction chains.
    

### `UTFA_CombatStateMachine`

Defines discrete combat phases:

Start → InitiativeRoll → TurnCycle → VictoryCheck → End
### `UTFA_TargetingService`

Handles tile selection, AoE radius, line-of-sight.

### `UTFA_DeterministicRNG`

Per-encounter seeded RNG for replayability.

---

## GAS Integration

- **Abilities**: Implemented via `UGameplayAbility` subclasses.
    
- **AttributeSets**: Hold stats (HP, STR, etc.).
    
- **Effects**: Apply buffs/debuffs; generate `StatusApplied` events.
    
- Each `AbilityTask` emits a `LogEvent` on resolution.
    

---

## Event Flow (Combat)

1. `EncounterStarted`
    
2. `TurnStarted`
    
3. `AbilityActivated`
    
4. `Hit/Miss → DamageApplied`
    
5. `StatusApplied/Removed`
    
6. `TurnEnded`
    
7. `EncounterEnded`
    

Each step emits a structured `FTFA_LogEvent` consumed by the Journal.

---

## RTwP Layer

RTwP overlays a **time dilation** and **Player-AI** control:

- When toggled, `UTFA_TurnManager` continues normally.
    
- Player inputs replaced by AI-generated Actions.
    
- Animations play at accelerated rate (`PlaybackSpeedMultiplier`).
    

---

## Debugging

Use the **Event Browser** to trace a full combat sequence, verify deterministic RNG, and replay from snapshot.

See also → [[Unified Tactical AI (Technical)]], [[Journal System (Technical)]].
