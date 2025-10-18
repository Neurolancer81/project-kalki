---
tags:
  - kalki/testing
  - kalki/qa
  - kalki/debugging
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Testing Procedures

## Overview
This document defines testing strategies, procedures, and best practices for the Kalki project. Comprehensive testing ensures code quality, prevents regressions, and maintains stability across all systems.

---

## Purpose

### Why Testing Matters

**Problems without testing:**
1. **Regressions** - New code breaks old features
2. **Hidden Bugs** - Issues not discovered until too late
3. **Multiplayer Desyncs** - Network bugs are hard to find
4. **Performance Issues** - Slow code goes unnoticed
5. **Poor User Experience** - Bugs reach players

**Benefits of good testing:**
- ✅ Catch bugs early
- ✅ Prevent regressions
- ✅ Document expected behavior
- ✅ Enable confident refactoring
- ✅ Improve code quality

---

## Testing Pyramid
```
        ┌─────────────┐
        │   Manual    │  Few, expensive, slow
        │   Testing   │  Full gameplay tests
        ├─────────────┤
        │ Integration │  Moderate, test system interactions
        │   Tests     │  Multiple systems working together
        ├─────────────┤
        │    Unit     │  Many, cheap, fast
        │   Tests     │  Test individual functions/classes
        └─────────────┘
```

**Balance:**
- **Many unit tests** - Fast, automated, test individual functions
- **Some integration tests** - Test system interactions
- **Few manual tests** - Full gameplay validation

---

## Test Types

### 1. Unit Tests

**What:** Test individual functions/classes in isolation

**When:** For logic-heavy code (math, algorithms, data processing)

**Tools:** Unreal Automation System

**Example:**
```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridCoordTest,
    "Kalki.Grid.GridCoord.ManhattanDistance",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridCoordTest::RunTest(const FString& Parameters)
{
    // Arrange
    FKalkiGridCoord A(0, 0);
    FKalkiGridCoord B(3, 4);

    // Act
    int32 Distance = UKalkiMathLibrary::ManhattanDistance(A, B);

    // Assert
    TestEqual(TEXT("Manhattan distance should be 7"), Distance, 7);

    return true;
}
```

---

### 2. Integration Tests

**What:** Test multiple systems working together

**When:** For system interactions (Grid + Pathfinding, Combat + UI)

**Tools:** Unreal Automation System, PIE

**Example:**
```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridVisualizerTest,
    "Kalki.Grid.Integration.GridAndVisualizer",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridVisualizerTest::RunTest(const FString& Parameters)
{
    // Arrange
    UWorld* World = GetTestWorld();
    UKalkiGridManager* GridManager = World->GetSubsystem<UKalkiGridManager>();
    AKalkiGridVisualizer* Visualizer = SpawnTestVisualizer(World);

    // Act
    GridManager->CreateGrid(10, 10, 100.0f, FVector::ZeroVector);

    // Assert
    TestEqual(
        TEXT("Visualizer should create instances for all tiles"),
        Visualizer->GetInstanceCount(),
        100
    );

    return true;
}
```

---

### 3. Manual Tests

**What:** Human testing of gameplay features

**When:** For feel, balance, user experience

**Tools:** PIE, packaged builds

**Procedure:**
1. Define test scenario
2. Execute test steps
3. Document results
4. Report bugs

---

## Console Commands for Testing

### Grid Testing Commands

**Create Test Grid:**
```cpp
// In KalkiCheatManager.h
UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void CreateTestGrid(int32 SizeX, int32 SizeY);

UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void SetTileElevation(int32 X, int32 Y, float Elevation);

UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void SetTileWalkable(int32 X, int32 Y, bool bWalkable);

UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void CreateTestPlatform(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float Elevation);

UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void CreateTestRamp(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float StartElev, float EndElev);

UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void PrintGridInfo();

UFUNCTION(Exec, Category = "Kalki|Grid|Testing")
void PrintTileInfo(int32 X, int32 Y);
```

**Usage:**
```
CreateTestGrid 20 20
SetTileElevation 10 10 200
CreateTestPlatform 5 5 10 10 300
CreateTestRamp 0 0 5 0 0 500
PrintTileInfo 10 10
```

---

