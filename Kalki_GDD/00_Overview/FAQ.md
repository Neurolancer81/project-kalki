---
tags:
  - kalki/faq
  - kalki/questions
  - kalki/troubleshooting
status: living-document
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Frequently Asked Questions (FAQ)

## Overview
This document answers common questions about the Kalki project, its development, design decisions, and technical implementation. It serves as a quick reference for both team members and external contributors.

---

## General Questions

### What is Kalki?

Kalki is a turn-based tactical RPG inspired by D&D 5e, featuring grid-based combat, character abilities, and strategic positioning. Players control characters on a tactical grid, using D&D-style mechanics (d20 rolls, ability scores, etc.) to defeat enemies.

**Key Features:**
- Turn-based tactical combat
- D&D 5e rules implementation
- Grid-based movement and positioning
- Character classes and abilities
- Multiplayer support

**See also:** [[01_Architecture/Project_Overview]]

---

### Why D&D 5e?

**Reasons:**
1. **Proven system** - 50+ years of refinement
2. **Player familiarity** - Many players know D&D
3. **Balanced mechanics** - Well-tested rules
4. **Depth** - Complex without being overwhelming
5. **Flexibility** - Easy to adapt to video games

D&D 5e provides a solid foundation while allowing us to focus on implementation rather than designing rules from scratch.

---

### What platforms will Kalki support?

**Initial Release:**
- Windows PC (primary)

**Future Considerations:**
- Mac
- Linux
- Consoles (Xbox, PlayStation, Switch)
- Mobile (stretch goal)

PC is the initial focus to establish the game, with ports considered based on success.

---

### Is Kalki single-player or multiplayer?

**Both!**

**Single-Player:**
- Campaign missions
- AI opponents
- Practice mode
- Solo progression

**Multiplayer:**
- 2v2 and 4v4 matches
- Ranked and casual modes
- Online matchmaking
- Spectator mode

Single-player comes first (Phases 1-3), multiplayer in Phase 4.

**See also:** [[01_Architecture/Project_Roadmap]]

---

### When will Kalki be released?

**Target Timeline:**
- Phase 1 (Foundation): March 2025
- Phase 2 (Combat): June 2025
- Phase 3 (Content): September 2025
- Phase 4 (Multiplayer): November 2025
- Phase 5 (Launch): January 2026

**Total:** ~12 months from start to launch

**Note:** These are estimates and subject to change based on development progress.

**See also:** [[01_Architecture/Project_Roadmap]]

---

### How can I contribute to Kalki?

**As a Developer:**
1. Review documentation
2. Follow coding standards
3. Submit pull requests
4. Write tests for your code

**As a Designer:**
1. Review design documents
2. Provide feedback
3. Create content (levels, characters)
4. Balance testing

**As a Tester:**
1. Report bugs with clear repro steps
2. Provide gameplay feedback
3. Test multiplayer
4. Suggest improvements

**See also:** [[02_Conventions/Version_Control_Guidelines]]

---

## Development Questions

### What engine is Kalki built on?

**Unreal Engine 5.3+**

**Why Unreal?**
- Industry-standard
- Excellent performance
- Great networking
- Blueprint + C++
- Active community
- Free for small projects

**Language:** C++ for core systems, Blueprint for UI/content

---

### Why C++ instead of Blueprint?

**C++ for Core Systems:**
- Better performance
- Type safety
- Easier debugging
- Version control friendly
- Clearer architecture
- More maintainable

**Blueprint for:**
- UI (UMG widgets)
- Level design
- Quick prototyping
- Designer-friendly content

**Approach:** C++ backend, Blueprint frontend

**See also:** [[02_Conventions/Coding_Standards]]

---

### What IDE should I use?

**Recommended:**
- **Visual Studio 2022** (Windows primary)
- **Rider** (great alternative, better Unreal support)

**Also Works:**
- Visual Studio Code (lightweight)
- CLion (JetBrains, paid)

**Note:** Visual Studio is free (Community Edition) and has the best Unreal integration on Windows.

---

### How do I set up the project?

**Prerequisites:**
1. Unreal Engine 5.3+ installed
2. Visual Studio 2022 with C++ workload
3. Git + Git LFS installed
4. 50GB+ free disk space

**Setup Steps:**
1. Clone repository: `git clone [repo-url]`
2. Run `Setup.bat` (generates project files)
3. Open `Kalki.uproject`
4. Compile in Visual Studio
5. Press Play in editor

