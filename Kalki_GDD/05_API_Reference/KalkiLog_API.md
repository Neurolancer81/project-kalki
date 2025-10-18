---
tags:
  - kalki/api
  - kalki/logging
  - kalki/reference
status: complete
phase: 1
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# KalkiLog API Reference

## Overview
Complete API reference for the KalkiLog static logging utility. This document provides detailed information about all logging functions, channels, severity levels, and usage patterns.

---

## Purpose

### What is KalkiLog?

**KalkiLog** is a centralized, channel-based logging system for Kalki. It provides:
- Static utility functions (no instance needed)
- Multiple logging channels (System, Combat, Movement, etc.)
- Four severity levels (Verbose, Log, Warning, Error)
- Optional metadata and context tracking
- Combat session tracking
- Zero runtime cost in shipping builds

**See also:** [[03_Systems/Logging_System]] for architectural details and patterns.

---

## Quick Start

### Basic Usage

```cpp
#include "Logging/KalkiLog.h"

// Simple log
KalkiLog::Grid(TEXT("Grid initialized"));

// With severity
KalkiLog::Combat(TEXT("Invalid target"), EKalkiLogSeverity::Warning);

// With context
KalkiLog::Character(TEXT("HP changed"), EKalkiLogSeverity::Log, this);

// With formatting
KalkiLog::Grid(
    FString::Printf(TEXT("Tile (%d, %d) elevation: %.1f"), X, Y, Elevation)
);

// Verbose logging (debug details)
KalkiLog::AI(TEXT("Detailed AI state"), EKalkiLogSeverity::Verbose);
````

---

## Core Function

### Log()

**Signature:**

```cpp
static void Log(
    EKalkiLogChannel Channel,
    const FString& Message,
    EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
    const UObject* Context = nullptr
);
```

**Parameters:**

- `Channel` - Which logging channel to use
- `Message` - The message to log
- `Severity` - Severity level (default: Log)
- `Context` - Optional context object (default: nullptr)

**Description:** Core logging function. All channel-specific functions call this internally.

**Example:**

```cpp
KalkiLog::Log(
    EKalkiLogChannel::Grid,
    TEXT("Grid created"),
    EKalkiLogSeverity::Log,
    this
);
```

**Notes:**

- Prefer channel-specific functions (Grid, Combat, etc.) for cleaner code
- Use this directly only when channel is dynamic

---

## Channel Functions

All channel functions have the same signature:

```cpp
static void ChannelName(
    const FString& Message,
    EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
    const UObject* Context = nullptr
);
```

---

### System()

**Channel:** `EKalkiLogChannel::System`

**Purpose:** Core system and initialization logging

**Example:**

```cpp
KalkiLog::System(TEXT("GridManager initialized"));
KalkiLog::System(TEXT("Failed to load config"), EKalkiLogSeverity::Error);
KalkiLog::System(
    FString::Printf(TEXT("Subsystem loaded: %s"), *SubsystemName)
);
```

**When to use:**

- Subsystem initialization/shutdown
- System state changes
- Configuration loading
- Critical system events
- Application lifecycle

---

### Combat()

**Channel:** `EKalkiLogChannel::Combat`

**Purpose:** Combat system logging

**Example:**

```cpp
KalkiLog::Combat(TEXT("Character took 10 damage"));
KalkiLog::Combat(
    FString::Printf(TEXT("%s attacked %s for %d damage"), 
        *Attacker->GetName(), *Target->GetName(), Damage)
);
KalkiLog::Combat(TEXT("Invalid target"), EKalkiLogSeverity::Warning);
```

**When to use:**

- Damage dealt/received
- Attack actions
- Combat state changes
- Combat validation
- Hit/miss results

---

### Movement()

**Channel:** `EKalkiLogChannel::Movement`

**Purpose:** Character movement logging

**Example:**

```cpp
KalkiLog::Movement(TEXT("Character moved to (5, 7)"));
KalkiLog::Movement(
    FString::Printf(TEXT("%s moved from (%d,%d) to (%d,%d)"),
        *Character->GetName(), StartX, StartY, EndX, EndY)
);
KalkiLog::Movement(TEXT("Path blocked"), EKalkiLogSeverity::Warning);
```

**When to use:**

- Character movement
- Pathfinding
- Movement validation
- Teleportation
- Position updates

---

### Abilities()

**Channel:** `EKalkiLogChannel::Abilities`

**Purpose:** Ability system logging

**Example:**

```cpp
KalkiLog::Abilities(TEXT("Fireball cast"));
KalkiLog::Abilities(
    FString::Printf(TEXT("%s cast %s"), *Character->GetName(), *AbilityName)
);
KalkiLog::Abilities(TEXT("Not enough mana"), EKalkiLogSeverity::Warning);
```

**When to use:**

- Ability casting
- Ability validation
- Cooldown management
- Resource costs
- Target selection
- Effect application

---

### TurnSystem()

**Channel:** `EKalkiLogChannel::TurnSystem`

**Purpose:** Turn-based system logging

**Example:**

```cpp
KalkiLog::TurnSystem(TEXT("Turn started"));
KalkiLog::TurnSystem(
    FString::Printf(TEXT("Turn %d: %s"), TurnNumber, *Character->GetName())
);
KalkiLog::TurnSystem(TEXT("Initiative rolled"));
```

**When to use:**

- Turn order
- Initiative rolls
- Turn transitions
- Action point management
- Round tracking

---

### Grid()

**Channel:** `EKalkiLogChannel::Grid`

**Purpose:** Grid system logging

**Example:**

```cpp
KalkiLog::Grid(TEXT("Grid created: 20x20"));
KalkiLog::Grid(
    FString::Printf(TEXT("Tile (%d, %d) elevation set to %.1f"), X, Y, Elev)
);
KalkiLog::Grid(TEXT("Invalid coordinate"), EKalkiLogSeverity::Warning);
```

**When to use:**

- Grid creation/destruction
- Tile modifications
- Coordinate validation
- Grid queries
- Occupancy changes

---

### Dialogue()

**Channel:** `EKalkiLogChannel::Dialogue`

**Purpose:** Dialogue system logging

**Example:**

```cpp
KalkiLog::Dialogue(TEXT("Conversation started"));
KalkiLog::Dialogue(
    FString::Printf(TEXT("%s: %s"), *Speaker, *Line)
);
KalkiLog::Dialogue(TEXT("Dialogue node not found"), EKalkiLogSeverity::Error);
```

**When to use:**

- Conversation flow
- Dialogue choices
- NPC interactions
- Quest dialogue
- Dialogue validation

---

### Inventory()

**Channel:** `EKalkiLogChannel::Inventory`

**Purpose:** Inventory system logging

**Example:**

```cpp
KalkiLog::Inventory(TEXT("Item added to inventory"));
KalkiLog::Inventory(
    FString::Printf(TEXT("%s acquired %s"), *Character->GetName(), *ItemName)
);
KalkiLog::Inventory(TEXT("Inventory full"), EKalkiLogSeverity::Warning);
```

**When to use:**

- Item acquisition/removal
- Equipment changes
- Inventory capacity
- Item usage
- Trading

---

### AI()

**Channel:** `EKalkiLogChannel::AI`

**Purpose:** AI system logging

**Example:**

```cpp
KalkiLog::AI(TEXT("Enemy selected target"));
KalkiLog::AI(
    FString::Printf(TEXT("%s chose action: %s"), *Enemy->GetName(), *Action)
);
KalkiLog::AI(TEXT("No valid targets found"), EKalkiLogSeverity::Warning);
```

**When to use:**

- AI decision making
- Behavior tree execution
- Target selection
- Pathfinding
- AI state changes

---

### Network()

**Channel:** `EKalkiLogChannel::Network`

**Purpose:** Multiplayer/networking logging

**Example:**

```cpp
KalkiLog::Network(TEXT("Client connected"));
KalkiLog::Network(
    FString::Printf(TEXT("RPC received: %s"), *RPCName)
);
KalkiLog::Network(TEXT("Replication failed"), EKalkiLogSeverity::Error);
```

**When to use:**

- Connection/disconnection events
- RPC calls
- Replication events
- Network errors
- Authority validation
- Desync detection

---

### Quest()

**Channel:** `EKalkiLogChannel::Quest`

**Purpose:** Quest system logging

**Example:**

```cpp
KalkiLog::Quest(TEXT("Quest started: Rescue the Princess"));
KalkiLog::Quest(
    FString::Printf(TEXT("Quest objective completed: %s"), *ObjectiveName)
);
KalkiLog::Quest(TEXT("Quest already active"), EKalkiLogSeverity::Warning);
```

**When to use:**

- Quest start/completion
- Objective tracking
- Quest state changes
- Reward distribution
- Quest validation

---

### Ruleset()

**Channel:** `EKalkiLogChannel::Ruleset`

**Purpose:** D&D 5e rules implementation logging

**Example:**

```cpp
KalkiLog::Ruleset(TEXT("Advantage applied to attack roll"));
KalkiLog::Ruleset(
    FString::Printf(TEXT("Saving throw: %s DC %d - %s"),
        *SaveType, DC, bSuccess ? TEXT("Success") : TEXT("Failure"))
);
KalkiLog::Ruleset(TEXT("Invalid ability score"), EKalkiLogSeverity::Error);
```

**When to use:**

- Dice rolls
- Saving throws
- Ability checks
- Advantage/disadvantage
- Rules validation
- Modifier calculations

---

### Debug()

**Channel:** `EKalkiLogChannel::Debug`

**Purpose:** Debug-only logging

**Example:**

```cpp
KalkiLog::Debug(TEXT("Debug info"));
KalkiLog::Debug(
    FString::Printf(TEXT("Variable state: %s = %d"), *VarName, Value),
    EKalkiLogSeverity::Verbose
);
```

**When to use:**

- Temporary debug output
- Development-only logs
- Performance tracking
- State inspection
- Testing information

---

### UI()

**Channel:** `EKalkiLogChannel::UI`

**Purpose:** User interface logging

**Example:**

```cpp
KalkiLog::UI(TEXT("Widget created: CombatLog"));
KalkiLog::UI(TEXT("ViewModel initialized"));
KalkiLog::UI(TEXT("Widget binding failed"), EKalkiLogSeverity::Error);
```

**When to use:**

- Widget creation/destruction
- ViewModel operations
- UI events
- Data binding
- User input handling

---

## Enums

### EKalkiLogChannel

**Definition:**

```cpp
UENUM(BlueprintType)
enum class EKalkiLogChannel : uint8
{
    System      UMETA(DisplayName = "System"),
    Combat      UMETA(DisplayName = "Combat"),
    Movement    UMETA(DisplayName = "Movement"),
    Abilities   UMETA(DisplayName = "Abilities"),
    TurnSystem  UMETA(DisplayName = "Turn System"),
    Grid        UMETA(DisplayName = "Grid"),
    Dialogue    UMETA(DisplayName = "Dialogue"),
    Inventory   UMETA(DisplayName = "Inventory"),
    AI          UMETA(DisplayName = "AI"),
    Network     UMETA(DisplayName = "Network"),
    Quest       UMETA(DisplayName = "Quest"),
    Ruleset     UMETA(DisplayName = "Ruleset"),
    Debug       UMETA(DisplayName = "Debug"),
    UI          UMETA(DisplayName = "UI")
};
```

**Values:**

|Value|Description|Use Case|
|---|---|---|
|`System`|Core systems|Initialization, critical events|
|`Combat`|Combat system|Damage, attacks, combat flow|
|`Movement`|Movement system|Character movement, pathfinding|
|`Abilities`|Ability system|Casting, cooldowns, validation|
|`TurnSystem`|Turn-based combat|Turn order, initiative, actions|
|`Grid`|Grid system|Tiles, coordinates, grid queries|
|`Dialogue`|Dialogue system|Conversations, NPC interactions|
|`Inventory`|Inventory system|Items, equipment, trading|
|`AI`|AI system|Decision making, behavior|
|`Network`|Multiplayer|RPCs, replication, connections|
|`Quest`|Quest system|Quest tracking, objectives|
|`Ruleset`|D&D 5e rules|Dice rolls, rules validation|
|`Debug`|Debug logging|Temporary debug output|
|`UI`|User interface|Widgets, ViewModels, input|

---

### EKalkiLogSeverity

**Definition:**

```cpp
UENUM(BlueprintType)
enum class EKalkiLogSeverity : uint8
{
    Verbose     UMETA(DisplayName = "Verbose"),
    Log         UMETA(DisplayName = "Log"),
    Warning     UMETA(DisplayName = "Warning"),
    Error       UMETA(DisplayName = "Error")
};
```

**Values:**

|Value|Color|Use Case|
|---|---|---|
|`Verbose`|Gray|Detailed debug info, state dumps|
|`Log`|White|Normal information, expected events|
|`Warning`|Yellow|Unexpected but recoverable, potential issues|
|`Error`|Red|Critical problems, failures|

**Guidelines:**

**Use Verbose for:**

- Detailed debug information
- State dumps
- Performance metrics
- Trace-level logging
- Development-only details

**Use Log for:**

- Normal operation
- Successful actions
- State changes
- Standard information

**Use Warning for:**

- Unexpected input (but handled)
- Fallback behavior triggered
- Deprecated usage
- Performance concerns
- Validation failures (non-critical)

**Use Error for:**

- Critical failures
- Null pointers
- Invalid state
- Unrecoverable errors
- System failures

---

### EKalkiLogMetadataLevel

**Definition:**

```cpp
UENUM(BlueprintType)
enum class EKalkiLogMetadataLevel : uint8
{
    None        UMETA(DisplayName = "None"),
    Minimal     UMETA(DisplayName = "Minimal"),
    Full        UMETA(DisplayName = "Full")
};
```

**Values:**

|Value|Description|
|---|---|
|`None`|No metadata attached to logs|
|`Minimal`|Basic metadata only (timestamp, channel, severity)|
|`Full`|All metadata (includes source object, session info, custom data)|

**Purpose:** Controls how much additional data is logged with each message. Higher levels provide more debugging information but use more memory and processing.

---

## Data Structures

### FKalkiLogMessage

**Definition:**

```cpp
USTRUCT(BlueprintType)
struct KALKI_API FKalkiLogMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    EKalkiLogChannel Channel;

    UPROPERTY(BlueprintReadOnly)
    EKalkiLogSeverity Severity;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly)
    int32 CombatSessionID;

    UPROPERTY(BlueprintReadOnly)
    FString CombatSessionName;

    UPROPERTY()
    TWeakObjectPtr<UObject> Source;
};
```

**Properties:**

**Timestamp** - When the message was logged

- Type: `FDateTime`
- Use: Sorting, filtering by time, session tracking

**Channel** - Category of the message

- Type: `EKalkiLogChannel`
- Use: Filtering by system/feature

**Severity** - Importance level

- Type: `EKalkiLogSeverity`
- Use: Filtering by criticality, color coding

**Message** - The actual log text

- Type: `FString`
- Use: Display, search, analysis

**Metadata** - Optional key-value pairs

- Type: `TMap<FString, FString>`
- Use: Additional context, structured data
- Example: `{"ActorName": "Fighter", "HP": "45", "MaxHP": "55"}`

**CombatSessionID** - Combat session this log belongs to

- Type: `int32`
- Value: Session ID or -1 if not in combat
- Use: Filter logs by combat encounter

**CombatSessionName** - Name of the combat session

- Type: `FString`
- Value: Session name or empty if not in combat
- Use: Display, filtering, organization

**Source** - Object that triggered the log

- Type: `TWeakObjectPtr<UObject>`
- Use: Context tracking, debugging
- Note: Weak pointer to avoid preventing garbage collection

---

### FKalkiLogMessage Helper Methods

**ToString():**

```cpp
FString ToString(bool bIncludeTimestamp = true, bool bIncludeChannel = true) const;
```

**Purpose:** Format message for display

**Parameters:**

- `bIncludeTimestamp` - Include timestamp in output
- `bIncludeChannel` - Include channel name in output

**Returns:** Formatted string

**Example:**

```cpp
FKalkiLogMessage Msg(EKalkiLogChannel::Combat, EKalkiLogSeverity::Log, TEXT("Attack hit"));
FString Formatted = Msg.ToString();
// Output: "[14:23:45][Combat][Log] Attack hit"