### Character Testing Commands
```cpp
UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void SpawnTestCharacter(int32 X, int32 Y);

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void SetHP(int32 HP);

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void SetMaxHP(int32 MaxHP);

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void DealDamage(int32 Damage);

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void HealCharacter(int32 Amount);

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void TeleportToTile(int32 X, int32 Y);

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void KillCharacter();

UFUNCTION(Exec, Category = "Kalki|Character|Testing")
void GodMode();
```

---

### Combat Testing Commands
```cpp
UFUNCTION(Exec, Category = "Kalki|Combat|Testing")
void StartTestCombat();

UFUNCTION(Exec, Category = "Kalki|Combat|Testing")
void EndCombat();

UFUNCTION(Exec, Category = "Kalki|Combat|Testing")
void SpawnTestEnemy(int32 X, int32 Y);

UFUNCTION(Exec, Category = "Kalki|Combat|Testing")
void KillAllEnemies();

UFUNCTION(Exec, Category = "Kalki|Combat|Testing")
void NextTurn();

UFUNCTION(Exec, Category = "Kalki|Combat|Testing")
void ShowCombatDebug();
```

---

### Visualization Testing Commands
```cpp
UFUNCTION(Exec, Category = "Kalki|Visualization|Testing")
void ShowGridVisualizer(bool bShow);

UFUNCTION(Exec, Category = "Kalki|Visualization|Testing")
void SetHoverColor(float R, float G, float B, float A);

UFUNCTION(Exec, Category = "Kalki|Visualization|Testing")
void SetSelectedColor(float R, float G, float B, float A);

UFUNCTION(Exec, Category = "Kalki|Visualization|Testing")
void RefreshGridVisuals();

UFUNCTION(Exec, Category = "Kalki|Visualization|Testing")
void ShowElevation(bool bShow);

UFUNCTION(Exec, Category = "Kalki|Visualization|Testing")
void TestMovementRange(int32 X, int32 Y, int32 Range);
```

---

## PIE (Play In Editor) Testing

### Single Player Testing

**Setup:**
1. Click Play dropdown → Play Options
2. Number of Players: 1
3. Net Mode: Standalone

**Test scenarios:**
- Grid creation and visualization
- Character movement
- Ability casting
- UI functionality
- Combat flow

---

### Multiplayer Testing (Local)

**Setup:**
1. Click Play dropdown → Play Options
2. Number of Players: 2-4
3. Net Mode: Play As Listen Server
4. Check "Run Dedicated Server" (optional)

**Test scenarios:**

**Two Player Tests:**
```
Client 1 (Server):
1. Create grid
2. Spawn character at (5, 5)
3. Move to (10, 10)

Client 2 (Client):
1. Verify grid appears
2. Verify character appears at (5, 5)
3. Verify character moves to (10, 10)
4. Spawn own character at (15, 15)
5. Move to (10, 5)

Both Clients:
1. Verify both characters visible
2. Verify movement synchronized
3. Test simultaneous actions
4. Test turn-based combat
```

**Network Simulation:**
```
Console commands:
net PktLag=100        // 100ms latency
net PktLagVariance=20 // ±20ms variance
net PktLoss=2         // 2% packet loss
```

---

### Debug Visualizations

**Enable debug draws:**
```cpp
#if !UE_BUILD_SHIPPING
void AKalkiCharacter::DrawDebugInfo()
{
    // Draw character position
    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        50.0f,
        12,
        FColor::Green,
        false,
        0.1f
    );

    // Draw grid coordinate
    FString CoordString = FString::Printf(TEXT("(%d, %d)"), GridPosition.X, GridPosition.Y);
    DrawDebugString(
        GetWorld(),
        GetActorLocation() + FVector(0, 0, 100),
        CoordString,
        nullptr,
        FColor::White,
        0.1f
    );

    // Draw movement range
    if (bShowMovementRange)
    {
        for (const FKalkiGridCoord& Coord : ReachableTiles)
        {
            FVector TileLocation = GridManager->CoordToWorldPosition(Coord);
            DrawDebugBox(
                GetWorld(),
                TileLocation,
                FVector(50, 50, 10),
                FColor::Yellow,
                false,
                0.1f
            );
        }
    }
}
#endif
```

**Toggle debug command:**
```cpp
UFUNCTION(Exec, Category = "Kalki|Debug")
void ShowDebugInfo(bool bShow);
```

