---
tags:
  - kalki/roadmap
  - kalki/milestones
  - kalki/architecture
status: living-document
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Project Roadmap

## Overview
This document outlines the development roadmap for the Kalki project, including planned features, milestones, and release schedule. The roadmap is organized into phases, each building upon the previous one.

---

## Purpose

### Why a Roadmap?

**Benefits of a clear roadmap:**
- ✅ Clear development direction
- ✅ Prioritized feature list
- ✅ Realistic timelines
- ✅ Team alignment
- ✅ Stakeholder communication
- ✅ Progress tracking
- ✅ Scope management

**This roadmap:**
- Defines project phases
- Lists features per phase
- Sets milestones
- Estimates timelines
- Identifies dependencies
- Tracks progress

---

## Current Status

**Project Start Date:** January 2025  
**Current Phase:** Phase 1 - Foundation  
**Current Version:** v0.1.0 (Pre-Alpha)  
**Target Release:** TBD

**Completed:**
- ✅ Project structure defined
- ✅ Documentation framework established
- ✅ Core conventions documented
- ✅ Basic grid system design
- ✅ MVVM architecture defined

**In Progress:**
- 🔄 Grid system implementation
- 🔄 Core systems documentation
- 🔄 Testing framework setup

**Next Up:**
- ⏳ Grid visualization
- ⏳ Character system foundation
- ⏳ Basic movement

---

## Development Phases

### Phase Overview
```
Phase 1: Foundation (2 months)
└── Core systems, grid, basic movement

Phase 2: Combat Core (3 months)
└── Turn-based combat, abilities, AI

Phase 3: Content & Polish (3 months)
└── Characters, enemies, levels, VFX

Phase 4: Multiplayer (2 months)
└── Network play, matchmaking

Phase 5: Beta & Launch (2 months)
└── Polish, balance, release

Total Estimated Time: 12 months
```

---

## Phase 1: Foundation

**Duration:** 2 months  
**Goal:** Establish core systems and architecture  
**Target:** Playable prototype with grid and movement

### Milestones

**Milestone 1.1: Project Setup** ✅ COMPLETE
- [x] Project structure
- [x] Documentation framework
- [x] Naming conventions
- [x] Coding standards
- [x] Version control setup
- [x] Git workflow defined

**Milestone 1.2: Grid System** 🔄 IN PROGRESS
- [x] Grid coordinate system
- [x] Grid manager architecture
- [ ] Grid creation and management
- [ ] Tile data structure
- [ ] Coordinate conversion
- [ ] Spatial queries (neighbors, range)

**Milestone 1.3: Grid Visualization** ⏳ NEXT
- [ ] Grid visualizer actor
- [ ] Instanced mesh rendering
- [ ] Tile hover highlighting
- [ ] Tile selection
- [ ] Range highlighting
- [ ] Elevation visualization

**Milestone 1.4: Character Foundation**
- [ ] Character base class
- [ ] Character stats system
- [ ] Character grid positioning
- [ ] Character movement
- [ ] Character animation basics
- [ ] Character replication

**Milestone 1.5: Basic UI**
- [ ] MVVM framework implementation
- [ ] Health bar widget
- [ ] Character info panel
- [ ] Basic HUD
- [ ] Input handling

### Features

**Core Systems:**
- Grid system (complete architecture)
- Logging system
- MVVM UI framework
- Subsystem architecture
- Event system

**Grid:**
- Variable grid sizes
- Tile properties (walkable, elevation, occupancy)
- Coordinate system
- Spatial queries
- Visual feedback (hover, selection, range)

**Characters:**
- Character placement on grid
- Basic movement (click to move)
- Character stats (HP, Max HP)
- Simple animations (idle, walk)

**UI:**
- Character health bars
- Character selection
- Movement range preview
- Basic HUD elements

**Technical:**
- Performance baseline established
- Testing framework
- Documentation complete

### Success Criteria

- [ ] Can create 20×20 grid
- [ ] Can visualize grid with elevation
- [ ] Can place characters on grid
- [ ] Can move characters with pathfinding
- [ ] Can select and view character info
- [ ] Runs at 60 FPS
- [ ] No critical bugs
- [ ] Code documented
- [ ] Tests passing