FString SimpleFormat = Msg.ToString(false, false);
// Output: "[Log] Attack hit"
```

---

**GetColor():**

```cpp
FLinearColor GetColor() const;
```

**Purpose:** Get display color based on severity

**Returns:** Color for UI display

**Example:**

```cpp
FKalkiLogMessage Msg(EKalkiLogChannel::Combat, EKalkiLogSeverity::Warning, TEXT("Low HP"));
FLinearColor Color = Msg.GetColor();
// Returns: FLinearColor::Yellow

TextBlock->SetColorAndOpacity(Color);
```

**Color Map:**

- Verbose → Gray
- Log → White
- Warning → Yellow
- Error → Red

---

## Usage Patterns

### Pattern: Simple Logging

**When:** Logging simple messages without parameters

```cpp
KalkiLog::Grid(TEXT("Grid initialized"));
KalkiLog::Combat(TEXT("Combat started"));
KalkiLog::UI(TEXT("Widget created"));
KalkiLog::TurnSystem(TEXT("New turn"));
```

---

### Pattern: Formatted Messages

**When:** Logging with dynamic values

```cpp
// Using FString::Printf
KalkiLog::Grid(
    FString::Printf(TEXT("Grid created: %dx%d tiles"), SizeX, SizeY)
);

KalkiLog::Combat(
    FString::Printf(TEXT("%s dealt %d damage to %s"),
        *Attacker->GetName(),
        DamageAmount,
        *Target->GetName()
    )
);