**See also:** [[01_Architecture/Project_Overview]]

---

### Why is the repository so large?

**Size Breakdown:**
- Engine (40GB)
- Content/Assets (5-10GB)
- Binaries (2-3GB)
- Intermediate files (2-3GB)

**Solutions:**
- Git LFS handles large files
- `.gitignore` excludes generated files
- Download only what you need
- Use partial clone if available

**See also:** [[02_Conventions/Version_Control_Guidelines]]

---

### How do I run tests?

**In Editor:**
1. Window → Test Automation
2. Select tests to run
3. Click "Start Tests"

**Command Line:**
```bash
UnrealEditor.exe Kalki.uproject -ExecCmds="Automation RunTests Kalki; Quit"
```

**Console:**
```
Automation RunTests Kalki          // Run all tests
Automation RunTests Kalki.Grid     // Run grid tests
Automation List                    // List available tests
```

**See also:** [[04_Implementation/Testing_Procedures]]

---

## Design Questions

### Why grid-based instead of free movement?

**Advantages of Grid:**
1. **Tactical depth** - Clear positioning matters
2. **Turn-based clarity** - Easy to understand options
3. **Strategic planning** - Can think ahead
4. **D&D authenticity** - Matches tabletop experience
5. **Performance** - Easier to optimize than free movement
6. **Accessibility** - Lower skill floor

Grid-based movement fits our turn-based tactical design better than real-time free movement.

---

### Why square grid instead of hexagonal?

**Square Grid Benefits:**
1. **Simpler implementation** - Easier coordinates (X, Y)
2. **Familiar** - Most players understand squares
3. **Art production** - Easier to create assets
4. **Performance** - Simpler calculations
5. **D&D standard** - Most D&D uses squares

**Hexagonal Consideration:**
- Better for equal-distance neighbors
- More complex to implement
- May revisit in future

**Decision:** Square grid for v1, hex grid possible in future version.

**See also:** [[03_Systems/Grid_System]]

---

### How many character classes will there be?

**Launch (v1.0):**
- Fighter
- Wizard
- Rogue
- Cleric

**Post-Launch:**
- Barbarian
- Ranger
- Additional classes based on feedback

**Reasoning:** 
- 4 classes covers main archetypes (tank, DPS, caster, support)
- More classes added as updates
- Quality over quantity initially

**See also:** [[01_Architecture/Project_Roadmap]]

---

### Will there be character customization?

**Yes!**

**Visual Customization:**
- Color schemes
- Armor variations
- Cosmetic items (post-launch)

**Gameplay Customization:**
- Ability selection (choose from pool)
- Stat allocation
- Equipment choices
- Build variety

**Goal:** Let players express themselves while maintaining class identity.

---

### How long will combat encounters be?

**Target Duration:**
- Tutorial: 3-5 minutes
- Standard encounter: 5-10 minutes
- Boss fight: 10-15 minutes

**Design Goals:**
- Quick enough for mobile play sessions
- Long enough for tactical depth
- Respectful of player time
- Replayable without fatigue

---

### Will there be a story/campaign?

**V1.0 Launch:**
- Arena-based encounters
- Basic mission structure
- Minimal story (focus on combat)

**Post-Launch (Season 1+):**
- Campaign mode
- Story missions
- Character progression
- Narrative content

**Reasoning:** 
- Core combat first
- Story requires significant resources
- Can add story based on player interest

**See also:** [[01_Architecture/Project_Roadmap]]

---

## Technical Questions

### Why MVVM for UI?

**Benefits:**
1. **Separation of concerns** - Logic separate from visuals
2. **Testability** - Can test ViewModels without UI
3. **Reusability** - ViewModels work with multiple widgets
4. **Maintainability** - Easier to modify and debug
5. **Team workflow** - Programmers and designers can work separately

MVVM is more complex initially but pays off in larger projects.

**See also:** [[03_Systems/MVVM_Pattern]]

---

### Why use subsystems instead of singletons?

**Subsystems Advantages:**
1. **Built-in lifecycle** - Unreal manages creation/destruction
2. **Type-safe** - No casting needed
3. **Multiple instances** - Can have per-world managers
4. **Network support** - Replication-aware
5. **Editor support** - Shows in editor, debuggable

