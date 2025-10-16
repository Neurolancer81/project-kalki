---
tags:
  - kalki/system/logging
  - kalki/debugging
  - kalki/phase-1
status: complete
phase: 1
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Logging System

## Overview
The Logging System provides centralized, channel-based logging for the entire Kalki project. It wraps Unreal Engine's logging system with a clean, project-specific API that supports multiple channels, severity levels, and context tracking.

---

## Purpose

### Problems Solved
1. **Scattered Logging** - No consistent logging approach across codebase
2. **Hard to Filter** - Can't easily filter logs by system (Grid vs Combat vs UI)
3. **No Context** - Difficult to know which object logged what
4. **Verbose Syntax** - Unreal's UE_LOG is verbose and error-prone

### Design Goals
- ✅ **Simple API** - Easy to use, hard to misuse
- ✅ **Channel-Based** - Filter logs by system (Grid, Combat, UI, etc.)
- ✅ **Severity Levels** - Log, Warning, Error
- ✅ **Context Tracking** - Optional object context
- ✅ **Zero Runtime Cost** - Compiled out in shipping builds

---

## Architecture

### Static Utility Pattern
```cpp
class KalkiLog
{
public:
    // Static logging functions - no instance needed
    static void Log(EKalkiLogChannel Channel, const FString& Message, 
                   EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
                   const UObject* Context = nullptr);
    
    // Convenience functions per channel
    static void Grid(const FString& Message, 
                    EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
                    const UObject* Context = nullptr);
    
    static void Combat(const FString& Message, 
                      EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
                      const UObject* Context = nullptr);
    
    // ... one function per channel
};
```

**Why Static:**
- No initialization needed
- Global access from anywhere
- No memory overhead
- Clean call sites

---

## Core Enums

### EKalkiLogChannel

**Purpose:** Define logging channels for filtering
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

**When to Add New Channels:**
- Adding a major system (e.g., Inventory, Dialogue)
- Need to filter logs for specific feature
- System is complex enough to warrant separate logs

**Current Channels:**

**General** - Misc logs that don't fit elsewhere
**Combat** - Combat system, damage, abilities
**System** - Core systems, initialization, lifecycle
**Grid** - Grid operations, pathfinding, spatial queries
**UI** - Widget updates, input handling, HUD
**Ability** - Ability casting, targeting, effects (future)
**Character** - Character stats, progression (future)
**AI** - Enemy AI decisions, behavior trees (future)
**Network** - Multiplayer, replication, RPCs (future)

---

### EKalkiLogSeverity

**Purpose:** Define log importance
```cpp
UENUM(BlueprintType)  
enum class EKalkiLogSeverity : uint8  
{  
    Verbose     UMETA(DisplayName = "Verbose"),  // Detailed debug info  
    Log         UMETA(DisplayName = "Log"),      // Normal information  
    Warning     UMETA(DisplayName = "Warning"),  // Something unusual  
    Error       UMETA(DisplayName = "Error")     // Something went wrong  
};
```

**When to Use Each:**

**Log** - Normal operation
```cpp
KalkiLog::Grid(TEXT("Grid created: 20x20"));
KalkiLog::Combat(TEXT("Character took 10 damage"));
```

**Warning** - Something unexpected but not critical
```cpp
KalkiLog::Grid(TEXT("Invalid coord, skipping"), EKalkiLogSeverity::Warning);
KalkiLog::UI(TEXT("Widget not found, using default"), EKalkiLogSeverity::Warning);
```

**Error** - Critical problem
```cpp
KalkiLog::Grid(TEXT("GridManager is null!"), EKalkiLogSeverity::Error);
KalkiLog::System(TEXT("Failed to initialize combat system"), EKalkiLogSeverity::Error);
```

---

## Usage Examples

### Basic Logging
```cpp
// Simple log
KalkiLog::Grid(TEXT("Grid initialized"));

// With severity
KalkiLog::Grid(TEXT("Invalid coordinate"), EKalkiLogSeverity::Warning);

// With context (shows which object logged)
KalkiLog::Grid(TEXT("Tile updated"), EKalkiLogSeverity::Log, this);
```