**Phase 1 Target Date:** March 2025

---

## Phase 2: Combat Core

**Duration:** 3 months  
**Goal:** Implement turn-based tactical combat  
**Target:** Playable combat encounters

### Milestones

**Milestone 2.1: Turn System**
- [ ] Turn manager
- [ ] Initiative system
- [ ] Action point system
- [ ] Turn order UI
- [ ] End turn mechanics

**Milestone 2.2: Basic Combat**
- [ ] Melee attacks
- [ ] Damage calculation
- [ ] Hit/miss mechanics
- [ ] Death and respawn
- [ ] Combat state management

**Milestone 2.3: Ability System**
- [ ] Ability base class
- [ ] Ability targeting
- [ ] Range checking
- [ ] Area of effect
- [ ] Cooldown system
- [ ] Resource costs (mana, stamina)

**Milestone 2.4: D&D 5e Rules**
- [ ] Ability scores
- [ ] Skill checks
- [ ] Saving throws
- [ ] Advantage/disadvantage
- [ ] Proficiency bonus
- [ ] Critical hits

**Milestone 2.5: Basic AI**
- [ ] AI controller
- [ ] Target selection
- [ ] Basic tactics (move, attack)
- [ ] Threat assessment
- [ ] Ability usage logic

**Milestone 2.6: Combat UI**
- [ ] Combat log (MVVM)
- [ ] Ability bar
- [ ] Target info
- [ ] Initiative tracker
- [ ] Action feedback

### Features

**Combat System:**
- Turn-based combat flow
- Action point economy
- Attack rolls and damage
- Hit reactions
- Death mechanics

**Abilities:**
- 5-10 basic abilities
- Melee attacks
- Ranged attacks
- Area of effect spells
- Healing abilities
- Buff/debuff effects

**D&D 5e Rules:**
- Six ability scores
- D20 roll system
- Armor Class (AC)
- Saving throws
- Advantage/disadvantage
- Spell slots (basic)

**AI:**
- Basic enemy AI
- Aggro system
- Simple tactics
- Ability usage
- Positioning logic

**UI/UX:**
- Combat feedback (damage numbers)
- Turn indicators
- Ability tooltips
- Combat log (detailed)
- Victory/defeat screens

**Technical:**
- Combat state replication
- Turn synchronization (multiplayer prep)
- Performance optimization

### Success Criteria

- [ ] Can start and end combat
- [ ] Turn order works correctly
- [ ] Characters can attack and use abilities
- [ ] AI enemies provide challenge
- [ ] Combat log records all actions
- [ ] D&D 5e rules implemented accurately
- [ ] 4v4 combat runs at 60 FPS
- [ ] Combat feels tactical and fun
- [ ] All features documented
- [ ] Comprehensive tests

**Phase 2 Target Date:** June 2025

---

## Phase 3: Content & Polish

**Duration:** 3 months  
**Goal:** Add content and polish gameplay  
**Target:** Feature-complete single-player experience

### Milestones

**Milestone 3.1: Character Classes**
- [ ] Fighter class
- [ ] Rogue class
- [ ] Wizard class
- [ ] Cleric class
- [ ] Class-specific abilities
- [ ] Class progression

**Milestone 3.2: Enemy Variety**
- [ ] 5+ enemy types
- [ ] Boss enemies
- [ ] Enemy abilities
- [ ] Difficulty scaling
- [ ] Encounter design

**Milestone 3.3: Level Design**
- [ ] 5+ combat arenas
- [ ] Environmental variety
- [ ] Terrain features (high ground, obstacles)
- [ ] Interactive elements
- [ ] Visual polish

**Milestone 3.4: VFX & Audio**
- [ ] Ability VFX
- [ ] Combat effects
- [ ] UI sound effects
- [ ] Background music
- [ ] Ambient audio

**Milestone 3.5: Progression System**
- [ ] Experience points
- [ ] Leveling up
- [ ] Ability unlocks
- [ ] Equipment system (basic)
- [ ] Character customization

**Milestone 3.6: Game Loop**
- [ ] Main menu
- [ ] Character selection
- [ ] Mission/encounter selection
- [ ] Victory rewards
- [ ] Game over handling

