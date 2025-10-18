---
tags:
  - kalki/reference
  - kalki/terminology
  - kalki/definitions
  - type: reference
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Glossary

## Overview
This document defines all terminology, acronyms, and concepts used throughout the Kalki project documentation. It serves as a quick reference for understanding project-specific and domain-specific terms.

---

## Purpose

### Why a Glossary?

**Benefits:**
- ✅ Clear definitions of project terms
- ✅ Consistent terminology usage
- ✅ Onboarding new team members
- ✅ Reference for documentation
- ✅ Avoid confusion and ambiguity

**How to Use:**
- Terms organized alphabetically
- Cross-references to related terms
- Links to relevant documentation
- Examples where helpful

---

## A

### Ability
A special action a character can perform in combat, such as an attack, spell, or skill. Abilities typically have costs (action points, mana) and may have cooldowns.

**See also:** [[Cooldown]], [[Action Points]]  
**Documentation:** [[03_Systems/Ability_System]]

---

### Ability Score
In D&D 5e, one of six core statistics (Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma) that define a character's capabilities.

**See also:** [[Modifier]], [[D&D 5e]]

---

### AC (Armor Class)
A D&D 5e mechanic representing how difficult it is to land a hit on a character. Higher AC = harder to hit.

**Formula:** Base AC + Dexterity modifier + armor bonus

**See also:** [[Attack Roll]], [[D&D 5e]]

---

### Action Points (AP)
Resource used to perform actions during a character's turn. Different actions cost different amounts of AP.

**Example:** 
- Move: 1 AP
- Basic Attack: 1 AP
- Special Ability: 2 AP

**See also:** [[Turn]], [[Ability]]

---

### Advantage
D&D 5e mechanic where you roll two d20s and take the higher result. Granted by favorable conditions.

**Opposite:** [[Disadvantage]]  
**See also:** [[D20 Roll]], [[D&D 5e]]

---

### AI (Artificial Intelligence)
In Kalki, refers to computer-controlled enemy characters and their decision-making systems.

**See also:** [[Enemy]], [[Behavior Tree]]  
**Documentation:** [[03_Systems/AI_System]]

---

### AOE (Area of Effect)
An ability that affects multiple tiles in a specified area, rather than a single target.

**Examples:**
- Fireball (3-tile radius)
- Cone of Cold (5-tile cone)
- Chain Lightning (line)

**See also:** [[Ability]], [[Targeting]]

---

### API (Application Programming Interface)
The set of functions, classes, and methods exposed for use by other systems.

**Example:** KalkiGridManager API provides functions to query and modify the grid.

**Documentation:** [[05_API_Reference/]]

---

### Asset
Any content file used in the game: meshes, textures, materials, sounds, animations, etc.

