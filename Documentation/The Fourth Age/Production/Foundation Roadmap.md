---
tags:
  - production
  - roadmap
  - foundation
title: "Foundation Roadmap — Kalki: The Fourth Age"
---
# 🧱 Foundation Roadmap (Development Phases)

This roadmap captures the practical, step-by-step implementation plan to go from the **Obsidian GDD** to a **runnable, modular Unreal prototype**.

---

## 🧭 Phase 1 — Foundation Build (Core Architecture)

**Goal:** Establish all subsystems, data structures, and initialization flow.

### ✅ Tasks
1. **Project Setup**
   - Create clean Unreal 5.6 C++ project `Kalki`.
   - Implement folder layout `/Source/Kalki/` (Core, Systems, Data, DevTools).
   - Configure `Kalki.Build.cs` and add required modules (`GameplayAbilities`, `UMG`, `EnhancedInput`, `Niagara`).

2. **Core Classes**
   - Implement `AKalkiGameMode`, `AKalkiGameState`, `AKalkiPlayerController`, `AKalkiPlayerState`.
   - Build subsystem scaffolding:
     - `UKalkiJournalGameInstanceSubsystem`
     - `UKalkiCombatWorldSubsystem`
     - `UKalkiExplorationWorldSubsystem`
     - `UKalkiOverworldWorldSubsystem`
     - `UKalkiUIPlayerSubsystem`

3. **Deterministic Turn System**
   - Create `UKalkiTurnManager`, `UKalkiActionQueue`, `UKalkiCombatStateMachine`.
   - Simulate turn order with log outputs.

4. **Journal System (JaSoT Core)**
   - Define `FKalkiLogEvent` struct.
   - Implement `UKalkiGameLogRouter`, `UKalkiEventStore`, and `UKalkiJournalView`.
   - Log mock combat and dialogue events to console.

5. **Data Schema Integration**
   - Implement DataAssets: Abilities, Statuses, Actors, Quests, Encounters.
   - Validate schema loads via editor.
   - Begin tagging rulesets (e.g., `RulePack_5e`, `RulePack_Custom`).

6. **Test Simulation**
   - Mock combat with text logs:
     ```
     [Turn] Player attacks Bandit → Hit (14 dmg)
     [Turn] Bandit attacks Player → Miss
     ```
   - Journal records events with correct metadata and seeds.

7. **Tooling**
   - Build initial **Event Browser** (UMG Widget) for debugging Journal logs.
   - Add `tfa.journal.dump` → now `kalki.journal.dump` console command.

✅ **Success criteria:**  
- Engine runs with no subsystem load errors.  
- Journal logs reflect all key actions.  
- Turn loop deterministic and replayable.

---

## ⚔️ Phase 2 — Vertical Slice (Playable Prototype)

**Goal:** Deliver a small, replayable slice of full gameplay.

### ✅ Tasks
1. **Combat MVP**
   - Implement ability execution and resource tracking.
   - Add RTwP simulation (AI auto-controls player turns with time dilation).
   - Integrate UI widgets (`WBP_CombatHUD`, `WBP_Journal`).

2. **Unified Tactical AI**
   - Implement single AI framework for both enemies and Player-AI.
   - Data-driven tactical policies (aggressive, defensive, cautious).

3. **Exploration Mode**
   - Add simple level navigation and interaction events.
   - Log discoveries in Journal.

4. **Overworld Travel**
   - Implement node-based map navigation.
   - Random encounter triggering.

5. **Save/Load Prototype**
   - Journal snapshot system (`UKalkiSnapshotManager`).

6. **Rule Adapter**
   - Plug in multiple rule packs (5e-like, PF2e-like, custom math formulas).

7. **Visual Pass**
   - Stylized post-processing (Lumen + minimal Nanite assets).
   - Use placeholder characters (MetaHuman optional).

✅ **Success criteria:**  
- Fully playable combat scenario with event replay.  
- Journal persists through reload.  
- Swappable rulesets and replay fidelity.

---

## 🛠️ Phase 3 — Stabilization & Multiplayer

**Goal:** Finalize systems, add multiplayer TB mode, and prepare vertical slice for showcase.

### ✅ Tasks
1. **Optimization & Refactor**
   - Consolidate logging and seed control.
   - Refactor subsystems for network determinism.

2. **Multiplayer Implementation**
   - TB-only co-op for up to 6 players.
   - Journal synchronization server-authoritative.

3. **Save/Replay**
   - Journal-based save states verified through replays.
   - “Load and resume” feature from JaSoT log.

4. **Tool Polishing**
   - Expand Event Browser, Snapshot Inspector, and Tag Visualizer.
   - Build internal debug dashboard.

5. **Pre-Release Preparation**
   - QA pass for determinism and consistency.
   - Package `v0.1` build.

✅ **Success criteria:**  
- Stable deterministic multiplayer.  
- Logs and snapshots can fully reproduce sessions.  
- Clean modular systems with documented interfaces.

---

## 🔁 Maintenance Practices

- Keep this roadmap updated after each milestone.
- Version control every system (Git branching per subsystem).
- Tag commits with roadmap task IDs (e.g., `Phase1_03_TurnManager`).
- Log retrospective notes into [[Production/Backlog]] after each phase.

---

📎 **Related References**
- [[Production/Backlog]]
- [[Production/Roadmap]]
- [[Technical/Subsystem Architecture]]
- [[Technical/Class Architecture]]
- [[Systems/Journal System (Technical)]]