Subsystems are Unreal's recommended pattern over manual singletons.

**See also:** [[04_Implementation/Common_Patterns]]

---

### How is networking handled?

**Architecture:**
- Client-Server model
- Server has authority
- Clients are simulated proxies
- Replication for state sync
- RPCs for events

**Approach:**
- Server validates all actions
- Clients predict locally
- Server corrects if needed
- Minimize bandwidth

**See also:** [[04_Implementation/Multiplayer_Guidelines]]

---

### What about cheating/anti-cheat?

**Strategy:**

**Server-Side Validation:**
- All gameplay on server
- Clients send inputs only
- Server validates legality
- Impossible to hack gameplay

**Client-Side Checks:**
- Detect modified files
- Check for common cheats
- Report suspicious behavior

**Additional:**
- Player reporting system
- Automated ban system
- Regular monitoring

**Philosophy:** Trust server, not clients.

---

### How is performance optimized?

**Key Optimizations:**

**CPU:**
- Minimal tick actors
- Object pooling
- Cached subsystems
- Efficient algorithms
- Staggered updates

**GPU:**
- Instanced meshes
- LODs (3-4 levels)
- Simple materials (< 200 instructions)
- Texture streaming
- Draw call reduction

**Memory:**
- Lazy loading
- Asset unloading
- Weak pointers
- Careful replication

**Target:** 60 FPS @ 1080p on mid-range PC

**See also:** [[04_Implementation/Performance_Guidelines]]

---

### Why use Git LFS?

**Problem:** 
- Git doesn't handle large files well
- Binary files cause repo bloat
- Cloning becomes very slow

**Solution (Git LFS):**
- Stores large files separately
- Only downloads when needed
- Repository stays small
- Fast cloning

**What uses LFS:**
- Textures (.png, .jpg, .tga)
- Meshes (.fbx, .uasset)
- Audio (.wav, .mp3)
- Unreal assets (.umap, .uasset)

**See also:** [[02_Conventions/Version_Control_Guidelines]]

---

## Gameplay Questions

### How does turn order work?

**Initiative System (D&D 5e):**
1. Each character rolls initiative at combat start
2. Roll: d20 + Dexterity modifier
3. Characters act in order from highest to lowest
4. Ties broken by Dexterity score, then spawn order
5. Order stays fixed for entire combat

**Turn Structure:**
- Character's turn begins
- Spend Action Points (move, attack, abilities)
- End turn (voluntary or AP exhausted)
- Next character's turn

**See also:** [[00_Overview/Glossary]] (Initiative)

---

### How do action points work?

**System:**
- Each character gets Action Points (AP) per turn
- Different actions cost different AP
- Turn ends when AP exhausted or player chooses to end

**Example AP Costs:**
- Move 1 tile: 1 AP
- Basic Attack: 1 AP
- Special Ability: 2 AP
- Ultimate Ability: 3 AP

**Typical AP per turn:** 3-4

**Design:** Allows interesting choices (multiple weak actions vs one strong action)

---

### Can you move after attacking?

**Yes!** (if you have AP remaining)

**Example Turn:**
- Start with 3 AP
- Move 1 tile (1 AP remaining)
- Attack enemy (0 AP remaining)
- Turn ends

**OR:**
- Attack enemy (2 AP remaining)
- Move 2 tiles (0 AP remaining)
- Turn ends

**Flexibility:** Spend AP in any order, unlike strict "move then action" systems.

---

### How does advantage/disadvantage work?

**D&D 5e Mechanic:**

**Advantage:**
- Roll 2d20, take higher result
- Granted by favorable conditions (high ground, flanking, buffs)
- Cannot stack (multiple advantages = just advantage)

**Disadvantage:**
- Roll 2d20, take lower result
- Applied by unfavorable conditions (debuffs, difficult terrain)
- Cannot stack (multiple disadvantages = just disadvantage)

**Advantage vs Disadvantage:**
- Cancel each other out
- Becomes normal roll (1d20)

**See also:** [[00_Overview/Glossary]] (Advantage, Disadvantage)

---

### What happens when a character dies?

**In Combat:**
- Character becomes inactive
- Cannot be revived during combat
- Drops out of turn order
- Counts as defeat if all characters die

**After Combat:**
- Victory: Characters auto-revive at 1 HP
- Defeat: Return to menu, try again

