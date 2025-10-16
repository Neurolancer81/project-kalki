---
tags:
  - kalki/claude
  - kalki/workflow
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
---

# Context Guidelines - How to Work With Claude

## Overview
This document defines how to effectively communicate with Claude (AI assistant) when working on the Kalki project. Following these guidelines ensures consistent, high-quality code generation and maintains project standards across chat sessions.

---

## Starting a New Chat Session

### Essential Context Documents

**Always provide these documents at the start of a new chat:**

1. **[[00_Overview/Current_Status]]** - What's complete, what's next
2. **[[02_Conventions/Naming_Conventions]]** - How to name things
3. **[[02_Conventions/Coding_Standards]]** - How to write code
4. **This document** - How to respond

### Additional Context Based on Work

**For Grid/Pathfinding Work:**
- [[03_Systems/Grid_System]]
- [[05_API_Reference/KalkiGridManager_API]]

**For UI/Widget Work:**
- [[03_Systems/MVVM_Pattern]]
- [[03_Systems/Combat_Log_System]]

**For Character/Combat Work:**
- [[07_Roadmap/Phase_4_Character_Movement]]
- [[04_Implementation/Multiplayer_Guidelines]]

---

## Session Initialization Template

**Use this template when starting a new chat:**
```
I'm working on Kalki, a D&D 5e tactical RPG in Unreal Engine 5.

Please read these documents to understand the project:
1. Current_Status.md - Project state
2. Naming_Conventions.md - How we name things
3. Coding_Standards.md - How we write code
4. Context_Guidelines.md - How to work with me

Today's task: [Describe what you want to implement]

Relevant system docs:
- [List specific system docs needed]

Follow all conventions in the docs.
Provide code one class at a time.
Wait for my approval before continuing.
```

---

## Communication Style

### How to Ask Questions

**✅ Good - Clear and Specific:**
```
I need to implement A* pathfinding for the grid system.

Requirements:
- Find shortest path between two tiles
- Respect walkability
- Account for elevation costs
- Return path as array of coordinates

Follow conventions in Grid_System.md.
Provide the pathfinding class first, one class at a time.
```

**❌ Bad - Vague:**
```
Make pathfinding work
```

---

### Providing Feedback

**✅ Good - Specific Corrections:**
```
The SetViewModel function should override the base class.
Remove the UFUNCTION macro and add the override keyword.
Parameter type should be UKalkiViewModelBase*, not UKalkiGridInfoViewModel*.
```

**❌ Bad - Unclear:**
```
That's wrong, fix it
```

---

## Code Generation Rules

### Delivery Format

**Always deliver code one class at a time:**

1. **Provide full class** (header + implementation)
2. **Wait for explicit approval** ("Yes", "Next", "Continue")
3. **Then provide next class**
4. **Never provide multiple classes without approval**

**Example Flow:**
```
Claude: [Provides KalkiPathfinding.h and .cpp]
       "Ready for the next class?"

You:   "Yes"

Claude: [Provides next class]
       "Ready for the next class?"

You:   "Yes, but change X in the previous class"

Claude: [Provides corrected previous class]
       "Changes made. Ready to continue?"
```

---

### Code Structure Requirements

**Every class must have:**

1. ✅ **Copyright header** - `// Copyright of V.S. Puranam and no one else`
2. ✅ **File path comment** - `// Public/Grid/KalkiClassName.h`
3. ✅ **Proper includes** - CoreMinimal first, Generated.h last
4. ✅ **Class documentation** - Brief description with context
5. ✅ **Section dividers** - Organize related functions
6. ✅ **Function documentation** - For public/protected functions
7. ✅ **Proper naming** - Follow Naming_Conventions.md
8. ✅ **Proper formatting** - Follow Coding_Standards.md

---

### Naming Convention Rules

**Claude must follow these prefixes:**

- `UKalki*` - UObject-derived classes
- `AKalki*` - Actor classes
- `FKalki*` - Structs
- `EKalki*` - Enums
- `IKalki*` - Interfaces

**File naming:**
- Headers: `KalkiClassName.h`
- Implementation: `KalkiClassName.cpp`
- Blueprints: `BP_ClassName`
- Widgets: `WBP_WidgetName`

**See [[02_Conventions/Naming_Conventions]] for complete rules.**

---

### Code Quality Standards

**Every function must:**
- ✅ Check for null pointers before use
- ✅ Validate inputs (coordinates, ranges, etc.)
- ✅ Log errors with KalkiLog
- ✅ Check authority for multiplayer modifications
- ✅ Return early on validation failure
- ✅ Use const correctness
- ✅ Have clear, descriptive names

**Example:**
```cpp
void UKalkiGridManager::SetElevation(const FKalkiGridCoord& Coord, float Elevation)
{
    // Check authority
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("SetElevation - No authority"), EKalkiLogSeverity::Warning);
        return;
    }
    
    // Validate coordinate
    if (!IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetElevation - Invalid coord: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Error
        );
        return;
    }
    
    // Main logic
    FKalkiGridTile* Tile = GridTiles.Find(Coord);
    if (Tile)
    {
        Tile->Elevation = Elevation;
        OnTileChanged.Broadcast(Coord);
    }
}
```