---

### Formatted Messages
```cpp
// Using FString::Printf
KalkiLog::Grid(
    FString::Printf(TEXT("Grid created: %dx%d tiles"), SizeX, SizeY)
);

KalkiLog::Combat(
    FString::Printf(TEXT("Character took %d damage from %s"), 
        DamageAmount, 
        *DamageSource->GetName())
);

// Multiple values
KalkiLog::Grid(
    FString::Printf(TEXT("Tile (%d, %d) elevation set to %.1f"), 
        Coord.X, Coord.Y, Elevation)
);
```

---

### Conditional Logging
```cpp
// Only log in development builds
#if !UE_BUILD_SHIPPING
    KalkiLog::Grid(TEXT("Debug: Pathfinding took 5ms"));
#endif

// Log based on condition
if (bDebugMode)
{
    KalkiLog::AI(TEXT("Enemy targeting player"));
}

// Early return with error log
if (!IsValid(Character))
{
    KalkiLog::Combat(TEXT("Character is invalid"), EKalkiLogSeverity::Error);
    return;
}
```

---

### Logging in Different Contexts

**In Subsystems:**
```cpp
void UKalkiGridManager::CreateGrid(int32 SizeX, int32 SizeY)
{
    KalkiLog::Grid(
        FString::Printf(TEXT("Creating grid: %dx%d"), SizeX, SizeY),
        EKalkiLogSeverity::Log,
        this  // Pass 'this' for context
    );
    
    // ... create grid ...
    
    KalkiLog::Grid(TEXT("Grid created successfully"));
}
```

**In Actors:**
```cpp
void AKalkiCharacter::TakeDamage(float DamageAmount)
{
    KalkiLog::Combat(
        FString::Printf(TEXT("%s took %.1f damage"), *GetName(), DamageAmount),
        EKalkiLogSeverity::Log,
        this
    );
}
```

**In Widgets:**
```cpp
void UKalkiCombatLogWidget::AddMessage(const FString& Message)
{
    KalkiLog::UI(
        FString::Printf(TEXT("Adding message: %s"), *Message),
        EKalkiLogSeverity::Log,
        this
    );
}
```

---

## Implementation

### KalkiLog.h
```cpp
// Public/Logging/KalkiLog.h  
  
#pragma once  
  
#include "CoreMinimal.h"  
#include "Logging/KalkiLogTypes.h"  
  
class UKalkiLogSubsystem;  
  
/**  
 * Static helper functions for convenient logging throughout the codebase * Usage: KalkiLog::Combat(TEXT("Arjuna attacks Goblin")); */namespace KalkiLog  
{  
    // Get the log subsystem (helper function)  
    KALKI_API UKalkiLogSubsystem* GetLogSubsystem(const UObject* WorldContextObject);  
  
    // Channel-specific logging functions  
    KALKI_API void System(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Combat(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Movement(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Abilities(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void TurnSystem(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Grid(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Dialogue(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Inventory(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void AI(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Network(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Quest(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Ruleset(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
    KALKI_API void Debug(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Verbose, const UObject* WorldContextObject = nullptr);  
  
    KALKI_API void UI(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);  
  
    // Advanced: Log with full message struct (for metadata)  
    KALKI_API void LogMessage(const FKalkiLogMessage& Message, const UObject* WorldContextObject = nullptr);  
  
    // Utility: Log with metadata helpers  
    KALKI_API void CombatWithMetadata(  
        const FString& Message,   
const TMap<FString, FString>& Metadata,  
        EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,  
        const UObject* WorldContextObject = nullptr);  
  
    KALKI_API void MovementWithMetadata(  
        const FString& Message,   
const TMap<FString, FString>& Metadata,  
        EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,  
        const UObject* WorldContextObject = nullptr);  
}
```

---

