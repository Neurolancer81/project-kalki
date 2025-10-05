---
tags: [gdd, tech]
---

# Technical Architecture — Unreal Engine 5.6

## Core Modules
| Module | Description |
|---------|-------------|
| **Framework** | GameMode, GameState, Subsystems |
| **Combat** | TurnManager, ActionQueue, GAS Integration |
| **AI** | Unified Tactical AI (Behavior Trees + Policies) |
| **Rules** | Data-driven math formulas |
| **Journal** | JaSoT (Router, Store, Snapshot) |
| **UI** | HUD, Journal, Tooltip Renderer |
| **Networking** | Turn replication, authoritative Journal |

## Subsystem Hierarchy
```GameInstanceSubsystem  
├─ JournalRouter  
├─ SnapshotManager  
WorldSubsystem  
├─ CombatManager  
├─ ExplorationManager  
PlayerSubsystem  
├─ UIManager  
└─ InputProfile
```


## Key Unreal 5.6 features leveraged
- **GAS** (Gameplay Ability System)  
- **Enhanced Input**  
- **Control Rig + Sequencer**  
- **Lumen/Nanite Rendering**  
- **World Partition (optional)**  
- **Subsystems Framework**

See also → [[Subsystem Architecture]], [[Combat System (Technical)]], [[Unified Tactical AI (Technical)]].
