---
tags: [gdd, journal]
---

# Journal as Source of Truth (JaSoT)

## Core concept
Everything that happens in the game is logged as a **standardized event**.  
The Journal acts as both the **player journal** and the **internal state ledger**.

## Architecture
- **Producer Systems** → emit `LogEvent` delegates.  
- **GameLogRouter (GISubsystem)** → enriches & stores events.  
- **EventStore** → append-only memory/disk buffer.  
- **SnapshotManager** → periodic saves & replays.  
- **JournalView** → builds player-facing timeline.

## Event Structure (`FTFA_LogEvent`)
```EventId, SchemaVersion, EventType, GameplayTags[],  
Timestamp, TurnIndex, TickIndex,  
SourceRef, TargetRefs[],  
Numbers{}, Outcome, TextTokens[],  
SeedBefore, SeedAfter, RelatedIds[],  
VisibilityMask, ContextIds (Quest, Encounter, etc.)
```


## Example flow
1. Wizard casts Firebolt.  
2. GAS resolves → `OnAbilityResolved` delegate fires.  
3. Router enriches → EventStore writes.  
4. CombatLog UI displays formatted text.  
5. Journal UI archives for story continuity.

See also → [[Journal System (Technical)]], [[Combat System (Technical)]].
