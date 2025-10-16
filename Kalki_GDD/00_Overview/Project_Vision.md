---
tags:
  - kalki/overview
  - kalki/vision
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
---

# Project Vision

## Overview
Kalki is a tactical role-playing game inspired by Dungeons & Dragons 5th Edition, combining deep turn-based combat with character progression and storytelling. This document defines the core vision, design pillars, and target experience.

---

## The Elevator Pitch

**"Baldur's Gate 3 meets XCOM"**

A D&D 5e tactical RPG where:
- **Strategic Combat** - Grid-based tactical battles with elevation and positioning
- **Character Depth** - D&D 5e classes, abilities, and progression
- **Meaningful Choices** - Decisions that impact story, characters, and gameplay
- **Multiplayer Ready** - Co-op campaign and competitive modes

---

## Core Design Pillars

### 1. Tactical Depth
**"Every decision matters"**

**Combat should be:**
- Grid-based with clear tactical feedback
- Elevation and positioning are crucial
- Abilities have meaningful choices (not just "bigger numbers")
- Risk vs reward decisions every turn
- Multiple solutions to encounters

**Inspired by:**
- XCOM 2 (tactical positioning, cover, elevation)
- Into the Breach (perfect information, puzzle-like)
- Divinity Original Sin 2 (environmental interactions)

---

### 2. D&D 5e Authenticity
**"Faithful to the source"**

**Rules implementation:**
- Core D&D 5e mechanics (classes, abilities, spells)
- Action economy (action, bonus action, reaction, movement)
- Dice rolls and probability
- Advantage/disadvantage system
- Concentration mechanics

**NOT a 1:1 translation:**
- Real-time with pause (not pure turn-based)
- Streamlined for video game pace
- Quality of life improvements
- PC-friendly adaptations

**Inspired by:**
- Baldur's Gate 3 (faithful but adapted)
- Solasta (D&D rules focus)

---

### 3. Character Investment
**"Your character, your story"**

**Players should feel:**
- Character progression is meaningful
- Builds have identity and playstyle
- Abilities evolve and unlock new tactics
- Choices in character development matter

**Features:**
- Multiple classes (Fighter, Wizard, Rogue, Cleric, etc.)
- Subclasses and specializations
- Feat system
- Equipment and loot that changes playstyle

**Inspired by:**
- Baldur's Gate series (deep character builds)
- Pathfinder: Wrath of the Righteous (build variety)

---

### 4. Multiplayer Viability
**"Better with friends"**

**Multiplayer modes:**
- Co-op campaign (2-4 players)
- Competitive arenas (PvP tactical battles)
- Hot-seat tactical puzzles

**Technical foundation:**
- Server authority from day one
- Deterministic replication
- Lag-tolerant (turn-based helps)
- Drop-in/drop-out friendly

**Inspired by:**
- Divinity Original Sin 2 (excellent co-op)
- XCOM 2 (challenging co-op missions)

---

## Target Audience

### Primary Audience
**Tactical RPG enthusiasts (25-45 years old)**

**They enjoy:**
- Deep, strategic gameplay
- Character customization
- Turn-based or pausable combat
- D&D (tabletop or video games)

**They play:**
- XCOM series
- Baldur's Gate 3
- Divinity Original Sin 2
- Fire Emblem series
- Pathfinder games

---

### Secondary Audience
**D&D fans exploring video games**

**They want:**
- Authentic D&D experience
- Less focus on real-time reflexes
- Story and character focus
- Familiar rules and mechanics

---

## Core Gameplay Loop

### Moment-to-Moment (Combat)
```
1. Survey battlefield
   ↓
2. Plan turn (movement, abilities, positioning)
   ↓
3. Execute actions
   ↓
4. Enemy turn (react, adapt)
   ↓
5. Adjust strategy
   ↓
[Repeat until victory/defeat]
```

---

### Session-to-Session (Campaign)
```
1. Story/dialogue sequences
   ↓
2. Make narrative choices
   ↓
3. Tactical combat encounters
   ↓
4. Loot and rewards
   ↓
5. Character progression (level up, gear)
   ↓
6. Rest/recover (camp, shop)
   ↓
[Repeat]
```

---

## Game Modes

### 1. Campaign (Primary)
**Single-player or co-op story experience**

- Linear narrative with branching choices
- 20-30 hours main story
- 40-60 hours with side content
- Character progression from level 1 to 10-12
- Multiple difficulty settings

---

### 2. Skirmish (Secondary)
**Standalone tactical battles**

- Quick tactical scenarios (15-30 minutes)
- Pre-made or custom teams
- Various objectives (elimination, capture, survival)
- Leaderboards and challenges

---

### 3. Arena (Multiplayer)
**PvP tactical combat**

- 1v1 or 2v2 battles
- Draft or bring-your-own team
- Turn-based tactical combat
- Ranked and casual modes

---

## Technical Foundation

### Engine
**Unreal Engine 5.3+**