**Future Consideration:**
- Permadeath mode (hardcore)
- Revival abilities
- Death penalties

---

### Is there friendly fire?

**Design Decision: No friendly fire**

**Reasoning:**
1. **Frustrating** - Punishes positioning too harshly
2. **Confusing** - Hard to track in turn-based
3. **Slows gameplay** - Players overly cautious
4. **Not fun** - Feels like punishment

**Alternative:** 
- Some abilities may have team benefits (heal allies in AOE)
- Encourages grouping instead of spreading

**Exception:** May add as optional difficulty modifier

---

## Troubleshooting

### The game won't compile

**Common Causes:**

**1. Missing Engine Version:**
- Install Unreal Engine 5.3+
- Match exact version in .uproject

**2. Missing Visual Studio:**
- Install VS 2022
- Include C++ game development workload
- Restart after install

**3. Outdated Project Files:**
- Delete `.vs`, `Binaries`, `Intermediate` folders
- Right-click .uproject → "Generate Visual Studio Files"
- Try compiling again

**4. Git LFS Issues:**
- Ensure Git LFS installed: `git lfs install`
- Pull LFS files: `git lfs pull`

**See also:** [[04_Implementation/Testing_Procedures]]

---

### The grid isn't appearing

**Checklist:**

**1. Grid Created?**
- Check GridManager initialized
- Call `CreateGrid()` explicitly
- Verify `IsGridCreated()` returns true

**2. Visualizer Spawned?**
- Check AKalkiGridVisualizer in level
- Verify `CreateGridVisuals()` called
- Check visualizer bound to grid events

**3. Visual Settings?**
- Check mesh assigned
- Check material assigned
- Verify not hidden
- Check camera can see grid

**4. Console Errors?**
- Open Output Log
- Look for grid-related errors
- Check KalkiLog messages

**See also:** [[03_Systems/Grid_Visualizer]]

---

### Characters aren't replicating in multiplayer

**Checklist:**

**1. Replication Enabled?**
- Check `bReplicates = true`
- Check `SetReplicateMovement(true)`

**2. Properties Replicated?**
- Add `Replicated` UPROPERTY specifier
- Implement `GetLifetimeReplicatedProps()`

**3. Authority?**
- Only server can spawn/modify
- Check `HasAuthority()` before modifications

**4. Network Settings?**
- Check NetUpdateFrequency
- Verify dedicated server vs listen server

**See also:** [[04_Implementation/Multiplayer_Guidelines]]

---

### Performance is poor (low FPS)

**Quick Fixes:**

**1. Check Profiling:**
```
stat fps        // Show frame rate
stat unit       // Show bottleneck
stat game       // Game thread
stat gpu        // GPU time
```

**2. Common Issues:**
- Too many ticking actors (disable tick)
- Complex materials (reduce instructions)
- Missing LODs (add LOD levels)
- Large textures (reduce resolution)
- No instanced meshes (use instancing)

**3. Reduce Quality:**
```
sg.ViewDistanceQuality 1
sg.ShadowQuality 0
sg.PostProcessQuality 0
r.ScreenPercentage 70
```

**See also:** [[04_Implementation/Performance_Guidelines]]

---

### Git merge conflicts

**Resolution Steps:**

**1. Identify Conflicts:**
```bash
git status
# Shows conflicted files
```

**2. Open Conflicted File:**
- Look for conflict markers: `<<<<<<<`, `=======`, `>>>>>>>`
- Decide what to keep (yours, theirs, or combination)

**3. Resolve:**
- Edit file to desired state
- Remove conflict markers
- Save file

**4. Mark Resolved:**
```bash
git add ConflictedFile.cpp
git commit -m "merge: resolve conflicts"
```

**Prevention:**
- Pull frequently
- Small, focused commits
- Communicate with team

**See also:** [[02_Conventions/Version_Control_Guidelines]]

---

## Best Practices

### What's the best way to learn the codebase?

**Recommended Path:**

**1. Read Documentation:**
- Start with [[01_Architecture/Project_Overview]]
- Read [[02_Conventions/All]] for standards
- Review [[03_Systems/Grid_System]] as example

**2. Run the Project:**
- Compile and run
- Test features in editor
- Read console logs

**3. Study Examples:**
- Look at KalkiGridManager implementation
- Examine MVVM examples
- Review test cases

**4. Start Small:**
- Fix documentation typos
- Add comments
- Write tests
- Small bug fixes