KalkiLog::Movement(
    FString::Printf(TEXT("%s moved from (%d,%d) to (%d,%d)"),
        *Character->GetName(), OldX, OldY, NewX, NewY
    )
);
```

---

### Pattern: Severity Levels

**When:** Logging with different importance

```cpp
// Verbose - Detailed debug info
KalkiLog::AI(TEXT("Detailed AI state evaluation"), EKalkiLogSeverity::Verbose);

// Log - Normal operation (default)
KalkiLog::Grid(TEXT("Tile updated"));

// Warning - Unexpected but handled
KalkiLog::Grid(TEXT("Invalid coordinate, using default"), EKalkiLogSeverity::Warning);

// Error - Critical failure
KalkiLog::Grid(TEXT("GridManager is null!"), EKalkiLogSeverity::Error);
```

---

### Pattern: Context Tracking

**When:** Want to know which object logged the message

```cpp
// In actor/component
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    KalkiLog::Combat(
        FString::Printf(TEXT("Took %d damage"), Damage),
        EKalkiLogSeverity::Log,
        this  // Pass 'this' as context
    );
}

// Output includes context: "[Character_Fighter] Took 10 damage"
```

---

### Pattern: Combat Session Tracking

**When:** Grouping logs by combat encounter

```cpp
// Start combat session
void StartCombat(const FString& EncounterName)
{
    int32 SessionID = GenerateSessionID();
    
    KalkiLog::Combat(
        FString::Printf(TEXT("Combat started: %s (Session %d)"), 
            *EncounterName, SessionID)
    );
    
    // All subsequent combat logs automatically include session ID
    CurrentCombatSessionID = SessionID;
}

