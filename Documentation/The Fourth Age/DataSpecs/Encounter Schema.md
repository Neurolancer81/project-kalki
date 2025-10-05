---
tags: [data, encounter, schema]
---

# Encounter Schema

## Purpose
Defines spawn layout, environment, and seed for combat encounters.

---

## Fields
| Field | Type | Description |
|--------|------|-------------|
| `EncounterId` | FName | Unique identifier. |
| `Map` | SoftObjectPath | Level reference. |
| `Actors` | Array | Spawned ActorIds and positions. |
| `Seed` | Int | RNG seed. |
| `Trigger` | Enum | Manual / Auto / Random. |

---

## Example
```json
{
  "EncounterId": "BanditAmbush",
  "Map": "/Game/Maps/Forest_Arena",
  "Actors": ["Enemy_Bandit", "Enemy_BanditLeader"],
  "Seed": 12345
}

See also → [[Combat System (Technical)]], [[Unified Tactical AI (Technical)]].