### KalkiLog.cpp
```cpp

// Private/Logging/KalkiLog.cpp  
  
#include "Logging/KalkiLog.h"  
#include "Logging/KalkiLogSubsystem.h"  
#include "Engine/World.h"  
#include "Engine/Engine.h"  
  
namespace KalkiLog  
{  
    // Private helper function  
    static void LogToChannel(  
        EKalkiLogChannel Channel,   
const FString& Message,   
EKalkiLogSeverity Severity,   
const UObject* WorldContextObject)  
    {        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);  
        if (LogSubsystem)  
        {            LogSubsystem->Log(Channel, Severity, Message);  
        }    }  
    UKalkiLogSubsystem* GetLogSubsystem(const UObject* WorldContextObject)  
    {        // Try to get world from context object  
        UWorld* World = nullptr;  
        if (WorldContextObject)  
        {            World = WorldContextObject->GetWorld();  
        }        // Fallback: try to get from game instance  
        if (!World && GEngine)  
        {            // Try to get first valid world  
            for (const FWorldContext& Context : GEngine->GetWorldContexts())  
            {                if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)  
                {                    World = Context.World();  
                    break;  
                }            }        }  
        if (!World)  
        {            return nullptr;  
        }  
        return World->GetSubsystem<UKalkiLogSubsystem>();  
    }  
    // Now all channel functions are one-liners  
  
    void System(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::System, Message, Severity, WorldContextObject);  
    }  
    void Combat(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Combat, Message, Severity, WorldContextObject);  
    }  
    void Movement(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Movement, Message, Severity, WorldContextObject);  
    }  
    void Abilities(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Abilities, Message, Severity, WorldContextObject);  
    }  
    void TurnSystem(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::TurnSystem, Message, Severity, WorldContextObject);  
    }  
    void Grid(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Grid, Message, Severity, WorldContextObject);  
    }  
    void Dialogue(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Dialogue, Message, Severity, WorldContextObject);  
    }  
    void Inventory(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Inventory, Message, Severity, WorldContextObject);  
    }  
    void AI(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::AI, Message, Severity, WorldContextObject);  
    }  
    void Network(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Network, Message, Severity, WorldContextObject);  
    }  
    void Quest(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Quest, Message, Severity, WorldContextObject);  
    }  
    void Ruleset(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Ruleset, Message, Severity, WorldContextObject);  
    }  
    void Debug(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::Debug, Message, Severity, WorldContextObject);  
    }  
    void UI(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)  
    {        LogToChannel(EKalkiLogChannel::UI, Message, Severity, WorldContextObject);  
    }
    
  
    void LogMessage(const FKalkiLogMessage& Message, const UObject* WorldContextObject)  
    {        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);  
        if (LogSubsystem)  
        {            LogSubsystem->LogMessage(Message);  
        }    }  
    void CombatWithMetadata(  
        const FString& Message,   
const TMap<FString, FString>& Metadata,  
        EKalkiLogSeverity Severity,  
        const UObject* WorldContextObject)  
    {        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);  
        if (LogSubsystem)  
        {            FKalkiLogMessage LogMsg(EKalkiLogChannel::Combat, Severity, Message);  
            LogMsg.Metadata = Metadata;  
            LogSubsystem->LogMessage(LogMsg);  
        }    }  
    void MovementWithMetadata(  
        const FString& Message,   
const TMap<FString, FString>& Metadata,  
        EKalkiLogSeverity Severity,  
        const UObject* WorldContextObject)  
    {        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);  
        if (LogSubsystem)  
        {            FKalkiLogMessage LogMsg(EKalkiLogChannel::Movement, Severity, Message);  
            LogMsg.Metadata = Metadata;  
            LogSubsystem->LogMessage(LogMsg);  
        }    }}
```

---

## Filtering Logs

### In Editor Output Log

**Filter by channel:**
```
LogKalkiGrid: Grid created: 20x20
LogKalkiCombat: Character took damage
LogKalkiUI: Widget initialized
```

**In Output Log window:**
1. Type filter: `LogKalkiGrid`
2. Only shows Grid logs

**Filter by severity:**
1. Warning button - Shows only warnings and errors
2. Error button - Shows only errors

---

### In Code

**Conditional compilation:**
```cpp
// Only log in Development and DebugGame builds
#if !UE_BUILD_SHIPPING
    KalkiLog::Grid(TEXT("Debug information"));
#endif

// Only in Debug builds
#if UE_BUILD_DEBUG
    KalkiLog::AI(TEXT("Detailed AI state"));
#endif
```