// Combat logs during session
void OnAttack()
{
    // This log will be tagged with current session ID
    KalkiLog::Combat(TEXT("Attack executed"));
}

// End combat session
void EndCombat()
{
    KalkiLog::Combat(
        FString::Printf(TEXT("Combat ended (Session %d)"), CurrentCombatSessionID)
    );
    
    CurrentCombatSessionID = -1;
}
```

---

### Pattern: Metadata Attachment

**When:** Adding structured data to logs

```cpp
// Create log message with metadata
FKalkiLogMessage LogMsg(
    EKalkiLogChannel::Combat,
    EKalkiLogSeverity::Log,
    TEXT("Character took damage")
);

// Add metadata
LogMsg.Metadata.Add(TEXT("CharacterName"), Character->GetName());
LogMsg.Metadata.Add(TEXT("DamageAmount"), FString::FromInt(Damage));
LogMsg.Metadata.Add(TEXT("DamageType"), UEnum::GetValueAsString(DamageType));
LogMsg.Metadata.Add(TEXT("CurrentHP"), FString::FromInt(CurrentHP));
LogMsg.Metadata.Add(TEXT("MaxHP"), FString::FromInt(MaxHP));

// Log it
LogSystem->LogMessage(LogMsg);
```

---

### Pattern: Verbose Debug Logging

**When:** Detailed debugging information

```cpp
void EvaluateAIDecision()
{
    KalkiLog::AI(TEXT("=== AI Decision Evaluation Start ==="), EKalkiLogSeverity::Verbose);
    
    KalkiLog::AI(
        FString::Printf(TEXT("Current State: %s"), *GetCurrentState()),
        EKalkiLogSeverity::Verbose
    );
    
    KalkiLog::AI(
        FString::Printf(TEXT("Evaluating %d possible actions"), PossibleActions.Num()),
        EKalkiLogSeverity::Verbose
    );
    
    for (const FAction& Action : PossibleActions)
    {
        KalkiLog::AI(
            FString::Printf(TEXT("  Action: %s, Score: %.2f"), *Action.Name, Action.Score),
            EKalkiLogSeverity::Verbose
        );
    }
    
    KalkiLog::AI(
        FString::Printf(TEXT("Selected Action: %s"), *BestAction.Name),
        EKalkiLogSeverity::Verbose
    );
    
    KalkiLog::AI(TEXT("=== AI Decision Evaluation End ==="), EKalkiLogSeverity::Verbose);
}
```

---

### Pattern: Turn System Logging

**When:** Tracking turn-based combat flow

```cpp
void StartNewTurn(AKalkiCharacter* Character, int32 TurnNumber)
{
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("=== Turn %d: %s ==="), TurnNumber, *Character->GetName())
    );
    
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("Initiative: %d"), Character->GetInitiative())
    );
    
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("Action Points: %d"), Character->GetActionPoints())
    );
}