---

## Automated Test Examples

### Example: Grid Coordinate Tests
```cpp
// KalkiGridCoordTest.cpp

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridCoordEqualityTest,
    "Kalki.Grid.GridCoord.Equality",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridCoordEqualityTest::RunTest(const FString& Parameters)
{
    FKalkiGridCoord A(5, 7);
    FKalkiGridCoord B(5, 7);
    FKalkiGridCoord C(3, 2);

    TestTrue(TEXT("Equal coords should be equal"), A == B);
    TestFalse(TEXT("Different coords should not be equal"), A == C);
    TestTrue(TEXT("Different coords should be not equal"), A != C);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridCoordHashTest,
    "Kalki.Grid.GridCoord.Hash",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridCoordHashTest::RunTest(const FString& Parameters)
{
    FKalkiGridCoord A(5, 7);
    FKalkiGridCoord B(5, 7);
    FKalkiGridCoord C(7, 5);

    uint32 HashA = GetTypeHash(A);
    uint32 HashB = GetTypeHash(B);
    uint32 HashC = GetTypeHash(C);

    TestEqual(TEXT("Equal coords should have equal hashes"), HashA, HashB);
    TestNotEqual(TEXT("Different coords should have different hashes"), HashA, HashC);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridCoordMapTest,
    "Kalki.Grid.GridCoord.MapUsage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridCoordMapTest::RunTest(const FString& Parameters)
{
    TMap<FKalkiGridCoord, int32> TestMap;

    FKalkiGridCoord Coord(5, 7);
    TestMap.Add(Coord, 42);

    TestTrue(TEXT("Map should contain added coord"), TestMap.Contains(Coord));
    TestEqual(TEXT("Map should return correct value"), TestMap[Coord], 42);

    FKalkiGridCoord OtherCoord(3, 2);
    TestFalse(TEXT("Map should not contain other coord"), TestMap.Contains(OtherCoord));

    return true;
}
```

---