**Why Unreal:**
- ✅ Excellent for tactical games (XCOM 2, Gears Tactics)
- ✅ Blueprint + C++ flexibility
- ✅ Strong multiplayer support
- ✅ Great visual fidelity
- ✅ Active community and marketplace

---

### Target Platforms

**Primary:** PC (Steam)
- Mouse + keyboard (primary)
- Controller support (secondary)

**Future:** Console (PS5, Xbox Series X/S)
- Controller-first UI
- Performance optimization

---

### Performance Targets

**PC:**
- 60 FPS @ 1080p (mid-range hardware)
- 30 FPS minimum
- Scalable graphics (low to ultra)

**Console:**
- 60 FPS @ 1080p (performance mode)
- 30 FPS @ 4K (quality mode)

---

## Art Direction

### Visual Style
**Stylized Realism**

- Not photorealistic, not cartoony
- Hand-painted textures with clean lines
- Readable at tactical camera distance
- Character personality shows through design

**References:**
- Baldur's Gate 3 (character detail)
- XCOM 2 (clean tactical visuals)
- Hades (stylized but detailed)

---

### UI/UX Philosophy
**"Clear, not cluttered"**

**Principles:**
- Information at a glance (HP, AP, status effects)
- Minimal chrome (UI doesn't obscure action)
- Consistent visual language
- Tooltips for everything
- Context-sensitive (only show relevant info)

**References:**
- Into the Breach (perfect information display)
- Slay the Spire (card game clarity)
- XCOM 2 (tactical overlay)

---

### Camera
**Isometric Tactical Camera**

- Fixed isometric angle (classic tactical view)
- Zoom in/out
- Rotate in 90° increments
- Free camera mode (for screenshots, exploration)

---

## D&D 5e Adaptation

### What We Keep
**Core mechanics:**
- ✅ Ability scores (STR, DEX, CON, INT, WIS, CHA)
- ✅ Classes and subclasses
- ✅ Spells and abilities
- ✅ D20 rolls (attack, saves, checks)
- ✅ Advantage/disadvantage
- ✅ Concentration
- ✅ Reactions
- ✅ Action economy

---

### What We Adapt
**Streamlined for video games:**

**Initiative:**
- D&D: Roll initiative, turn order
- Kalki: Player phase → Enemy phase (XCOM-style)

**Movement:**
- D&D: Move up to speed (30 ft)
- Kalki: Movement points (6 tiles typical)

**Exploration:**
- D&D: Theater of the mind, skill checks
- Kalki: Story beats, cinematic sequences, dialogue trees

**Resting:**
- D&D: Short/long rests
- Kalki: Camp system (story moments, character interactions)

---

### What We Cut (For Now)
**Too complex or not fun in video game:**
- ❌ Spell components (material, somatic)
- ❌ Weight/encumbrance (unless meaningful)
- ❌ Food/water tracking
- ❌ Disease/poison (beyond status effects)
- ❌ Reaction spam (limit to key abilities)

---

## Scope and Priorities

### MVP (Minimum Viable Product)
**Goal: Playable tactical core**

**Must Have:**
- ✅ Grid-based movement
- ✅ Basic combat (attack, damage, HP)
- ✅ 2-3 classes (Fighter, Wizard, Rogue)
- ✅ 5-10 core abilities per class
- ✅ Turn-based combat flow
- ✅ 3-5 combat encounters (proof of concept)
- ✅ Basic UI (HP, AP, abilities)

**Can Skip:**
- Story/narrative
- Voice acting
- Full campaign
- Multiplayer
- Advanced classes
- Loot system

---

### Phase 1: Core Systems (Current)
**Foundation for everything else**

- ✅ Logging system
- ✅ Grid system
- ✅ Grid visualization
- ⏳ Pathfinding (next)
- ⏳ Character movement
- ⏳ Basic combat
- ⏳ UI framework

---

### Phase 2: Gameplay Loop
**Make it fun to play**

- Combat system (attacks, damage, death)
- Ability system (casting, targeting, effects)
- Turn system (player/enemy phases)
- AI (basic enemy behavior)
- 3 playable classes
- 5 enemy types
- 10 encounters

---

### Phase 3: Polish and Content
**Make it a complete experience**

- Campaign structure
- Progression system
- More classes and abilities
- Loot and equipment
- Advanced AI
- Visual polish
- Sound and music

---

### Phase 4: Multiplayer
**Extend the experience**

- Netcode finalization
- Co-op campaign
- PvP arenas
- Balance tuning
- Dedicated servers

---

## Success Metrics

### Player Experience Goals
**What we want players to feel:**

- ✅ **"One more turn"** - Combat is engaging
- ✅ **"That was clutch!"** - Moments of triumph
- ✅ **"I should try that build"** - Replayability
- ✅ **"We crushed that!"** - Co-op satisfaction
- ✅ **"I'm a tactical genius"** - Outsmart AI

---

### Quality Bars

**Combat:**
- Players understand options within 5 seconds
- Clear feedback on hit/miss/damage
- Abilities feel impactful (visuals + audio)
- Enemy AI provides challenge (not artificial difficulty)

**Performance:**
- 60 FPS in combat (PC mid-range)
- No loading during combat
- Quick restarts (under 3 seconds)

**Polish:**
- No critical bugs in release
- All tooltips accurate and helpful
- UI is readable and clear
- Camera never obscures important info

---

## Inspirations and References

### Core Inspirations

**XCOM 2 (Tactical Combat):**
- Grid-based positioning
- Cover system
- Elevation matters
- Overwatch and reactions
- "That's XCOM, baby" moments

**Baldur's Gate 3 (D&D Authenticity):**
- Faithful to D&D 5e
- Dice rolls and probability
- Character depth
- Narrative integration
- Environmental interactions

**Divinity Original Sin 2 (Co-op):**
- Excellent co-op integration
- Environmental combos
- Character builds
- Replayability

**Into the Breach (Tactical Clarity):**
- Perfect information
- Clear consequences
- Puzzle-like encounters
- Tight gameplay loop

---

### Secondary References

**Fire Emblem series:**
- Character relationships
- Permadeath tension
- Class progression

**Darkest Dungeon:**
- Party composition
- Stress/affliction systems
- Risk management

**Slay the Spire:**
- Build variety
- Clear information
- Iterative gameplay

---

## Design Principles

### 1. Clarity Over Complexity
**Players should always understand:**
- What can I do?
- What will happen if I do X?
- Why did that happen?

---

### 2. Meaningful Choices
**Every decision should matter:**
- No "correct" choice
- Risk vs reward
- Tradeoffs, not upgrades

---

### 3. Emergent Gameplay
**Systems interact in interesting ways:**
- Abilities combo with each other
- Terrain affects tactics
- Enemy composition changes strategy

---

### 4. Respect Player Time
**No artificial padding:**
- Fast combat (30 seconds per turn ideal)
- Quick restarts
- Skip animations option
- Save anywhere

---

### 5. Fail Forward
**Failure should be:**
- Educational (learn what works)
- Recoverable (not game over)
- Optional (difficulty settings)

---

## Not Our Game

**To maintain focus, we explicitly avoid:**

### Not a Platformer
- No jumping puzzles
- No reflex-based challenges
- Grid movement only

### Not a Sandbox
- Not open world
- Linear with branches (not full freedom)
- Authored encounters (not procedural)

### Not a Roguelike
- Not permadeath focus
- Not run-based
- Progression persists

### Not a Strategy Game
- Not base building
- Not resource management focus
- Not 4X gameplay

### Not a Survival Game
- Not crafting focus
- Not hunger/thirst mechanics
- Not scavenging emphasis

---

## Future Vision (Post-Launch)

### DLC/Expansions
- New classes and subclasses
- New campaign chapters
- New enemy types
- New game modes

### Community Features
- Level editor
- Custom campaigns
- Workshop support
- Modding tools

### Long-Term Support
- Balance patches
- Quality of life improvements
- Community events
- Seasonal content

---

## Why This Game Matters

### For Players
**A tactical RPG that:**
- Respects their intelligence
- Offers meaningful choices
- Provides replayability
- Works great solo or co-op
- Captures D&D magic in video game form

### For Us
**A project that:**
- Showcases technical skill
- Demonstrates game design mastery
- Builds a community
- Has commercial viability
- Is something we'd want to play

---

## Constraints and Considerations

### Team Size
**Solo developer (with AI assistance)**

**Implications:**
- Scope must be realistic
- Focus on systems over content
- Modular design for expansion
- Community can contribute content

---

### Budget
**Indie/self-funded**

**Implications:**
- Marketplace assets acceptable
- Focus on mechanics over graphics
- Stylized art (timeless, cheaper)
- Smart scope management

---

### Timeline
**Flexible but focused**

**Target:**
- MVP: 3-6 months
- Alpha: 6-12 months
- Beta: 12-18 months
- Release: 18-24 months

---

## Related Documents

### Foundation
- [[00_Overview/Current_Status]] - Where we are now
- [[00_Overview/Technical_Stack]] - Technologies used

### Architecture
- [[01_Architecture/Project_Structure]] - How code is organized
- [[01_Architecture/Module_Overview]] - System breakdown

### Roadmap
- [[07_Roadmap/Phase_3_Pathfinding]] - Next immediate goal
- [[07_Roadmap/Phase_4_Character_Movement]] - Following phase
- [[07_Roadmap/Future_Systems]] - Long-term plans

### Implementation
- [[04_Implementation/Common_Patterns]] - How we build features
- [[04_Implementation/Multiplayer_Guidelines]] - Technical considerations

### Systems
- [[03_Systems/Grid_System]] - Example completed system
- [[03_Systems/MVVM_Pattern]] - UI architecture

---

## Document Info
**File Path:** `Kalki_GDD/00_Overview/Project_Vision.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Purpose:** Define what we're building and why