void EndTurn(AKalkiCharacter* Character)
{
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("%s ended turn"), *Character->GetName())
    );
}
```

---

### Pattern: Ruleset Logging

**When:** Logging D&D 5e rules implementation

```cpp
void RollAttack(AKalkiCharacter* Attacker, AKalkiCharacter* Target)
{
    int32 D20Roll = FMath::RandRange(1, 20);
    int32 AttackBonus = Attacker->GetAttackBonus();
    int32 TotalRoll = D20Roll + AttackBonus;
    int32 TargetAC = Target->GetArmorClass();
    
    KalkiLog::Ruleset(
        FString::Printf(TEXT("Attack Roll: d20(%d) + %d = %d vs AC %d"),
            D20Roll, AttackBonus, TotalRoll, TargetAC)
    );
    
    if (D20Roll == 20)
    {
        KalkiLog::Ruleset(TEXT("Critical Hit!"));
    }
    else if (D20Roll == 1)
    {
        KalkiLog::Ruleset(TEXT("Critical Miss!"));
    }
    else if (TotalRoll >= TargetAC)
    {
        KalkiLog::Ruleset(TEXT("Attack Hit"));
    }
    else
    {
        KalkiLog::Ruleset(TEXT("Attack Miss"));
    }
}

void RollSavingThrow(AKalkiCharacter* Character, EAbility Ability, int32 DC)
{
    int32 D20Roll = FMath::RandRange(1, 20);
    int32 Modifier = Character->GetAbilityModifier(Ability);
    int32 Total = D20Roll + Modifier;
    bool bSuccess = Total >= DC;
    
    KalkiLog::Ruleset(
        FString::Printf(TEXT("%s Saving Throw: d20(%d) + %d = %d vs DC %d - %s"),
            *UEnum::GetValueAsString(Ability),
            D20Roll,
            Modifier,
            Total,
            DC,
            bSuccess ? TEXT("Success") : TEXT("Failure")
        )
    );
}
```

---

## Best Practices

### ✅ Do This:

**1. Use appropriate channel:**

```cpp
// ✅ GOOD - Correct channels
KalkiLog::Grid(TEXT("Grid created"));
KalkiLog::Combat(TEXT("Damage dealt"));
KalkiLog::Movement(TEXT("Character moved"));
KalkiLog::TurnSystem(TEXT("Turn started"));
KalkiLog::Ruleset(TEXT("d20 rolled"));
```

**2. Use appropriate severity:**

```cpp
// ✅ GOOD - Correct severity levels
KalkiLog::Grid(TEXT("Tile updated"));  // Normal = Log
KalkiLog::AI(TEXT("AI state details"), EKalkiLogSeverity::Verbose);  // Debug = Verbose
KalkiLog::Grid(TEXT("Invalid coord"), EKalkiLogSeverity::Warning);  // Unexpected = Warning
KalkiLog::Grid(TEXT("Manager is null"), EKalkiLogSeverity::Error);  // Critical = Error
```

**3. Provide context:**

```cpp
// ✅ GOOD - Includes who/what/where
KalkiLog::Combat(
    FString::Printf(TEXT("%s dealt %d damage to %s"),
        *Attacker->GetName(), Damage, *Target->GetName()
    ),
    EKalkiLogSeverity::Log,
    this
);
```

**4. Use Verbose for debug details:**

```cpp
// ✅ GOOD - Verbose for detailed debug info
KalkiLog::AI(
    FString::Printf(TEXT("Evaluating action: Score=%.2f, Priority=%d"), Score, Priority),
    EKalkiLogSeverity::Verbose
);
```

**5. Track combat sessions:**

```cpp
// ✅ GOOD - Group logs by combat encounter
StartCombat("Goblin Ambush");
// ... combat logs automatically grouped
EndCombat();
```

---

### ❌ Don't Do This:

**1. Wrong channel:**

```cpp
// ❌ BAD - Grid event in Combat channel
KalkiLog::Combat(TEXT("Grid created"));

// ✅ GOOD
KalkiLog::Grid(TEXT("Grid created"));
```

**2. Wrong severity:**

```cpp
// ❌ BAD - Normal event as Error
KalkiLog::Grid(TEXT("Tile updated"), EKalkiLogSeverity::Error);

// ❌ BAD - Critical error as Verbose
KalkiLog::System(TEXT("Subsystem failed to initialize!"), EKalkiLogSeverity::Verbose);