### Example: Grid Manager Tests
```cpp
// KalkiGridManagerTest.cpp

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridManagerCreateTest,
    "Kalki.Grid.GridManager.CreateGrid",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridManagerCreateTest::RunTest(const FString& Parameters)
{
    // Create test world
    UWorld* TestWorld = UWorld::CreateWorld(EWorldType::Game, false);
    UKalkiGridManager* GridManager = TestWorld->GetSubsystem<UKalkiGridManager>();

    // Test grid creation
    GridManager->CreateGrid(10, 10, 100.0f, FVector::ZeroVector);

    TestTrue(TEXT("Grid should be created"), GridManager->IsGridCreated());
    TestEqual(TEXT("Grid size X should match"), GridManager->GetGridSizeX(), 10);
    TestEqual(TEXT("Grid size Y should match"), GridManager->GetGridSizeY(), 10);

    // Test tile access
    FKalkiGridCoord TestCoord(5, 5);
    TestTrue(TEXT("Valid coord should be valid"), GridManager->IsValidCoord(TestCoord));

    FKalkiGridTile Tile = GridManager->GetTile(TestCoord);
    TestEqual(TEXT("Tile coordinate should match"), Tile.Coordinate, TestCoord);

    // Cleanup
    TestWorld->DestroyWorld(false);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridManagerBoundsTest,
    "Kalki.Grid.GridManager.BoundsChecking",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridManagerBoundsTest::RunTest(const FString& Parameters)
{
    UWorld* TestWorld = UWorld::CreateWorld(EWorldType::Game, false);
    UKalkiGridManager* GridManager = TestWorld->GetSubsystem<UKalkiGridManager>();

    GridManager->CreateGrid(10, 10, 100.0f, FVector::ZeroVector);

    // Test valid coordinates
    TestTrue(TEXT("(0,0) should be valid"), GridManager->IsValidCoord(FKalkiGridCoord(0, 0)));
    TestTrue(TEXT("(9,9) should be valid"), GridManager->IsValidCoord(FKalkiGridCoord(9, 9)));
    TestTrue(TEXT("(5,5) should be valid"), GridManager->IsValidCoord(FKalkiGridCoord(5, 5)));

    // Test invalid coordinates
    TestFalse(TEXT("(-1,0) should be invalid"), GridManager->IsValidCoord(FKalkiGridCoord(-1, 0)));
    TestFalse(TEXT("(0,-1) should be invalid"), GridManager->IsValidCoord(FKalkiGridCoord(0, -1)));
    TestFalse(TEXT("(10,0) should be invalid"), GridManager->IsValidCoord(FKalkiGridCoord(10, 0)));
    TestFalse(TEXT("(0,10) should be invalid"), GridManager->IsValidCoord(FKalkiGridCoord(0, 10)));
    TestFalse(TEXT("(100,100) should be invalid"), GridManager->IsValidCoord(FKalkiGridCoord(100, 100)));

    TestWorld->DestroyWorld(false);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridManagerNeighborsTest,
    "Kalki.Grid.GridManager.GetNeighbors",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridManagerNeighborsTest::RunTest(const FString& Parameters)
{
    UWorld* TestWorld = UWorld::CreateWorld(EWorldType::Game, false);
    UKalkiGridManager* GridManager = TestWorld->GetSubsystem<UKalkiGridManager>();

    GridManager->CreateGrid(10, 10, 100.0f, FVector::ZeroVector);

    // Test center tile (should have 4 cardinal neighbors)
    FKalkiGridCoord Center(5, 5);
    TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(Center, false);
    TestEqual(TEXT("Center tile should have 4 cardinal neighbors"), Neighbors.Num(), 4);

    // Test center tile with diagonals (should have 8 neighbors)
    TArray<FKalkiGridCoord> AllNeighbors = GridManager->GetNeighbors(Center, true);
    TestEqual(TEXT("Center tile should have 8 neighbors with diagonals"), AllNeighbors.Num(), 8);

    // Test corner tile (0,0) - should have 2 cardinal neighbors
    FKalkiGridCoord Corner(0, 0);
    TArray<FKalkiGridCoord> CornerNeighbors = GridManager->GetNeighbors(Corner, false);
    TestEqual(TEXT("Corner tile should have 2 cardinal neighbors"), CornerNeighbors.Num(), 2);

    // Test corner tile with diagonals - should have 3 neighbors
    TArray<FKalkiGridCoord> CornerAllNeighbors = GridManager->GetNeighbors(Corner, true);
    TestEqual(TEXT("Corner tile should have 3 neighbors with diagonals"), CornerAllNeighbors.Num(), 3);

    TestWorld->DestroyWorld(false);

    return true;
}
```

---

### Example: ViewModel Tests
```cpp
// KalkiCombatLogViewModelTest.cpp

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiCombatLogViewModelTest,
    "Kalki.UI.CombatLog.ViewModel.AddMessage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiCombatLogViewModelTest::RunTest(const FString& Parameters)
{
    // Create ViewModel
    UKalkiCombatLogViewModel* ViewModel = NewObject<UKalkiCombatLogViewModel>();
    ViewModel->Initialize();

    // Test initial state
    TestEqual(TEXT("ViewModel should start empty"), ViewModel->DisplayEntries.Num(), 0);

    // Create test message
    FKalkiLogMessage TestMessage;
    TestMessage.Message = TEXT("Test combat message");
    TestMessage.Severity = EKalkiLogSeverity::Log;
    TestMessage.Channel = EKalkiLogChannel::Combat;

    // Add message
    ViewModel->AddLogMessage(TestMessage);

    // Verify message added
    TestEqual(TEXT("ViewModel should have one message"), ViewModel->DisplayEntries.Num(), 1);
    TestEqual(
        TEXT("Message text should match"),
        ViewModel->DisplayEntries[0].DisplayText.ToString(),
        TEXT("Test combat message")
    );
    TestEqual(
        TEXT("Message severity should match"),
        ViewModel->DisplayEntries[0].Severity,
        EKalkiLogSeverity::Log
    );

    // Test message limit (100 max)
    for (int32 i = 0; i < 150; ++i)
    {
        FKalkiLogMessage ExtraMessage;
        ExtraMessage.Message = FString::Printf(TEXT("Message %d"), i);
        ExtraMessage.Severity = EKalkiLogSeverity::Log;
        ExtraMessage.Channel = EKalkiLogChannel::Combat;
        ViewModel->AddLogMessage(ExtraMessage);
    }

    TestEqual(
        TEXT("ViewModel should be limited to 100 messages"),
        ViewModel->DisplayEntries.Num(),
        100
    );

    // Cleanup
    ViewModel->Shutdown();

    return true;
}
```

