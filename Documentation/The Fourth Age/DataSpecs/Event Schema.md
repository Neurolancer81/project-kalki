---
tags: [data, journal, schema]
---

# Event Schema

## Purpose
Defines the structure of `FTFA_LogEvent` used in JaSoT.

---

## Fields
| Field | Type | Description |
|--------|------|-------------|
| `EventId` | Guid | Unique ID. |
| `EventType` | Enum | Combat, Quest, Dialogue, etc. |
| `GameplayTags` | TagContainer | Classification tags. |
| `Timestamp` | Float | World time. |
| `TurnIndex` | Int | Combat context. |
| `SourceRef` | SoftObjectPtr | Source actor. |
| `TargetRefs[]` | Array | Affected actors. |
| `Values{}` | Map | Numeric payload. |
| `TextTokens[]` | Array | UI text placeholders. |
| `SeedBefore/After` | Int | RNG info. |
| `ContextIds` | Struct | Quest, Encounter, Map, etc. |

---

## Example
```json
{
  "EventId": "uuid",
  "EventType": "DamageApplied",
  "GameplayTags": ["Event.Damage.Fire"],
  "SourceRef": "Wizard_Player",
  "TargetRefs": ["Enemy_Bandit"],
  "Values": { "Damage": 12 },
  "SeedBefore": 234,
  "SeedAfter": 235
}
```