### Features

**Characters:**
- 4 playable classes
- 20+ abilities total
- Character progression (levels 1-10)
- Visual customization options
- Unique playstyles per class

**Enemies:**
- 8+ enemy types
- 2+ boss enemies
- Varied abilities and tactics
- Difficulty scaling
- Interesting encounters

**Levels:**
- 6-8 combat arenas
- Varied terrain
- Environmental hazards
- Elevation gameplay
- Visual themes (forest, dungeon, ruins)

**VFX & Audio:**
- VFX for all abilities
- Hit effects
- Damage numbers
- UI sounds
- 3+ music tracks
- Ambient sounds

**Progression:**
- XP and leveling
- Ability unlocks
- Basic equipment
- Character builds
- Replayability

**Polish:**
- Tutorial
- Tooltips
- Balance pass
- Bug fixes
- Performance optimization
- Accessibility options

### Success Criteria

- [ ] 4 playable characters
- [ ] 8+ enemy types
- [ ] 6+ unique levels
- [ ] All VFX complete
- [ ] Audio complete
- [ ] Tutorial functional
- [ ] Game loop complete
- [ ] Balanced gameplay
- [ ] 60 FPS maintained
- [ ] Professional presentation
- [ ] Beta-ready build

**Phase 3 Target Date:** September 2025

---

## Phase 4: Multiplayer

**Duration:** 2 months  
**Goal:** Implement online multiplayer  
**Target:** Competitive multiplayer matches

### Milestones

**Milestone 4.1: Network Foundation**
- [ ] Dedicated server support
- [ ] Client-server architecture
- [ ] Replication optimization
- [ ] Network profiling

**Milestone 4.2: Matchmaking**
- [ ] Lobby system
- [ ] Player matching
- [ ] Team balancing
- [ ] Ranked/casual modes

**Milestone 4.3: Online Combat**
- [ ] Synchronized combat
- [ ] Turn synchronization
- [ ] Disconnection handling
- [ ] Reconnection support
- [ ] Spectator mode

**Milestone 4.4: Multiplayer UI**
- [ ] Lobby UI
- [ ] Player list
- [ ] Team selection
- [ ] Chat system
- [ ] Scoreboard

**Milestone 4.5: Anti-Cheat**
- [ ] Server validation
- [ ] Client checks
- [ ] Reporting system
- [ ] Ban system

### Features

**Network:**
- Dedicated server
- Client-server model
- Optimized replication
- < 100ms latency support
- Bandwidth optimization

**Matchmaking:**
- Quick match
- Ranked mode
- Custom games
- Team balancing
- Skill-based matching

**Gameplay:**
- 2v2 and 4v4 modes
- Synchronized turns
- Fair combat
- Disconnect recovery
- Spectator mode

**Social:**
- Friends list
- Invites
- In-game chat
- Player profiles
- Statistics tracking

**Anti-Cheat:**
- Server-side validation
- Cheat detection
- Player reporting
- Automated bans

### Success Criteria

- [ ] Stable multiplayer
- [ ] < 100ms latency target
- [ ] No desyncs
- [ ] Matchmaking works
- [ ] Ranked mode functional
- [ ] Anti-cheat effective
- [ ] Smooth player experience
- [ ] Disconnection handled gracefully
- [ ] Comprehensive testing
- [ ] Ready for beta

**Phase 4 Target Date:** November 2025

---

## Phase 5: Beta & Launch

**Duration:** 2 months  
**Goal:** Polish and release  
**Target:** Public launch

### Milestones

**Milestone 5.1: Beta Testing**
- [ ] Closed beta (100 players)
- [ ] Bug fixing
- [ ] Balance adjustments
- [ ] Performance optimization
- [ ] Feedback integration

**Milestone 5.2: Content Expansion**
- [ ] Additional characters
- [ ] Additional maps
- [ ] Additional abilities
- [ ] Cosmetics (if time)

**Milestone 5.3: Marketing**
- [ ] Website
- [ ] Trailer
- [ ] Screenshots
- [ ] Press kit
- [ ] Social media