---

## Performance

### Runtime Cost

**Development/Debug Builds:**
- Log call: ~0.01-0.1ms (negligible)
- String formatting: ~0.1-1ms (depends on complexity)

**Shipping Builds:**
- **Zero cost** - All UE_LOG calls compiled out
- KalkiLog functions still exist but do nothing

---

### Best Practices

**✅ Do This:**
```cpp
// Log important state changes
KalkiLog::Grid(TEXT("Grid initialized"));

// Log errors
KalkiLog::System(TEXT("Failed to load config"), EKalkiLogSeverity::Error);

// Log with context
KalkiLog::Combat(TEXT("Ability cast"), EKalkiLogSeverity::Log, this);
```

**❌ Don't Do This:**
```cpp
// Don't log every frame
void Tick(float DeltaTime)
{
    KalkiLog::Grid(TEXT("Tick"));  // Too spammy!
}

// Don't log trivial operations
int32 Result = A + B;  // Don't log this

// Don't log in tight loops
for (int32 i = 0; i < 10000; ++i)
{
    KalkiLog::Grid(TEXT("Loop"));  // 10,000 logs!
}
```

---

## Testing

### Verify Logging Works
```cpp
// In any class
void TestLogging()
{
    // Test all severities
    KalkiLog::Grid(TEXT("Normal log"));
    KalkiLog::Grid(TEXT("Warning log"), EKalkiLogSeverity::Warning);
    KalkiLog::Grid(TEXT("Error log"), EKalkiLogSeverity::Error);
    
    // Test with context
    KalkiLog::Grid(TEXT("With context"), EKalkiLogSeverity::Log, this);
    
    // Test formatting
    KalkiLog::Grid(FString::Printf(TEXT("Value: %d"), 42));
}
```

**Expected Output:**
```
LogKalkiGrid: Normal log
LogKalkiGrid: Warning: Warning log
LogKalkiGrid: Error: Error log
LogKalkiGrid: [ObjectName] With context
LogKalkiGrid: Value: 42
```

---

## Future Enhancements

### Potential Additions

**1. File Logging**
```cpp
// Write logs to file for bug reports
KalkiLog::WriteToFile(TEXT("Logs/Session.log"));
```

**2. Log Levels (Verbosity)**
```cpp
enum class EKalkiLogLevel
{
    Verbose,   // Everything
    Normal,    // Standard logs
    Minimal    // Warnings and errors only
};
```

**3. Log Filtering at Runtime**
```cpp
// Disable specific channels
KalkiLog::SetChannelEnabled(EKalkiLogChannel::AI, false);
```

**4. Blueprint Logging**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
static void LogGrid(const FString& Message);
```

---

## Related Documents

### Foundation
- [[02_Conventions/Coding_Standards]] - How to use logging properly
- [[04_Implementation/Error_Handling]] - When to log errors

### Systems
- [[03_Systems/Combat_Log_System]] - User-facing combat log (different system)
- [[03_Systems/Grid_System]] - Example of KalkiLog usage

### API Reference
- [[05_API_Reference/KalkiLog_API]] - Complete API documentation

### Architecture
- [[01_Architecture/Module_Overview]] - Logging subsystem overview

---

## Quick Reference

### Common Patterns

**System initialization:**
```cpp
KalkiLog::System(TEXT("GridManager initialized"));
```

**Error handling:**
```cpp
if (!IsValid(Object))
{
    KalkiLog::System(TEXT("Object invalid"), EKalkiLogSeverity::Error, this);
    return;
}
```

**State changes:**
```cpp
KalkiLog::Grid(FString::Printf(TEXT("Tile (%d,%d) elevation: %.1f"), X, Y, Elev));
```

**Warnings:**
```cpp
KalkiLog::Combat(TEXT("Target out of range"), EKalkiLogSeverity::Warning);
```

---

## Document Info
**File Path:** `Kalki_GDD/03_Systems/Logging_System.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 1  
**Complexity:** Beginner