---
tags: [data, item, schema]
---

# Item Schema

## Purpose


Defines all in-game items, weapons, armor, consumables, and relics.

---

## Fields
| Field | Type | Description |
|--------|------|-------------|
| `ItemId` | FName | Unique identifier. |
| `Name` | Text | Display name. |
| `Tags` | GameplayTagContainer | Item classification. |
| `Description` | Text | Tooltip. |
| `Value` | Int | Gold cost. |
| `Weight` | Float | Encumbrance. |
| `EquipSlot` | Enum | Weapon, Armor, Accessory. |
| `Modifiers` | Map | Stat bonuses. |
| `AbilityGranted` | AbilityId | Optional granted ability. |

---

## Example
```json
{
  "ItemId": "IronSword",
  "Tags": ["Item.Weapon.Melee"],
  "Value": 50,
  "EquipSlot": "Weapon",
  "Modifiers": { "STR": 1 }
}
```
