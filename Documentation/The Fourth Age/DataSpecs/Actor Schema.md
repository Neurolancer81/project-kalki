---
tags: [data, actor, schema]
---

# Actor Schema

## Purpose
Defines attributes, policies, and inventory for characters, companions, and enemies.

---

## Fields
| Field | Type | Description |
|--------|------|-------------|
| `ActorId` | FName | Unique identifier. |
| `Name` | Text | Display name. |
| `FactionId` | FName | Reference to faction. |
| `Attributes` | Struct | Base stats (HP, STR, DEX, WIS, etc.). |
| `Abilities` | Array | List of AbilityIds. |
| `Statuses` | Array | Active status IDs. |
| `Inventory` | Array | ItemIds. |
| `AI_Policy` | UTFA_TacticalPolicy | Decision weights. |

---

## Example JSON
```json
{
  "ActorId": "Enemy_Bandit",
  "Name": "Bandit",
  "FactionId": "Faction_Bandits",
  "Attributes": { "HP": 20, "STR": 12, "DEX": 14 },
  "Abilities": ["MeleeAttack", "ThrowDagger"]
}```

See also → [[Unified Tactical AI (Technical)]], [[Combat System (Technical)]].
```