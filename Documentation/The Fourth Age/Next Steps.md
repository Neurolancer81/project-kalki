---
tags: [dashboard, overview, workflow]
title: Developer Dashboard — Next Steps
---

# 📍 Developer Dashboard — *Kalki: The Fourth Age*

Welcome back, developer.  
This page tracks your **current focus**, **short-term goals**, and **direct links** to the most important project files.

> “Every line of code, a verse in fate’s log.”  

---

## 🧩 Current Phase: **Foundation Build (Phase 1)**

### 🎯 Primary Objectives
- ✅ Establish Unreal project architecture (`/Source/Kalki/`)
- ✅ Implement subsystem scaffolds (Combat, Journal, Exploration, UI)
- ✅ Create and load DataAssets (Abilities, Actors, Encounters)
- 🧠 Test deterministic Turn Manager & Journal logging
- 🧰 Build developer tools (Event Browser, RNG Seed Viewer)

📘 **Detailed roadmap:** [[Production/Foundation Roadmap]]

---

## ⚙️ Engineering References
| Topic | Link |
|-------|------|
| Coding & Naming Standards | [[Technical/Unreal Coding Standards (Project)]] |
| Subsystem Layout | [[Technical/Subsystem Architecture]] |
| Class Hierarchy | [[Technical/Class Architecture]] |
| Data Schemas | [[Data_Specs/]] |
| GAS Integration | [[Technical/Gameplay Ability System Integration]] |
| Journal System | [[Systems/Journal System (Technical)]] |

---

## 🗓️ Production Tracking
| Document | Purpose |
|-----------|----------|
| [[Production/Backlog]] | Running to-do list (active) |
| [[Production/Milestones]] | Major deliverables timeline |
| [[Production/Foundation Roadmap]] | Detailed technical step plan |
| [[Production/Roadmap]] | Long-term macro roadmap |
| [[Production/Risk Register]] | Known project risks & mitigations |

---

## 🧠 Design & Worldbuilding
| Area | Description |
|------|--------------|
| [[GDD/00_Vision]] | High-level creative + mechanical intent |
| [[World/Overview]] | Setting and world tone |
| [[Narrative/Story Overview]] | Story and themes |

---

## ⚔️ Active Development Focus
**Subsystems under construction:**
- `UKalkiTurnManager`
- `UKalkiJournalGameInstanceSubsystem`
- `UKalkiCombatWorldSubsystem`

Each must log deterministic events through `FKalkiLogEvent` and propagate to the UI.

---

## 🧰 Developer Tools Setup
- [ ] Enable `Editor Utility Widgets` plugin  
- [ ] Create **Event Browser (UMG)** to view Journal events  
- [ ] Bind console commands:  
  - `kalki.journal.dump`  
  - `kalki.snapshot.save`  
  - `kalki.rng.seed <id>`  

---

## 🔗 Daily Workflow Shortcuts
- **Code reference:** [[Technical/Unreal Coding Standards (Project)]]
- **Today’s tasks:** [[Production/Backlog]]
- **Debug tool ideas:** [[Technical/Debug and Tools]]
- **If lost:** [[Vault_Index]]

---

> 🧩 *“Consistency and clarity are the foundations of single-developer success.”*
