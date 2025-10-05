---
tags: [systems, overworld]
---

# Overworld System

## Concept
Strategic map composed of nodes (locations) and edges (routes).  
Travel is time/resource based, triggering random encounters.

---

## Components
| Class | Description |
|--------|-------------|
| `UTFA_WorldSubsystem_Overworld` | Controls travel and events. |
| `UTFA_NodeData` | DataAsset for map nodes. |
| `UTFA_EncounterManager` | Handles random rolls & transitions. |

---

## Flow
1. Player selects destination node.  
2. System computes travel time.  
3. Rolls for random events (deterministic RNG).  
4. Emits `TravelSegmentCompleted` event.  
5. Encounters trigger `LoadCombatLevel()`.

---

## Journal Integration
Every travel segment emits a `TravelStarted` and `TravelEnded` event for timeline reconstruction.

See also → [[Exploration System]], [[Journal System (Technical)]].