// ✅ GOOD
KalkiLog::Grid(TEXT("Tile updated"));  // Default is Log
KalkiLog::System(TEXT("Subsystem failed!"), EKalkiLogSeverity::Error);
```

**3. Too verbose:**

```cpp
// ❌ BAD - Logs every frame
void Tick(float DeltaTime)
{
    KalkiLog::System(TEXT("Tick"));  // Spam!
}

// ✅ GOOD - Only log significant events
void OnHealthChanged(int32 NewHP)
{
    KalkiLog::Combat(FString::Printf(TEXT("HP changed to %d"), NewHP));
}
```

**4. No context:**

```cpp
// ❌ BAD - What failed? Where?
KalkiLog::System(TEXT("Failed"), EKalkiLogSeverity::Error);

// ✅ GOOD - Clear and specific
KalkiLog::System(
    FString::Printf(TEXT("Failed to load config: %s"), *Filename),
    EKalkiLogSeverity::Error,
    this
);
```

---

## Output Examples

### Console Output Format

**With context:**

```
LogKalkiGrid: [GridManager_0] Grid created: 20x20 tiles
LogKalkiCombat: [Character_Fighter] Took 10 damage (45/55 HP)
LogKalkiMovement: [Character_Wizard] Moved from (5,5) to (10,10)
LogKalkiTurnSystem: === Turn 3: Fighter ===
```

**Without context:**

```
LogKalkiGrid: Grid initialized
LogKalkiCombat: Combat started
LogKalkiSystem: Configuration loaded
LogKalkiRuleset: Attack Roll: d20(15) + 5 = 20 vs AC 18
```

**With severity:**

```
LogKalkiGrid: Verbose: Detailed tile state evaluation
LogKalkiGrid: Warning: Invalid coordinate, using default
LogKalkiCombat: Error: Character is null!
LogKalkiNetwork: Warning: RPC validation failed
```

**With combat session:**

```
LogKalkiCombat: [Session 1: Goblin Ambush] Combat started
LogKalkiCombat: [Session 1: Goblin Ambush] Fighter attacks Goblin
LogKalkiCombat: [Session 1: Goblin Ambush] Goblin takes 12 damage
LogKalkiCombat: [Session 1: Goblin Ambush] Combat ended
```

---

## Filtering Logs

### In Editor Output Log

**Filter by channel:**

1. Click filter icon
2. Type: `LogKalkiGrid` or `LogKalkiCombat` 33. Only logs from that channel are shown

**Filter by severity:**
1. Click "Warnings" button - Show warnings + errors only
2. Click "Errors" button - Show only errors
3. Uncheck both - Show all levels including Verbose

**Filter by text:**
1. Type search term in filter box
2. Shows logs containing that text
3. Example: "damage" shows all damage-related logs

**Filter by combat session:**
1. Search for session ID: `Session 1`
2. Shows all logs from that combat encounter

---

### In Code

**Conditional compilation:**
```cpp
#if !UE_BUILD_SHIPPING
    // Only in Development/Debug
    KalkiLog::Grid(TEXT("Debug information"));
#endif

#if UE_BUILD_DEBUG
    // Only in Debug
    KalkiLog::System(TEXT("Detailed debug info"), EKalkiLogSeverity::Verbose);
#endif
```

**Runtime conditions:**
```cpp
if (bVerboseLogging)
{
    KalkiLog::AI(TEXT("Detailed AI state"), EKalkiLogSeverity::Verbose);
}

if (bLogMovement)
{
    KalkiLog::Movement(
        FString::Printf(TEXT("%s moved to (%d,%d)"), *Name, X, Y)
    );
}
```

---

## Performance Considerations

### Zero Cost in Shipping

**All UE_LOG calls are compiled out in shipping builds:**
```cpp
// Development/Debug: Logs normally
// Shipping: Code removed entirely (zero cost)
KalkiLog::Grid(TEXT("Message"));
```

---

### String Formatting Cost

**FString::Printf has runtime cost:**
```cpp
// This always evaluates, even if log is disabled
KalkiLog::Grid(
    FString::Printf(TEXT("Value: %d"), ExpensiveCalculation())
);

// Better: Conditional for expensive operations
if (bDebugMode)
{
    KalkiLog::Grid(
        FString::Printf(TEXT("Value: %d"), ExpensiveCalculation())
    );
}
```

---

### Verbose Logging Performance

**Verbose logs can impact performance:**
```cpp
// Wrap verbose logging in conditionals
#if !UE_BUILD_SHIPPING
    if (bEnableVerboseLogging)
    {
        KalkiLog::AI(TEXT("Detailed state dump"), EKalkiLogSeverity::Verbose);
    }
#endif
```

---

### Logging Frequency

**Guidelines:**
- **Per-frame:** Avoid (except debug builds with flags)
- **Per-action:** Good (player input, state changes)
- **Per-event:** Good (spawn, destroy, combat)
- **Initialization:** Good (once per session)
- **Verbose:** Use sparingly, conditionally

---

## Common Use Cases

### Use Case 1: System Initialization
```cpp
void UKalkiGridManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    KalkiLog::System(TEXT("GridManager initializing..."));

    // Initialize data
    GridTiles.Empty();
    bGridCreated = false;

    KalkiLog::System(TEXT("GridManager initialized successfully"));
}
```

---

### Use Case 2: Combat with Session Tracking
```cpp
void AKalkiCombatManager::StartCombat(const FString& EncounterName)
{
    // Generate session
    CurrentSessionID = GenerateSessionID();
    CurrentSessionName = EncounterName;

    KalkiLog::Combat(
        FString::Printf(TEXT("=== Combat Started: %s (Session %d) ==="),
            *EncounterName, CurrentSessionID)
    );

    // Roll initiative
    KalkiLog::TurnSystem(TEXT("Rolling initiative..."));
    RollInitiative();

    // Start first turn
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("Turn 1: %s"), *GetCurrentCharacter()->GetName())
    );
}