**See also:** [[Content]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Authority
In Unreal's networking model, the machine (server) that has authoritative control over gameplay state.

**See also:** [[Server]], [[Client]], [[Replication]]  
**Documentation:** [[04_Implementation/Multiplayer_Guidelines]]

---

## B

### Behavior Tree
AI decision-making system that uses a tree structure to determine what actions an AI should take.

**See also:** [[AI]], [[Enemy]]

---

### Blueprint
Unreal Engine's visual scripting system. In Kalki, used primarily for UI and level design.

**Counterpart:** C++ (used for core systems)  
**Prefix:** BP_ (e.g., BP_Character_Warrior)

---

### Bounds
The valid range or area for something. Grid bounds = the valid coordinates within the grid.

**Example:** Grid bounds for 20×20 grid: X[0-19], Y[0-19]

**See also:** [[Validation]]

---

## C

### Cardinal Direction
The four main directions on a grid: North, South, East, West (or Up, Down, Left, Right).

**Contrast:** [[Diagonal Direction]]  
**See also:** [[Grid]], [[Neighbor]]

---

### Channel (Logging)
A category for log messages (Combat, Grid, UI, etc.) used to filter and organize logs.

**Available Channels:**
- System, Combat, Movement, Abilities, TurnSystem, Grid, Dialogue, Inventory, AI, Network, Quest, Ruleset, Debug, UI

**See also:** [[KalkiLog]], [[Severity]]  
**Documentation:** [[05_API_Reference/KalkiLog_API]]

---

### Chebyshev Distance
Distance metric where diagonal movement counts as 1 step. The maximum of the absolute differences in coordinates.

**Formula:** max(|X1 - X2|, |Y1 - Y2|)

**See also:** [[Manhattan Distance]], [[Euclidean Distance]]  
**Documentation:** [[03_Systems/Grid_Coordinate_System]]

---

### Client
In multiplayer, a player's game instance that connects to a server. Clients do not have authority over game state.

**See also:** [[Server]], [[Authority]], [[Replication]]

---

### Combat Log
UI element that displays a chronological record of combat events (attacks, damage, abilities used, etc.).

**Implementation:** MVVM pattern with ViewModel  
**See also:** [[MVVM]], [[ViewModel]]  
**Documentation:** [[03_Systems/Combat_Log_System]]

---

### Component
In Unreal, a modular piece of functionality attached to an actor (e.g., MovementComponent, MeshComponent).

**Prefix:** UC (e.g., UKalkiStatsComponent)  
**Documentation:** [[04_Implementation/Class_Templates]]

---

### Condition
A status effect applied to a character that modifies their capabilities (Stunned, Prone, Poisoned, etc.).

**See also:** [[Status Effect]], [[D&D 5e]]

---

### Cooldown
The time or turns that must pass before an ability can be used again after being cast.

**Example:** Fireball has a 3-turn cooldown

**See also:** [[Ability]], [[Turn]]

---

### Coordinate
A position on the grid, defined by X and Y values.

**Type:** FKalkiGridCoord  
**Example:** (5, 7) = X:5, Y:7  
**See also:** [[Grid]], [[Tile]]  
**Documentation:** [[03_Systems/Grid_Coordinate_System]]

---

### Critical Hit
In D&D 5e, rolling a natural 20 on an attack roll, resulting in extra damage.

**Opposite:** [[Critical Miss]]  
**See also:** [[Attack Roll]], [[D20 Roll]]

---

### Critical Miss
In D&D 5e, rolling a natural 1 on an attack roll, resulting in an automatic miss.

**Opposite:** [[Critical Hit]]  
**See also:** [[Attack Roll]], [[D20 Roll]]

---

## D

### D20 Roll
Rolling a 20-sided die, the core mechanic of D&D 5e used for attack rolls, saving throws, and ability checks.

**Notation:** d20 or 1d20  
**See also:** [[D&D 5e]], [[Advantage]], [[Disadvantage]]

---

### D&D 5e
Dungeons & Dragons Fifth Edition - the tabletop RPG ruleset that Kalki is based on.

**Key Mechanics:**
- D20 roll system
- Six ability scores
- Armor Class
- Advantage/Disadvantage
- Proficiency bonus

**Documentation:** [[01_Architecture/Project_Overview]]

---

### Damage
The reduction in a character's hit points, typically from attacks or hazards.

**See also:** [[HP]], [[Attack]]

---

### Data Binding
In MVVM, the automatic synchronization between ViewModel data and Widget display.

**See also:** [[MVVM]], [[ViewModel]], [[Widget]]  
**Documentation:** [[03_Systems/MVVM_Pattern]]

---

### Delegate
Unreal's event system. A delegate can broadcast to multiple listeners.

**Types:**
- Single-cast delegate
- Multi-cast delegate
- Dynamic delegate (Blueprint-compatible)

**Example:** OnDataChanged delegate in ViewModels

---

### Deterministic
Producing the same result given the same input. Important for networking to ensure all clients see the same game state.

**See also:** [[Replication]], [[Multiplayer]]

---

### Diagonal Direction
The four corner directions on a grid: NE, NW, SE, SW.

**Contrast:** [[Cardinal Direction]]  
**See also:** [[Grid]], [[Neighbor]]

---

### Disadvantage
D&D 5e mechanic where you roll two d20s and take the lower result. Applied by unfavorable conditions.

**Opposite:** [[Advantage]]  
**See also:** [[D20 Roll]], [[D&D 5e]]

---

## E

### Elevation
The vertical height of a tile on the grid. Affects line of sight, movement cost, and tactical positioning.

**Type:** float (world units)  
**See also:** [[Tile]], [[Grid]]  
**Documentation:** [[03_Systems/Grid_System]]

---

### Enemy
A hostile AI-controlled character that opposes the player in combat.

**See also:** [[AI]], [[NPC]]

---

### Euclidean Distance
The straight-line distance between two points. "As the crow flies."

**Formula:** √((X1-X2)² + (Y1-Y2)²)

**See also:** [[Manhattan Distance]], [[Chebyshev Distance]]  
**Documentation:** [[03_Systems/Grid_Coordinate_System]]

---

### Event
A notification that something has occurred, typically used to trigger responses in other systems.

**Implementation:** Delegates in Unreal  
**Example:** OnGridCreated, OnDataChanged, OnTileChanged

**See also:** [[Delegate]]

---

## F

### FKalkiGridCoord
The struct used to represent a coordinate on the grid.

**Properties:** X (int32), Y (int32)  
**Example:** FKalkiGridCoord(5, 7)  
**Documentation:** [[03_Systems/Grid_Coordinate_System]]

---

### FKalkiGridTile
The struct containing all data about a single grid tile.

**Properties:**
- Coordinate
- Elevation
- bWalkable
- Occupant
- WorldPosition

**Documentation:** [[03_Systems/Grid_System]]

---

### Flanking
A tactical advantage gained by attacking an enemy from the side or rear.

**Benefit:** Often grants advantage or bonus damage  
**See also:** [[Advantage]], [[Positioning]]

---

### Fog of War
Areas of the map that are hidden from the player's view, revealing only areas they can see.

**See also:** [[Line of Sight]], [[Vision]]

---

## G

### Game Mode
Unreal's class that defines the rules and flow of a game session.

**Prefix:** A (e.g., AKalkiGameMode)  
**Responsibilities:** Win conditions, player spawning, game rules

---

### GDD (Game Design Document)
The documentation that defines all aspects of the game's design, systems, and implementation.

**Location:** `Kalki_GDD/`  
**This document is part of the GDD**

---

### Git
Version control system used to track code changes and collaborate on the project.

**See also:** [[Repository]], [[Commit]]  
**Documentation:** [[02_Conventions/Version_Control_Guidelines]]

---

### Git LFS (Large File Storage)
Git extension for handling large binary files (textures, meshes, audio).

**Why needed:** Git doesn't handle large files well  
**Documentation:** [[02_Conventions/Version_Control_Guidelines]]

---

### Grid
The tactical battlefield represented as a 2D array of tiles. Characters move and fight on the grid.

**Type:** Square grid (not hex)  
**See also:** [[Tile]], [[Coordinate]]  
**Documentation:** [[03_Systems/Grid_System]]

---

## H

### Hazard
Environmental danger on the grid that can damage or affect characters (fire, spikes, poison gas, etc.).

**See also:** [[Tile]], [[Environment]]

---

### Hit Points (HP)
A character's health. When HP reaches 0, the character dies.

**See also:** [[Damage]], [[Death]]

---

### Hover
The visual feedback shown when the mouse cursor is over a tile.

**See also:** [[Selection]], [[Highlighting]]  
**Documentation:** [[03_Systems/Grid_Visualizer]]

---

## I

### Initiative
D&D 5e mechanic that determines turn order in combat. Higher initiative = earlier turn.

**Calculation:** D20 + Dexterity modifier  
**See also:** [[Turn Order]], [[Turn System]]

---

### Instance
An individual occurrence of a class. Multiple instances can exist from the same class.

**Example:** Multiple characters are instances of AKalkiCharacter

---

### Instanced Static Mesh
Unreal rendering optimization that draws many identical meshes in a single draw call.

**Use case:** Grid visualization (400 tiles = 1 draw call)  
**See also:** [[Performance]], [[Grid Visualizer]]  
**Documentation:** [[03_Systems/Grid_Visualizer]]

---

## K

### KalkiLog
The static logging utility class used throughout Kalki for logging messages.

**Usage:** `KalkiLog::Grid(TEXT("Message"));`  
**See also:** [[Channel]], [[Severity]]  
**Documentation:** [[05_API_Reference/KalkiLog_API]]

---

## L

### Level (Map)
An Unreal map file containing a game environment/level.

**Prefix:** L_ (e.g., L_Combat_Arena)  
**See also:** [[Arena]], [[Environment]]

---

### Level (Character)
A character's progression level, indicating their power and experience.

**Range:** 1-10 (in Kalki v1)  
**See also:** [[XP]], [[Progression]]

---

### LFS
See [[Git LFS]]

---

### Line of Sight (LOS)
Whether a character can see another character or location, used for targeting and vision.

**Blocked by:** Walls, obstacles, high elevation differences  
**See also:** [[Fog of War]], [[Targeting]]

---

### LOD (Level of Detail)
Multiple versions of a mesh at different polygon counts, switching based on distance from camera.

**Example:**
- LOD0: 5000 tris (close)
- LOD1: 2000 tris (medium)
- LOD2: 500 tris (far)

**See also:** [[Performance]], [[Optimization]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

## M

### Manhattan Distance
Grid distance metric where you can only move in cardinal directions (no diagonals). Sum of absolute coordinate differences.

**Formula:** |X1 - X2| + |Y1 - Y2|

**Use case:** Movement range, ability range in Kalki  
**See also:** [[Euclidean Distance]], [[Chebyshev Distance]]  
**Documentation:** [[03_Systems/Grid_Coordinate_System]]

---

### Material
Unreal asset that defines how a surface looks (color, roughness, metallic, etc.).

**Prefix:** M_ (e.g., M_GridTile)  
**See also:** [[Material Instance]], [[Texture]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Material Instance
A parameter-based variant of a master material. Cheaper to create and better for performance.

**Prefix:** MI_ (e.g., MI_GridTile_Blue)  
**See also:** [[Material]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Melee
Close-range combat, requiring the attacker to be adjacent to the target.

**Opposite:** [[Ranged]]  
**See also:** [[Attack]], [[Ability]]

---

### Mesh
3D model geometry.

**Types:**
- Static Mesh (SM_)
- Skeletal Mesh (SK_)

**See also:** [[Asset]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Modifier
In D&D 5e, a bonus or penalty derived from an ability score.

**Formula:** (Ability Score - 10) / 2, rounded down  
**Example:** Strength 16 = +3 modifier

**See also:** [[Ability Score]], [[D&D 5e]]

---

### Movement Range
The maximum distance a character can move in one turn, measured in tiles.

**Typical values:** 3-6 tiles  
**See also:** [[Action Points]], [[Manhattan Distance]]

---

### MVVM (Model-View-ViewModel)
UI architecture pattern that separates data (Model), presentation (View/Widget), and logic (ViewModel).

**Benefits:**
- Separation of concerns
- Easier testing
- Reusable ViewModels

**See also:** [[ViewModel]], [[Widget]]  
**Documentation:** [[03_Systems/MVVM_Pattern]]

---

## N

### Neighbor
A tile adjacent to another tile. Can be cardinal (4) or include diagonals (8).

**See also:** [[Cardinal Direction]], [[Diagonal Direction]], [[Grid]]  
**Documentation:** [[03_Systems/Grid_Coordinate_System]]

---

### Network Authority
See [[Authority]]

---

### NPC (Non-Player Character)
A character controlled by the game rather than a player. Can be friendly, neutral, or hostile.

**See also:** [[Enemy]], [[AI]]

---

## O

### Occupant
The actor (usually a character) currently positioned on a tile.

**Property:** TWeakObjectPtr in FKalkiGridTile  
**See also:** [[Tile]], [[Character]]

---

### Optimization
Improving game performance (frame rate, memory usage, load times, etc.).

**See also:** [[Performance]]  
**Documentation:** [[04_Implementation/Performance_Guidelines]]

---

## P

### Pathfinding
Algorithm for finding a valid path from one tile to another, avoiding obstacles.

**Algorithm:** A* (A-star)  
**See also:** [[Grid]], [[Movement]]

---

### Performance
How efficiently the game runs (frame rate, memory, load times).

**Target:** 60 FPS @ 1080p  
**See also:** [[Optimization]]  
**Documentation:** [[04_Implementation/Performance_Guidelines]]

---

### Phase
A stage of development in the project roadmap.

**Phases:**
1. Foundation
2. Combat Core
3. Content & Polish
4. Multiplayer
5. Beta & Launch

**See also:** [[Roadmap]], [[Milestone]]  
**Documentation:** [[01_Architecture/Project_Roadmap]]

---

### Positioning
The tactical placement of characters on the grid. Good positioning provides advantages (high ground, flanking, cover).

**See also:** [[Tactics]], [[Grid]]

---

### Prefix
The identifier at the start of an asset or class name indicating its type.

**Examples:**
- A = Actor
- U = UObject
- F = Struct
- E = Enum
- SM_ = Static Mesh
- T_ = Texture

**See also:** [[Naming Convention]]  
**Documentation:** [[02_Conventions/Naming_Conventions]]

---

### Proficiency
D&D 5e mechanic indicating trained skill in something. Adds proficiency bonus to related rolls.

**See also:** [[Proficiency Bonus]], [[D&D 5e]]

---

### Proficiency Bonus
In D&D 5e, a bonus added to rolls for things you're proficient in. Increases with level.

**Formula:** +2 at level 1-4, +3 at 5-8, +4 at 9-12, etc.  
**See also:** [[Proficiency]], [[Level]]

---

## R

### Range
The maximum distance an ability can reach, measured in tiles (Manhattan distance in Kalki).

**Example:** Fireball spell has range 5 (can target up to 5 tiles away)

**See also:** [[Ability]], [[Manhattan Distance]]

---

### Ranged
Long-distance combat, allowing attacks from multiple tiles away.

**Opposite:** [[Melee]]  
**See also:** [[Attack]], [[Ability]]

---

### Replication
In Unreal multiplayer, the automatic synchronization of game state from server to clients.

**See also:** [[Authority]], [[Server]], [[Client]]  
**Documentation:** [[04_Implementation/Multiplayer_Guidelines]]

---

### Repository
Git storage location for the project code and assets.

**Location:** GitHub (typically)  
**See also:** [[Git]], [[Version Control]]  
**Documentation:** [[02_Conventions/Version_Control_Guidelines]]

---

### RPC (Remote Procedure Call)
Function call that executes on a different machine (server or client) in multiplayer.

**Types:**
- Server RPC (client → server)
- Client RPC (server → client)
- Multicast RPC (server → all clients)

**See also:** [[Replication]], [[Multiplayer]]  
**Documentation:** [[04_Implementation/Multiplayer_Guidelines]]

---

## S

### Saving Throw
D&D 5e mechanic where a character rolls to resist an effect (spell, poison, trap, etc.).

**Formula:** D20 + ability modifier + proficiency (if proficient)  
**See also:** [[D20 Roll]], [[D&D 5e]]

---

### Selection
The visual feedback showing which tile is currently selected (clicked).

**See also:** [[Hover]], [[Highlighting]]  
**Documentation:** [[03_Systems/Grid_Visualizer]]

---

### Server
In multiplayer, the authoritative game instance that controls game state. Clients connect to the server.

**See also:** [[Client]], [[Authority]], [[Replication]]

---

### Session
A period of gameplay from start to finish. For combat logging, a single combat encounter.

**See also:** [[Combat Log]], [[Logging]]

---

### Severity
The importance level of a log message.

**Levels:**
- Verbose (detailed debug)
- Log (normal info)
- Warning (potential issue)
- Error (critical problem)

**See also:** [[KalkiLog]], [[Channel]]  
**Documentation:** [[05_API_Reference/KalkiLog_API]]

---

### Skeletal Mesh
3D character model with bones/skeleton for animation.

**Prefix:** SK_ (e.g., SK_Character_Warrior)  
**See also:** [[Mesh]], [[Animation]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Spatial Query
Looking up information based on position (neighbors, tiles in range, etc.).

**See also:** [[Grid]], [[Pathfinding]]

---

### Static Mesh
3D model without bones (environment, props, non-animated objects).

**Prefix:** SM_ (e.g., SM_Barrel)  
**See also:** [[Mesh]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Status Effect
A temporary condition applied to a character that modifies their stats or capabilities.

**Examples:** Poisoned, Buffed, Stunned, Hasted

**See also:** [[Condition]], [[Buff]], [[Debuff]]

---

### Subsystem
Unreal's framework for creating persistent manager objects.

**Types:**
- World Subsystem (per-level)
- Game Instance Subsystem (persistent)

**Example:** UKalkiGridManager (World Subsystem)

**See also:** [[Singleton]]  
**Documentation:** [[04_Implementation/Class_Templates]]

---

## T

### Tactics
Strategic positioning and decision-making in combat.

**Key elements:**
- High ground advantage
- Flanking
- Focus fire
- Zoning

**See also:** [[Positioning]], [[Combat]]

---

### Targeting
The process of selecting which tile/character an ability will affect.

**Types:**
- Single target
- Self
- AOE
- Line
- Cone

**See also:** [[Ability]], [[AOE]]

---

### Texture
2D image file used in materials to define surface appearance.

**Prefix:** T_ (e.g., T_Character_Diffuse)  
**See also:** [[Material]], [[Asset]]  
**Documentation:** [[02_Conventions/Asset_Guidelines]]

---

### Tile
A single square on the grid. Contains properties like elevation, walkability, and occupant.

**Type:** FKalkiGridTile  
**See also:** [[Grid]], [[Coordinate]]  
**Documentation:** [[03_Systems/Grid_System]]

---

### Turn
A character's opportunity to act in combat. During their turn, they can move and use abilities.

**See also:** [[Turn Order]], [[Action Points]], [[Initiative]]

---

### Turn Order
The sequence in which characters take their turns, determined by initiative.

**See also:** [[Initiative]], [[Turn System]]

---

### Turn System
The game system that manages turn-based combat flow.

**See also:** [[Turn]], [[Turn Order]], [[Initiative]]

---

## U

### UI (User Interface)
The visual elements that display information to the player and accept input.

**Examples:** Health bars, ability icons, combat log, menus

**See also:** [[Widget]], [[HUD]], [[MVVM]]

---

### Unreal Engine
The game engine Kalki is built on.

**Version:** 5.3+  
**Language:** C++ (core systems) + Blueprint (UI/content)

---

## V

### Validation
Checking that data or input is correct/safe before using it.

**Examples:**
- Coordinate validation (is it in bounds?)
- Move validation (is tile walkable?)
- RPC validation (is input legitimate?)

**See also:** [[Error Handling]]  
**Documentation:** [[04_Implementation/Error_Handling]]

---

### VFX (Visual Effects)
Particle effects and visual feedback (explosions, spell effects, hit impacts).

**Prefix:** PS_ (Particle System) or NS_ (Niagara System)  
**See also:** [[Particle System]]

---

### ViewModel
In MVVM, the class that contains UI data and logic, separate from the visual widget.

**Prefix:** UKalkiViewModel (base class)  
**Example:** UKalkiCombatLogViewModel

**See also:** [[MVVM]], [[Widget]]  
**Documentation:** [[03_Systems/MVVM_Pattern]]

---

### Vision
How far a character can see, used for fog of war and targeting.

**See also:** [[Line of Sight]], [[Fog of War]]

---

## W

### Walkable
Property of a tile indicating whether characters can move through it.

**bWalkable:** Boolean in FKalkiGridTile  
**See also:** [[Tile]], [[Movement]]

---

### Widget
Unreal's UI element. The visual component in MVVM.

**Prefix:** UKalkiWidget (base class)  
**Blueprint Prefix:** WBP_ (e.g., WBP_HealthBar)

**See also:** [[MVVM]], [[ViewModel]], [[UI]]  
**Documentation:** [[03_Systems/MVVM_Pattern]]

---

### World Space
3D coordinate system using X, Y, Z in world units (cm in Unreal).

**Contrast:** [[Grid Space]] (2D coordinate system using tile indices)  
**See also:** [[Coordinate Conversion]]

---

## X

### XP (Experience Points)
Points earned through combat and quests, used for leveling up.

**See also:** [[Level]], [[Progression]]

---

## Acronyms

Quick reference for common acronyms:

| Acronym | Full Term | Category |
|---------|-----------|----------|
| AC | Armor Class | D&D |
| AI | Artificial Intelligence | System |
| AOE | Area of Effect | Combat |
| AP | Action Points | Combat |
| API | Application Programming Interface | Technical |
| BP | Blueprint | Unreal |
| CI/CD | Continuous Integration/Deployment | Development |
| CPU | Central Processing Unit | Performance |
| D&D | Dungeons & Dragons | Game |
| DC | Difficulty Class | D&D |
| FPS | Frames Per Second | Performance |
| GDD | Game Design Document | Documentation |
| GPU | Graphics Processing Unit | Performance |
| HP | Hit Points | Combat |
| HUD | Heads-Up Display | UI |
| LFS | Large File Storage | Git |
| LOD | Level of Detail | Graphics |
| LOS | Line of Sight | Combat |
| MVVM | Model-View-ViewModel | Architecture |
| NPC | Non-Player Character | Game |
| ORM | Occlusion-Roughness-Metallic | Graphics |
| PIE | Play In Editor | Development |
| RAM | Random Access Memory | Performance |
| RPC | Remote Procedure Call | Networking |
| UI | User Interface | Interface |
| UMG | Unreal Motion Graphics | UI |
| VFX | Visual Effects | Graphics |
| VRAM | Video RAM | Performance |
| XP | Experience Points | Progression |

---

## D&D 5e Terms

Quick reference for D&D-specific terms:

| Term | Definition |
|------|------------|
| Ability Score | One of six core stats (STR, DEX, CON, INT, WIS, CHA) |
| Advantage | Roll 2d20, take higher |
| Armor Class (AC) | Defense rating, target for attack rolls |
| Critical Hit | Natural 20 on attack roll, extra damage |
| Critical Miss | Natural 1 on attack roll, automatic miss |
| d20 | Twenty-sided die, core of D&D mechanics |
| Disadvantage | Roll 2d20, take lower |
| Initiative | Roll to determine turn order (d20 + DEX mod) |
| Modifier | Bonus/penalty from ability score |
| Natural 20/1 | Rolling exactly 20 or 1 on d20 (before modifiers) |
| Proficiency | Trained skill, adds proficiency bonus |
| Proficiency Bonus | Bonus for proficient skills (scales with level) |
| Saving Throw | Roll to resist effect (d20 + ability mod) |
| Spell Slot | Resource for casting spells |

---

## Related Documents

### Reference
- [[02_Conventions/Naming_Conventions]] - Naming standards
- [[02_Conventions/Coding_Standards]] - Code conventions
- [[03_Systems/All]] - System documentation
- [[05_API_Reference/All]] - API documentation

### Architecture
- [[01_Architecture/Project_Overview]] - Project context
- [[01_Architecture/Project_Roadmap]] - Development plan

---

## Document Info
**File Path:** `Kalki_GDD/00_Overview/Glossary.md`  
**Last Updated:** 2025-01-09  
**Status:** Living Document (Updated as needed)  
**Phase:** Foundation  
**Complexity:** Beginner  
**Note:** Comprehensive terminology reference for the Kalki project