**5. Ask Questions:**
- Check this FAQ first
- Ask team members
- Read related docs

---

### How should I structure my commit messages?

**Format:**
```
<type>(<scope>): <subject>

[optional body]

[optional footer]
```

**Good Examples:**
```
feat(grid): add elevation visualization
fix(combat): resolve turn order bug
docs(api): update GridManager reference
test(grid): add coordinate validation tests
```

**Bad Examples:**
```
fix stuff
update
wip
asdf
```

**See also:** [[02_Conventions/Version_Control_Guidelines]]

---

### When should I create a feature branch?

**Create Feature Branch When:**
- Adding new feature (feature/feature-name)
- Fixing bug (bugfix/bug-description)
- Refactoring (refactor/what-refactoring)
- Documentation only (docs/what-docs)

**Don't Create Branch For:**
- Tiny typo fixes (commit directly to develop)
- Emergency hotfixes (use hotfix branch)

**Branch Naming:**
```
feature/grid-elevation-system   ✅
bugfix/character-movement       ✅
docs/update-api-docs           ✅
johns-branch                   ❌
test                          ❌
wip                           ❌
```

**See also:** [[02_Conventions/Version_Control_Guidelines]]

---

## Future Plans

### Will there be mod support?

**Not at Launch, But:**
- May add in post-launch updates
- Depends on community interest
- Requires significant work
- Would love to support eventually

**What Mods Could Include:**
- Custom characters
- Custom abilities
- Custom maps
- Balance changes
- New game modes

**Timeline:** Post-launch, if successful

---

### Will there be a level editor?

**Planned (Post-Launch):**
- In-game map editor
- Share custom maps
- Community maps
- Workshop integration (Steam)

**Not Launch Priority:**
- Combat system first
- Content second
- Tools third

**See also:** [[01_Architecture/Project_Roadmap]]

---

### Will there be voice acting?

**V1.0:** No voice acting

**Reasoning:**
- Expensive
- Time-consuming
- Not critical for tactical combat
- Text is sufficient initially

**Future:**
- May add for story content
- Depends on success and budget
- Community voiceover packs possible

---

### Any plans for esports/tournaments?

**Long-Term Goal:**
- Tournament mode
- Spectator improvements
- Replay system
- Leaderboards
- Organized play support

**Requirements:**
- Game must be successful
- Balanced gameplay
- Active community
- Stable infrastructure

**Timeline:** 6+ months post-launch, if demand exists

---

## Contact & Support

### Where can I get help?

**During Development:**
- Check this FAQ first
- Review relevant documentation
- Ask team members
- Create GitHub issue (for bugs)

**Post-Launch:**
- Official Discord server
- Steam community forums
- Support email
- Bug tracker

---

### How do I report a bug?

**Information Needed:**
1. **Description** - What happened?
2. **Steps to Reproduce** - How to make it happen again?
3. **Expected Behavior** - What should happen?
4. **Actual Behavior** - What actually happens?
5. **Logs** - Any error messages?
6. **System Info** - OS, GPU, etc.

**Where to Report:**
- GitHub Issues (development)
- Bug tracker (post-launch)
- Discord #bug-reports (community)

**See also:** [[04_Implementation/Testing_Procedures]]

---

### Where can I suggest features?

**Development Phase:**
- Team discussion
- Design review meetings
- Documentation updates

**Post-Launch:**
- Discord #suggestions
- Community forums
- Surveys
- Reddit feedback threads

**Note:** Not all suggestions can be implemented, but all are considered.

---

## Related Documents

### Getting Started
- [[01_Architecture/Project_Overview]] - Project introduction
- [[01_Architecture/Project_Roadmap]] - Development timeline
- [[00_Overview/Glossary]] - Terminology reference

### Development
- [[02_Conventions/All]] - Coding standards
- [[04_Implementation/All]] - Implementation guides
- [[04_Implementation/Testing_Procedures]] - Testing guide

### Systems
- [[03_Systems/All]] - System documentation
- [[05_API_Reference/All]] - API references

---

## Document Info
**File Path:** `Kalki_GDD/00_Overview/FAQ.md`  
**Last Updated:** 2025-01-09  
**Status:** Living Document (Updated regularly)  
**Phase:** Foundation  
**Complexity:** Beginner  
**Note:** Answers to common questions about Kalki development and design