void AKalkiCombatManager::EndCombat()
{
    KalkiLog::Combat(
        FString::Printf(TEXT("=== Combat Ended (Session %d) ==="), CurrentSessionID)
    );

    CurrentSessionID = -1;
    CurrentSessionName.Empty();
}
```

---

### Use Case 3: Turn System Logging
```cpp
void UKalkiTurnManager::StartTurn(AKalkiCharacter* Character, int32 TurnNumber)
{
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("=== Turn %d: %s ==="), TurnNumber, *Character->GetName())
    );

    // Log character state
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("HP: %d/%d, AP: %d"),
            Character->GetCurrentHP(),
            Character->GetMaxHP(),
            Character->GetActionPoints()
        )
    );

    // Verbose: Log all status effects
    if (bVerboseLogging)
    {
        KalkiLog::TurnSystem(
            FString::Printf(TEXT("Status Effects: %s"), *GetStatusEffectsString(Character)),
            EKalkiLogSeverity::Verbose
        );
    }
}

void UKalkiTurnManager::EndTurn(AKalkiCharacter* Character)
{
    KalkiLog::TurnSystem(
        FString::Printf(TEXT("%s ended turn"), *Character->GetName())
    );
}
```

---

### Use Case 4: D&D 5e Rules Logging
```cpp
void UKalkiRulesetManager::MakeAttackRoll(
    AKalkiCharacter* Attacker,
    AKalkiCharacter* Defender,
    bool bHasAdvantage,
    bool bHasDisadvantage
)
{
    // Roll d20
    int32 Roll1 = FMath::RandRange(1, 20);
    int32 Roll2 = 0;
    int32 FinalRoll = Roll1;

    // Handle advantage/disadvantage
    if (bHasAdvantage || bHasDisadvantage)
    {
        Roll2 = FMath::RandRange(1, 20);
        
        if (bHasAdvantage)
        {
            FinalRoll = FMath::Max(Roll1, Roll2);
            KalkiLog::Ruleset(
                FString::Printf(TEXT("Advantage: Rolled %d and %d, using %d"),
                    Roll1, Roll2, FinalRoll)
            );
        }
        else
        {
            FinalRoll = FMath::Min(Roll1, Roll2);
            KalkiLog::Ruleset(
                FString::Printf(TEXT("Disadvantage: Rolled %d and %d, using %d"),
                    Roll1, Roll2, FinalRoll)
            );
        }
    }
    else
    {
        KalkiLog::Ruleset(
            FString::Printf(TEXT("d20 Roll: %d"), FinalRoll)
        );
    }

    // Calculate total
    int32 AttackBonus = Attacker->GetAttackBonus();
    int32 Total = FinalRoll + AttackBonus;
    int32 AC = Defender->GetArmorClass();

    KalkiLog::Ruleset(
        FString::Printf(TEXT("Attack: %d + %d = %d vs AC %d"),
            FinalRoll, AttackBonus, Total, AC)
    );

    // Result
    if (FinalRoll == 20)
    {
        KalkiLog::Ruleset(TEXT("Natural 20 - Critical Hit!"));
    }
    else if (FinalRoll == 1)
    {
        KalkiLog::Ruleset(TEXT("Natural 1 - Critical Miss!"));
    }
    else if (Total >= AC)
    {
        KalkiLog::Ruleset(TEXT("Hit!"));
    }
    else
    {
        KalkiLog::Ruleset(TEXT("Miss!"));
    }
}
```

---

### Use Case 5: Movement with Pathfinding
```cpp
void AKalkiCharacter::MoveTo(const FKalkiGridCoord& Target)
{
    FKalkiGridCoord StartPos = GridPosition;

    KalkiLog::Movement(
        FString::Printf(TEXT("%s moving from (%d,%d) to (%d,%d)"),
            *GetName(), StartPos.X, StartPos.Y, Target.X, Target.Y),
        EKalkiLogSeverity::Log,
        this
    );

    // Find path
    TArray<FKalkiGridCoord> Path = Pathfinding->FindPath(StartPos, Target);

    if (Path.Num() == 0)
    {
        KalkiLog::Movement(
            FString::Printf(TEXT("%s: No path found to (%d,%d)"),
                *GetName(), Target.X, Target.Y),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    // Verbose: Log entire path
    if (bVerboseLogging)
    {
        FString PathString;
        for (const FKalkiGridCoord& Coord : Path)
        {
            PathString += FString::Printf(TEXT("(%d,%d) "), Coord.X, Coord.Y);
        }
        
        KalkiLog::Movement(
            FString::Printf(TEXT("Path: %s"), *PathString),
            EKalkiLogSeverity::Verbose,
            this
        );
    }

    // Execute movement
    ExecuteMovement(Path);

    KalkiLog::Movement(
        FString::Printf(TEXT("%s arrived at (%d,%d)"),
            *GetName(), Target.X, Target.Y),
        EKalkiLogSeverity::Log,
        this
    );
}
```

---

### Use Case 6: Ability System with Metadata
```cpp
void UKalkiAbility::Cast(AKalkiCharacter* Caster, const FKalkiGridCoord& Target)
{
    // Create detailed log with metadata
    FKalkiLogMessage LogMsg(
        EKalkiLogChannel::Abilities,
        EKalkiLogSeverity::Log,
        FString::Printf(TEXT("%s cast %s"), *Caster->GetName(), *AbilityName)
    );

    // Add metadata
    LogMsg.Metadata.Add(TEXT("CasterName"), Caster->GetName());
    LogMsg.Metadata.Add(TEXT("AbilityName"), AbilityName);
    LogMsg.Metadata.Add(TEXT("TargetX"), FString::FromInt(Target.X));
    LogMsg.Metadata.Add(TEXT("TargetY"), FString::FromInt(Target.Y));
    LogMsg.Metadata.Add(TEXT("ManaCost"), FString::FromInt(ManaCost));
    LogMsg.Metadata.Add(TEXT("CooldownTurns"), FString::FromInt(CooldownTurns));
    
    // Set source
    LogMsg.Source = Caster;

    // Log it
    LoggingSystem->LogMessage(LogMsg);

    // Apply ability effects
    ApplyEffects(Caster, Target);
}
```

---

## Troubleshooting

### Logs Not Appearing

**Problem:** Logs don't show in Output Log

**Solutions:**
1. Check filter settings - may be filtered out
2. Verify channel name correct (`LogKalkiGrid`, not `LogGrid`)
3. Check severity filter (Warnings/Errors only?)
4. Ensure not in Shipping build
5. Check Output Log is open (Window → Developer Tools → Output Log)
6. Verify Verbose logs aren't being filtered

---

### Too Many Logs

**Problem:** Output Log is spammed

**Solutions:**
1. Use channel filters to focus on specific systems
2. Remove per-frame logging
3. Add conditional compilation (`#if !UE_BUILD_SHIPPING`)
4. Increase severity threshold (filter out Verbose/Log)
5. Add runtime debug flags
6. Disable verbose logging in production

---

### Context Not Showing

**Problem:** Context object name not in log

**Solutions:**
1. Verify passing `this` as third parameter
2. Check object is valid (`IsValid(this)`)
3. Ensure object has valid name (`GetName()` returns non-empty)
4. Try passing explicit name instead of object

---

### Combat Session Not Tracked

**Problem:** Logs not grouped by combat session

**Solutions:**
1. Verify `CurrentSessionID` is set when combat starts
2. Check session ID is passed to log system
3. Ensure session ID cleared when combat ends
4. Verify combat log channel is used

---

## Related Documents

### Foundation
- [[03_Systems/Logging_System]] - Architecture and patterns
- [[04_Implementation/Error_Handling]] - When to log errors
- [[02_Conventions/Coding_Standards]] - Logging standards

### Examples
- [[03_Systems/Grid_System]] - Grid logging examples
- [[03_Systems/Combat_Log_System]] - User-facing logging
- [[04_Implementation/Common_Patterns]] - Logging patterns

### API References
- [[05_API_Reference/KalkiGridManager_API]] - Grid system API
- [[05_API_Reference/KalkiCombatLogViewModel_API]] - Combat log API

---

## Quick Reference Card
```cpp
// Include
#include "Logging/KalkiLog.h"
#include "Logging/KalkiLogTypes.h"

// Basic usage
KalkiLog::Grid(TEXT("Message"));

// With severity
KalkiLog::Combat(TEXT("Message"), EKalkiLogSeverity::Warning);

// With context
KalkiLog::Character(TEXT("Message"), EKalkiLogSeverity::Log, this);

// Verbose debug
KalkiLog::AI(TEXT("Debug details"), EKalkiLogSeverity::Verbose);

// With formatting
KalkiLog::System(
    FString::Printf(TEXT("Value: %d"), Value)
);

// Channels (14 total)
KalkiLog::System()      // Core systems
KalkiLog::Combat()      // Combat system
KalkiLog::Movement()    // Character movement
KalkiLog::Abilities()   // Ability system
KalkiLog::TurnSystem()  // Turn-based combat
KalkiLog::Grid()        // Grid system
KalkiLog::Dialogue()    // Dialogue system
KalkiLog::Inventory()   // Inventory system
KalkiLog::AI()          // AI system
KalkiLog::Network()     // Multiplayer
KalkiLog::Quest()       // Quest system
KalkiLog::Ruleset()     // D&D 5e rules
KalkiLog::Debug()       // Debug logging
KalkiLog::UI()          // User interface

// Severity (4 levels)
EKalkiLogSeverity::Verbose   // Detailed debug (gray)
EKalkiLogSeverity::Log       // Normal info (white)
EKalkiLogSeverity::Warning   // Caution (yellow)
EKalkiLogSeverity::Error     // Critical (red)

// FKalkiLogMessage properties
FDateTime Timestamp
EKalkiLogChannel Channel
EKalkiLogSeverity Severity
FString Message
TMap<FString, FString> Metadata
int32 CombatSessionID
FString CombatSessionName
TWeakObjectPtr<UObject> Source

// Helper methods
FString ToString(bool bTimestamp, bool bChannel)
FLinearColor GetColor()
```

---

## Document Info
**File Path:** `Kalki_GDD/05_API_Reference/KalkiLog_API.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 1  
**Complexity:** Beginner  
**Note:** Complete API reference for KalkiLog system (corrected with actual implementation)