---

## Responding to Requests

### Analysis Before Coding

**Before generating code, Claude should:**

1. **Understand the requirement** - Ask clarifying questions if needed
2. **Check existing systems** - Reference relevant docs
3. **Propose architecture** - High-level design if complex
4. **Wait for approval** - Get confirmation before coding
5. **Then generate code** - One class at a time

**Example:**
```
You: "I need pathfinding"

Claude: "I'll implement A* pathfinding for the grid system.

Architecture:
- UKalkiPathfinding (Object) - A* implementation
- FKalkiGridPath (Struct) - Path result with waypoints and cost
- Integration with GridManager for tile queries

This will use the existing GetNeighbors and GetTilesInRange functions.
Movement cost will include elevation changes.

Does this match your vision? Should I proceed?"

You: "Yes, but also include line-of-sight checks"

Claude: "Adding line-of-sight to the pathfinding class.
Ready to generate the code?"
```

---

### Explaining Design Decisions

**When making architectural choices, explain why:**

**✅ Good:**
```
I'm using UObject instead of AActor for the pathfinding class because:
- Pathfinding is stateless (no need for transform, tick, etc.)
- Lighter weight (no actor overhead)
- Can be used as a utility (static-like functions)
- Matches Unreal's design (UNavigationPath, etc.)
```

**❌ Bad:**
```
Here's the pathfinding class [provides code without explanation]
```

---

### Handling Ambiguity

**When requirements are unclear:**

**✅ Do this:**
```
You mentioned "movement range preview."

I can implement this two ways:

Option A: Flood-fill from origin (shows all reachable tiles)
- Respects movement points/action points
- More accurate for gameplay

Option B: Simple radius (Manhattan distance)
- Faster to calculate
- Good for visualization

Which approach do you prefer?
```

**❌ Don't do this:**
```
Here's the movement range code [assumes without asking]
```

---

## Common Patterns

### Pattern 1: New System Implementation

**Request:**
```
Implement [System Name] with [features]
```

**Claude Response:**
1. Analyze requirements
2. Check dependencies (existing systems)
3. Propose architecture (classes, structs, flow)
4. Wait for approval
5. Generate classes one at a time
6. Provide usage examples
7. Suggest test commands

---

### Pattern 2: Extending Existing System

**Request:**
```
Add [feature] to [existing system]
```

**Claude Response:**
1. Reference existing system doc
2. Explain where changes go
3. Show before/after for modified functions
4. Maintain existing patterns
5. Update related docs if needed

---

### Pattern 3: Bug Fix

**Request:**
```
Fix [problem description]
```

**Claude Response:**
1. Identify root cause
2. Explain why it happens
3. Propose solution
4. Show corrected code
5. Suggest test to verify fix

---

### Pattern 4: Refactoring

**Request:**
```
Refactor [code] to [goal]
```

**Claude Response:**
1. Analyze current implementation
2. Explain issues with current approach
3. Propose improved design
4. Show refactored code
5. Highlight improvements (performance, readability, etc.)

---

## Multiplayer Considerations

**Claude must always consider multiplayer:**

### Authority Checks
```cpp
// Always check for modifications
if (!HasAuthority())
{
    KalkiLog::System(TEXT("Modification on client - ignoring"));
    return;
}
```

### Replication
```cpp
// Mark replicated properties
UPROPERTY(Replicated)
int32 SomeValue;

// Implement GetLifetimeReplicatedProps
void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
```

### Local vs Replicated
**Claude should distinguish:**
- **Local only** - Visuals, UI, audio (no replication)
- **Replicated** - Gameplay state (health, position, etc.)

---

## Documentation Updates

**When creating new systems, Claude should remind:**
```
New system created: Pathfinding System

Consider updating these docs:
- Current_Status.md (add to Phase 3 complete)
- Add new doc: 03_Systems/Pathfinding_System.md
- Add new doc: 05_API_Reference/KalkiPathfinding_API.md
- Update: 07_Roadmap/Phase_4_Character_Movement.md (dependencies)

Would you like me to generate these documentation updates?
```

---

## Error Handling

### When Claude Makes a Mistake

**Claude should:**
1. Acknowledge the error clearly
2. Explain what was wrong
3. Provide corrected code
4. Apologize if appropriate

**Example:**
```
You're right - I made an error. The function should override the base class.

The issue:
- I added UFUNCTION macro (incorrect for overrides)
- Missing 'override' keyword (compiler safety)

Corrected version:
[provides fixed code]

Sorry for the confusion. The corrected version follows the base class signature correctly.
```

---

## Testing and Validation

**Claude should suggest test commands:**
```cpp
After implementing pathfinding, you can test with:

// Find path
FindPath 0 0 10 10

// Show path visually
ShowPath 5 5 15 15

// Test with obstacles
SetTileWalkable 7 7 false
FindPath 5 5 10 10

Expected results:
- First command: Straight line path (if no obstacles)
- Second command: Path highlights on grid
- Third command: Path routes around obstacle at (7,7)
```