**Milestone 5.4: Launch Preparation**
- [ ] Launch trailer
- [ ] Store page (Steam/Epic)
- [ ] Server infrastructure
- [ ] Support systems
- [ ] Launch day plan

**Milestone 5.5: Launch**
- [ ] Day 1 launch
- [ ] Monitoring
- [ ] Hotfixes
- [ ] Community management
- [ ] Post-launch support

### Features

**Polish:**
- Final balance pass
- Bug fixes
- Performance optimization
- Visual polish
- UX improvements

**Content:**
- 6 playable characters (stretch)
- 12+ enemy types (stretch)
- 10+ maps (stretch)
- 40+ abilities (stretch)

**Meta:**
- Player statistics
- Leaderboards
- Achievements
- Daily challenges
- Seasonal content (post-launch)

**Support:**
- Patch system
- Hotfix capability
- Analytics
- Crash reporting
- Community tools

### Success Criteria

- [ ] Beta successful
- [ ] All P1 bugs fixed
- [ ] Performance targets met
- [ ] Balanced gameplay
- [ ] Marketing materials ready
- [ ] Store pages live
- [ ] Servers ready
- [ ] Launch smooth
- [ ] Positive reception
- [ ] Support systems functional

**Phase 5 Target Date:** January 2026

---

## Post-Launch Plans

### Season 1 (Month 1-3)
- **Focus:** Community building, bug fixes
- New character
- New map
- Balance patches
- Quality of life improvements
- Community events

### Season 2 (Month 4-6)
- **Focus:** New game mode
- Team deathmatch mode
- New characters (2)
- New maps (2-3)
- Ranked season 2
- Cosmetics

### Season 3 (Month 7-9)
- **Focus:** Major content update
- Campaign mode (if successful)
- New characters (2)
- New maps (3)
- Major balance patch
- Tournament support

### Long-Term
- Ongoing support based on community
- Regular content updates
- Esports potential
- Mobile port (stretch)
- Console ports (stretch)

---

## Feature Priority

### Must-Have (P1)
Essential for launch:
- Grid system
- Character movement
- Turn-based combat
- Basic abilities (10+)
- AI opponents
- 4 playable characters
- 6+ enemy types
- 6+ maps
- Multiplayer (2v2 minimum)
- Combat log
- Tutorial

### Should-Have (P2)
Important but not critical:
- 6 playable characters
- 8+ enemy types
- 8+ maps
- 4v4 multiplayer
- Ranked mode
- Progression system
- Equipment system
- More abilities (40+)
- Spectator mode
- Replay system

### Nice-to-Have (P3)
Polish and extras:
- Cosmetics
- Emotes
- Custom games
- Tournament mode
- Detailed statistics
- Achievements
- Daily challenges
- Seasonal content
- Voice chat
- Mobile companion app

---

## Risk Management

### Technical Risks

**Risk:** Network performance issues  
**Mitigation:** Early multiplayer testing, dedicated server architecture  
**Contingency:** Delay multiplayer if needed, focus on single-player

**Risk:** Performance problems with large battles  
**Mitigation:** Performance budgets, regular profiling, optimization focus  
**Contingency:** Reduce max players, simplify VFX, optimize code

**Risk:** Unreal Engine version issues  
**Mitigation:** Stay on stable version, test updates thoroughly  
**Contingency:** Delay engine updates until stable

### Content Risks

**Risk:** Not enough content variety  
**Mitigation:** Modular design, asset reuse, procedural variation  
**Contingency:** Reduce scope, focus on quality over quantity

**Risk:** Balance issues  
**Mitigation:** Regular playtesting, data-driven balance, community feedback  
**Contingency:** Post-launch patches, ongoing balance support

### Schedule Risks

**Risk:** Features take longer than estimated  
**Mitigation:** Buffer time in schedule, regular progress tracking  
**Contingency:** Cut P3 features, delay launch if needed

**Risk:** Team capacity issues  
**Mitigation:** Clear documentation, good project management  
**Contingency:** Reduce scope, extend timeline

---

## Success Metrics

### Development Metrics

**Phase 1:**
- Grid system functional
- 60 FPS maintained
- All tests passing
- Documentation complete