---

## Manual Test Cases

### Test Case Template
```
Test Case ID: TC-GRID-001
Title: Grid Creation and Visualization
Priority: High
Category: Grid System
Prerequisites: None

Test Steps:
1. Start PIE
2. Open console (~)
3. Execute: CreateTestGrid 20 20
4. Verify: Grid appears in viewport
5. Verify: Grid is 20x20 tiles
6. Verify: Tiles are evenly spaced
7. Verify: Grid origin is at (0,0,0)

Expected Results:
- Grid renders correctly
- All 400 tiles visible
- No visual artifacts
- Console shows "Grid created: 20x20"

Actual Results:
[To be filled during test]

Pass/Fail:
[To be filled during test]

Notes:
[Any observations]
```

---

### Core Test Cases

**Grid System:**
```
TC-GRID-001: Grid Creation
TC-GRID-002: Tile Hover Highlight
TC-GRID-003: Tile Selection
TC-GRID-004: Elevation Display
TC-GRID-005: Walkability Visualization
TC-GRID-006: Movement Range Preview
TC-GRID-007: Coordinate Conversion
```

**Character System:**
```
TC-CHAR-001: Character Spawn
TC-CHAR-002: Character Movement
TC-CHAR-003: Take Damage
TC-CHAR-004: Healing
TC-CHAR-005: Death
TC-CHAR-006: Stats Display
TC-CHAR-007: Ability Casting
```

**Combat System:**
```
TC-COMBAT-001: Start Combat
TC-COMBAT-002: Turn Order
TC-COMBAT-003: Attack Action
TC-COMBAT-004: Ability Use
TC-COMBAT-005: Combat Log Update
TC-COMBAT-006: End Combat
TC-COMBAT-007: Victory/Defeat
```

**Multiplayer:**
```
TC-MP-001: Host Game
TC-MP-002: Join Game
TC-MP-003: Character Replication
TC-MP-004: Movement Sync
TC-MP-005: Combat Sync
TC-MP-006: Disconnect Handling
TC-MP-007: Reconnect
```

---

## Performance Testing

### Frame Rate Testing

**Target Performance:**
- 60 FPS @ 1080p (mid-range PC)
- 30 FPS minimum

**Test Scenarios:**
```
1. Empty Grid (20x20)
   - Expected: 60+ FPS

2. Grid with Characters (10 characters)
   - Expected: 60 FPS

3. Active Combat (4v4 battle)
   - Expected: 60 FPS

4. Large Grid (50x50)
   - Expected: 45+ FPS

5. VFX Heavy (multiple abilities)
   - Expected: 45+ FPS
```

**Console Commands:**
```
stat fps              // Show frame rate
stat unit             // Show frame time breakdown
stat game             // Game thread time
stat scenerendering   // Rendering stats
```

---

### Memory Testing

**Monitor Memory Usage:**
```
stat memory           // Memory stats
memreport             // Detailed memory report
obj list              // List all objects
```

**Test Scenarios:**
```
1. Idle in menu
   - Baseline memory usage

2. Grid created
   - Memory increase should be < 50MB

3. 10 characters spawned
   - Memory increase should be < 100MB

4. After 1 hour gameplay
   - Check for memory leaks
   - Memory should stabilize
```

---

### Network Performance Testing

**Latency Testing:**
```
// Simulate network conditions
net PktLag=50          // 50ms latency (good)
net PktLag=100         // 100ms latency (acceptable)
net PktLag=200         // 200ms latency (poor)
net PktLagVariance=30  // ±30ms jitter
```

**Bandwidth Testing:**
```
stat net              // Network stats
net.Replication.DebugDraw 1  // Show replication
```

**Test Scenarios:**
```
1. 2 players, no lag
   - Verify smooth gameplay
   - Verify instant feedback

2. 2 players, 100ms lag
   - Verify prediction works
   - Verify corrections smooth

3. 4 players, 100ms lag + 2% packet loss
   - Verify no desyncs
   - Verify error recovery
```

---

## Regression Testing

### Regression Test Suite

**Purpose:** Ensure new code doesn't break existing features

**Frequency:** Run before every commit/merge