---

## Performance Awareness

**Claude should flag performance concerns:**
```cpp
Note: GetTilesInRange is O(R²) where R is range.

For typical gameplay (range 5-10): ~25-100 tiles checked (fast)
For large ranges (range 20+): ~400+ tiles checked (noticeable)

Consider caching results if called frequently (e.g., every frame).
```

---

## Code Review Checklist

**Before considering a class "done", verify:**

- ✅ Copyright header present
- ✅ Proper includes (order: CoreMinimal, engine, project, generated)
- ✅ Class documentation comment
- ✅ Public functions documented
- ✅ Follows naming conventions
- ✅ Follows coding standards
- ✅ Null checks before pointer use
- ✅ Authority checks for multiplayer
- ✅ Error logging with KalkiLog
- ✅ Const correctness
- ✅ Early returns for validation
- ✅ Section dividers for organization

---

## Anti-Patterns to Avoid

### ❌ Don't Do These:

**1. Multiple classes without approval:**
```
Claude: Here's Class1, Class2, Class3, and Class4
```
**Why:** Overwhelming, hard to review, might need changes

---

**2. Assuming requirements:**
```
Claude: I assumed you wanted X, so I implemented Y
```
**Why:** Ask first, code second

---

**3. Incomplete error handling:**
```cpp
// Missing null check
GridManager->DoSomething();  // Crash if null!
```

---

**4. No logging:**
```cpp
if (error)
{
    return false;  // Silent failure
}
```

---

**5. Ignoring conventions:**
```cpp
class GridHelper { };  // Missing Kalki prefix
void myFunction() { }  // camelCase instead of PascalCase
```

---

## Advanced Topics

### When to Suggest Alternative Approaches

**Claude should suggest alternatives when:**
- Performance concern exists
- Simpler solution available
- Current approach has limitations
- Better pattern exists in Unreal

**Example:**
```
The current approach uses Tick() every frame (expensive).

Alternative: Use Timer for periodic updates
- More efficient (only runs when needed)
- Configurable interval
- Can pause/resume easily

Would you like me to refactor to use timers instead?
```

---

### Handling Technical Debt

**If implementing a "quick fix":**
```cpp
// TODO: Temporary solution - refactor when we have proper system
// Current approach doesn't handle edge case X
// See: Roadmap/Future_Systems.md - "Proper X System"
void TemporaryWorkaround()
{
    // Implementation
}
```

**And mention in response:**
```
This is a temporary solution that works for now.

Limitations:
- Doesn't handle case X
- Not optimal for Y

Future improvement:
- Implement proper system Z (see Future_Systems.md)

Is this acceptable for now, or should we implement the full system?
```

---

## Summary

### Golden Rules for Claude

1. **Read context docs** - Understand before coding
2. **Ask before assuming** - Clarify ambiguous requirements
3. **One class at a time** - Wait for approval
4. **Follow ALL conventions** - Naming, coding, multiplayer
5. **Validate everything** - Null checks, bounds checks, authority
6. **Log errors** - Use KalkiLog consistently
7. **Document code** - Classes, functions, complex logic
8. **Explain decisions** - Why this design? Why this approach?
9. **Suggest improvements** - Performance, patterns, alternatives
10. **Be humble** - Acknowledge mistakes, provide fixes

---

## Quick Reference

### Starting a New Session
```
1. Provide: Current_Status.md
2. Provide: Naming_Conventions.md
3. Provide: Coding_Standards.md
4. Provide: This document
5. Provide: Relevant system docs
6. State the task clearly
7. Wait for Claude's proposed approach
8. Approve and begin
```

### During Development
```
1. Claude proposes architecture
2. You approve/adjust
3. Claude generates one class
4. You review
5. You approve or request changes
6. Repeat until complete
```

### Finishing Up
```
1. Review all generated code
2. Test with provided commands
3. Update Current_Status.md
4. Consider new system docs
5. Note any TODOs or improvements
```

---

## Related Documents

### Essential Foundation
- [[00_Overview/Current_Status]] - Always start here
- [[02_Conventions/Naming_Conventions]] - Critical for code generation
- [[02_Conventions/Coding_Standards]] - Code structure rules

### Claude Guidelines
- [[06_How_To_Work_With_Claude/Code_Generation_Rules]] - Detailed code rules
- [[04_Implementation/Class_Templates]] - Template examples

### Project Understanding
- [[00_Overview/Project_Vision]] - What we're building
- [[01_Architecture/Project_Structure]] - How it's organized
- [[03_Systems/Grid_System]] - Example system doc

### Implementation
- [[04_Implementation/Common_Patterns]] - Recurring patterns
- [[04_Implementation/Multiplayer_Guidelines]] - Multiplayer rules
- [[04_Implementation/Testing_Procedures]] - How to test

---

## Document Info
**File Path:** `Kalki_GDD/06_How_To_Work_With_Claude/Context_Guidelines.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Purpose:** Train new Claude instances on project workflow