**Phase 2:**
- Combat system complete
- AI functional
- 10+ abilities working
- Combat feels tactical

**Phase 3:**
- 4+ characters complete
- 6+ maps done
- VFX/Audio complete
- Game loop functional

**Phase 4:**
- Multiplayer stable
- Matchmaking working
- < 100ms latency
- No critical bugs

**Phase 5:**
- Beta feedback positive
- Launch smooth
- Player retention good
- Community active

### Post-Launch Metrics

**Week 1:**
- Concurrent players
- Retention rate
- Crash rate < 1%
- Server uptime > 99%

**Month 1:**
- Active players
- Average session time
- Matchmaking times
- Player satisfaction

**Month 3:**
- Retention rate
- Community size
- Revenue (if applicable)
- Content engagement

---

## Dependencies

### External Dependencies

**Unreal Engine:**
- Version: 5.3+
- Updates: Monitor for stability
- Plugins: Minimal external plugins

**Third-Party Services:**
- Version control: Git + Git LFS
- Issue tracking: GitHub Issues
- CI/CD: GitHub Actions
- Hosting: TBD (Steam, Epic, etc.)

### Internal Dependencies

**Phase 2 depends on Phase 1:**
- Grid system must be complete
- Character movement must work
- Basic UI must exist

**Phase 3 depends on Phase 2:**
- Combat system must be stable
- Abilities must work
- AI must be functional

**Phase 4 depends on Phase 3:**
- Single-player must be complete
- Content must exist
- Balance must be reasonable

---

## Resource Allocation

### Team Roles (Current)

**Programmer (You):**
- Core systems
- Gameplay programming
- Tools development
- Technical documentation

**Designer (You):**
- Game design
- Level design
- Balance design
- Documentation

**Artist (Future/Outsource):**
- Character models
- Environment art
- VFX
- UI art

**Audio (Future/Outsource):**
- Sound effects
- Music
- Voice (if needed)

### Time Allocation

**Phase 1 (Foundation):**
- Programming: 80%
- Design: 15%
- Documentation: 5%

**Phase 2 (Combat):**
- Programming: 70%
- Design: 20%
- Documentation: 10%

**Phase 3 (Content):**
- Programming: 50%
- Design: 30%
- Content creation: 20%

**Phase 4 (Multiplayer):**
- Programming: 80%
- Testing: 15%
- Documentation: 5%

**Phase 5 (Launch):**
- Polish: 40%
- Testing: 30%
- Marketing: 20%
- Support: 10%

---

## Flexibility and Iteration

### Agile Approach

**This roadmap is flexible:**
- Review every 2 weeks
- Adjust based on learnings
- Respond to feedback
- Pivot if needed

**Iterative Development:**
- Build minimum viable features
- Test and gather feedback
- Iterate and improve
- Polish what works, cut what doesn't

**Feedback Loops:**
- Internal playtesting (weekly)
- External testing (Phase 3+)
- Community feedback (Phase 5+)
- Data-driven decisions

---

## Communication Plan

### Internal Updates

**Daily:**
- Progress tracking
- Blocker identification
- Quick syncs

**Weekly:**
- Sprint reviews
- Demo latest work
- Plan next sprint

**Monthly:**
- Milestone reviews
- Roadmap adjustments
- Big picture check

### External Updates

**Monthly (Public):**
- Development blog
- Feature highlights
- Community engagement

**Quarterly:**
- Major announcements
- Roadmap updates (public version)
- Community surveys

---

## Related Documents 
### Architecture 
- [[01_Architecture/Project_Overview]] - Project vision 
- [[02_Conventions/All]] - Standards to follow 
### Implementation 
- [[03_Systems/All]] - System designs 
- [[04_Implementation/All]] - Implementation guides  
### Reference 
- [[05_API_Reference/All]] - API documentation 
---
## Document Info 
**File Path:** `Kalki_GDD/01_Architecture/Project_Roadmap.md` 
**Last Updated:** 2025-01-09 
**Status:** Living Document (Updated Regularly)
**Phase:** Foundation 
**Complexity:** Beginner 
**Note:** High-level roadmap - subject to change based on progress and feedback