**Test Suite:**
```
Core Systems:
- Grid creation
- Tile queries
- Coordinate conversion
- Neighbor queries
- Range queries

Visualization:
- Grid rendering
- Hover highlights
- Selection feedback
- Elevation display

UI:
- Widget creation
- ViewModel binding
- Data updates
- Event handling

Multiplayer:
- Character replication
- Movement sync
- RPC execution
- Authority checks
```

**Automation:**
```cpp
// Run all tests
Automation RunTests Kalki

// Run specific category
Automation RunTests Kalki.Grid

// Run with filter
Automation RunTests Kalki.Grid.GridManager
```

---

## Bug Reporting

### Bug Report Template
```
Bug ID: BUG-GRID-042
Title: [Brief description]
Severity: Critical / High / Medium / Low
Priority: P1 / P2 / P3
Category: Grid / Character / Combat / UI / Network
Status: Open / In Progress / Resolved / Closed

Description:
[Detailed description of the bug]

Steps to Reproduce:
1. [Step 1]
2. [Step 2]
3. [Step 3]

Expected Behavior:
[What should happen]

Actual Behavior:
[What actually happens]

Reproduction Rate:
100% / Sometimes / Rare

Environment:
- Build: Development / Shipping
- Platform: Windows / Mac / Linux
- Net Mode: Standalone / Listen Server / Dedicated Server / Client

Logs:
[Relevant log output]

Screenshots/Video:
[Attach if applicable]

Additional Notes:
[Any other relevant information]
```

---

### Bug Severity Definitions

**Critical (P1):**
- Crash
- Data loss
- Multiplayer desync
- Cannot proceed in game

**High (P2):**
- Major feature broken
- Frequent issue
- Significant impact on gameplay

**Medium (P3):**
- Minor feature broken
- Workaround exists
- Moderate impact

**Low (P4):**
- Cosmetic issue
- Rare occurrence
- Minimal impact

---

## Testing Checklist

### Pre-Commit Checklist

Before committing code:
- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] Unit tests pass
- [ ] Manual testing complete
- [ ] No console errors in PIE
- [ ] Multiplayer tested (if applicable)
- [ ] Performance acceptable
- [ ] Documentation updated
- [ ] Code reviewed

---

### Pre-Release Checklist

Before releasing a build:
- [ ] All critical bugs fixed
- [ ] All high priority bugs fixed
- [ ] Regression tests pass
- [ ] Performance tests pass
- [ ] Multiplayer tests pass (all modes)
- [ ] All platforms tested
- [ ] Content reviewed
- [ ] Build size acceptable
- [ ] Save/load works
- [ ] Settings persist
- [ ] Tutorial works
- [ ] All features accessible
- [ ] No placeholder content
- [ ] Credits complete

---

## Related Documents

### Foundation
- [[04_Implementation/Error_Handling]] - Error handling patterns to test
- [[04_Implementation/Multiplayer_Guidelines]] - Multiplayer testing
- [[04_Implementation/Common_Patterns]] - Patterns to test

### Systems
- [[03_Systems/Grid_System]] - Grid testing examples
- [[03_Systems/Logging_System]] - Log analysis for testing

### Implementation
- [[04_Implementation/Class_Templates]] - Test templates
- [[02_Conventions/Coding_Standards]] - Code quality standards

---

## Quick Reference

### Running Tests

**Console:**
```
// Run all tests
Automation RunTests Kalki

// Run specific category
Automation RunTests Kalki.Grid
Automation RunTests Kalki.UI

// List available tests
Automation List

// Set verbosity
Automation SetMinimumPriority High
```

**Command Line:**
```
// Run tests from command line
UnrealEditor.exe ProjectName.uproject -ExecCmds="Automation RunTests Kalki; Quit"
```

---

### Common Test Commands
```
// Grid
CreateTestGrid 20 20
PrintGridInfo
PrintTileInfo 10 10

// Character
SpawnTestCharacter 5 5
SetHP 50
TeleportToTile 10 10

// Combat
StartTestCombat
NextTurn
EndCombat

// Debug
ShowDebugInfo true
stat fps
stat memory
```

---

## Document Info
**File Path:** `Kalki_GDD/04_Implementation/Testing_Procedures.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate  
**Note:** Comprehensive testing guide for all systems