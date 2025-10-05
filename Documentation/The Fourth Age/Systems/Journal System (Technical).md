---
tags: [systems, journal, technical]
---

# Journal System (Technical)

## Purpose
Implements **JaSoT (Journal-as-Source-of-Truth)** — the global event log and state backbone.

---

## Subsystem Layout

```
UTFA_GameInstanceSubsystem_Journal  
├─ UTFA_GameLogRouter  
├─ UTFA_EventStore  
├─ UTFA_SnapshotManager  
└─ UTFA_JournalView
```

---

### UTFA_GameLogRouter
Receives broadcast delegates from systems (Combat, Dialogue, Quests), enriches them, and writes standardized `FTFA_LogEvent`.

**Responsibilities**
- Assign `EventId`
- Enrich with turn index, quest/encounter IDs
- Dispatch to `EventStore`
- Notify subscribed UIs

---

### FTFA_LogEvent Struct
| Field | Type | Description |
|--------|------|-------------|
| `EventId` | Guid | Unique identifier |
| `EventType` | Enum | Category (Combat, Quest, Dialogue, System) |
| `GameplayTags` | TagContainer | Classification |
| `Timestamp` | Float | World time |
| `TurnIndex` | Int | Combat context |
| `SourceRef` | SoftObjectPtr | Source actor |
| `TargetRefs[]` | Array | Targets |
| `Values{}` | Map | Numeric data |
| `TextTokens[]` | Array | For UI formatting |

---

### EventStore
- In-memory ring buffer for current session.
- Disk chunks written periodically (`.tfaevt` files).
- Append-only, versioned.

### SnapshotManager
- Writes `Snapshot(N)` + `Events(after N)` pairs.
- Supports replay and crash recovery.

### JournalView
- Builds player-facing entries (timeline, quests, combat log).
- Hover tooltips from tag lookups.

---

### Multiplayer integration
- Server is authoritative writer.
- Clients receive filtered `EventBundle` updates.
- Late join → snapshot + delta replay.

---

### Debug & Tools
- Editor “Event Browser” widget filters by tag, actor, or turn.
- “Export Segment” dumps JSON for bug reports.

See also → [[Combat System (Technical)]], [[Unified Tactical AI (Technical)]].
