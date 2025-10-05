---
tags: [root, overview]
---
---
tags: [root, overview, gdd]
title: The Fourth Age — Game Design Vault
---

# 🧭 The Fourth Age — Obsidian Vault

Welcome to the **official design vault** for *The Fourth Age*,  
a **3D party-based CRPG** built in **Unreal Engine 5.6**.  
This vault combines *technical documentation*, *design systems*, *data schemas*, and *narrative frameworks* in a single connected workspace.

---

## ⚔️ Game Overview

**Working Title:** *The Fourth Age*  
**Genre:** Tactical CRPG (3D, Party-Based, Turn-Based/RTwP hybrid)  
**Engine:** Unreal Engine 5.6  
**Team:** Solo Developer (Primary)  
**Tone:** Ancient Indian-inspired mythic realism (Mahajanapada era)  
**Core Philosophy:**  
> Every action is logged by fate itself — the Journal is the world’s memory.

---

## 📁 Folder Structure & Navigation

| Folder              | Description                                                                                         |
| ------------------- | --------------------------------------------------------------------------------------------------- |
| 📘 [[GDD]]          | Core Game Design Documents — pillars, systems, gameplay loops, and production vision.               |
| ⚙️ [[Systems]]      | Detailed subsystem designs — Combat, Unified AI, Journal, Rules, Exploration, Dialogue, UI.         |
| 🧩 [[Technical]]    | Unreal 5.6 architecture — subsystems, class diagrams, GAS integration, networking, build pipeline.  |
| 🗃️ [[Data_Specs]]  | JSON-like schemas for abilities, statuses, actors, items, quests, encounters, events, and factions. |
| 🧭 [[Production]]   | Backlog, milestones, roadmap, risk management, release pipeline.                                    |
| 🌍 [[World]]        | Lore and setting templates — cultures, factions, geography, pantheon, and magic.                    |
| 🧱 [[Narrative]]    | Story structure, quests, characters, companions, and themes.                                        |
| 🗒️ [[Notes_Inbox]] | Scratchpad for quick ideas, todos, or reference links.                                              |

---

## 🧠 Design Pillars

1. **Hybrid Combat:** Turn-Based core, optional RTwP automation (deterministic simulation).
2. **Unified Tactical AI:** One adaptive AI drives all agents — players, enemies, and automation.
3. **JaSoT (Journal-as-Source-of-Truth):** The Journal logs *everything* — combat, dialogue, travel, quests — serving as both debug ledger and in-world chronicle.
4. **Stylized Realism:** Inspired by Indian antiquity, stylized visuals enhanced by Unreal 5.6 features (Lumen, Nanite, Control Rig, Sequencer).
5. **Modular Rule System:** Plug-in architecture supports D&D 5e, PF2e, 13th Age-like, or custom math models.
6. **Solo-Friendly Scope:** Tooling, determinism, and automation built for single-developer production sustainability.

See details → [[GDD/01_Pillars and Vision]].

---

## 🧩 Core Technical Highlights
- 📐 Coding & naming conventions: [[Technical/Unreal Coding Standards (Project)]]

| System | Key Classes / Components |
|---------|--------------------------|
| Combat | `UTFA_TurnManager`, `UTFA_ActionQueue`, `UTFA_CombatStateMachine` |
| AI | `UTFA_TacticalAIComponent`, `UTFA_TacticalPolicy`, `UTFA_ActionEvaluator` |
| Journal | `UTFA_GameLogRouter`, `UTFA_EventStore`, `FTFA_LogEvent` |
| GAS Integration | `UGameplayAbility_TFA`, `UAttributeSet_Stats`, `GameplayTags` |
| Rules Adapter | `URulesAdapter`, `URulePack` DataAssets |
| UI/UX | `UTFA_UIManager`, `WBP_CombatHUD`, `WBP_Journal` |
| Tools | Event Browser, Snapshot Inspector, Ability Editor |

Reference → [[Technical/SubSystem Architecture]], [[Systems/Journal System (Technical)]].

---

## 📅 Production Overview

**Current Phase:** Framework Definition  
**Next Milestone:** Core Combat Prototype (M1)  
**Primary Focus:** Combat + Journal determinism  

🧾 **Backlog management:** [[Production/Backlog]]  
📆 **Milestones:** [[Production/Milestones]]  
🛠️ **Roadmap:** [[Production/Roadmap]]  
🚀 **Release Process:** [[Production/Release Pipeline]]

---

## 🧩 Data & Schema Reference

All data structures use consistent JSON-like formats for quick inspection and serialization.

| Schema | Location |
|---------|-----------|
| Abilities | [[Data_Specs/Ability_Schema]] |
| Statuses | [[Data_Specs/Status_Schema]] |
| Actors | [[Data_Specs/Actor_Schema]] |
| Items | [[Data_Specs/Item_Schema]] |
| Quests | [[Data_Specs/Quest_Schema]] |
| Encounters | [[Data_Specs/Encounter_Schema]] |
| Events (Log) | [[Data_Specs/Event_Schema]] |
| Factions | [[Data_Specs/Faction_Schema]] |

---

## 🌍 Narrative & World Templates

These are intentionally skeletal — ready for expansion.

| Section | Purpose |
|----------|----------|
| [[World/Overview]] | Defines the world and its tone. |
| [[World/Cultures]] | Societies, beliefs, and traditions. |
| [[World/Factions]] | Kingdoms, guilds, and religious orders. |
| [[Narrative/Story Overview]] | Main plot summary. |
| [[Narrative/Companions]] | Player companions and archetypes. |
| [[Narrative/Dialogue Samples]] | Prototype dialogue and tone examples. |

---

## 🔍 How to Use This Vault in Obsidian

### 🧱 Folder View
Use **"Folder Notes"** or **"Explorer"** for quick access.  
Every note cross-links horizontally for flat navigation.

### 🧭 Tag Conventions
| Tag | Meaning |
|------|---------|
| `#technical` | Engine/system architecture |
| `#systems` | Gameplay logic |
| `#data` | Schemas and structured definitions |
| `#production` | Management and planning |
| `#world`, `#narrative` | Creative content |

### 🪶 Backlinks
Use **`Ctrl+Shift+B`** (or sidebar) to trace design relationships — e.g., `Combat` → `AI` → `Journal`.

---

## 🧩 Quick Links

- [[GDD/00_Vision]] – High-level vision statement  
- [[Systems/Combat System (Technical)]] – Combat overview  
- [[Systems/Unified Tactical AI (Technical)]] – Shared AI logic  
- [[Systems/Journal System (Technical)]] – JaSoT details  
- [[Technical/Subsystem Architecture]] – Unreal 5.6 subsystems  
- [[Production/Backlog]] – Current development log  

---

> “The Fourth Age is not merely a time — it is a system of memory.  
> The world remembers every action, and the gods read the logs.”

