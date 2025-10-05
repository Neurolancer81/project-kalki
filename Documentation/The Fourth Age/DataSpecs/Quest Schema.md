---
tags: [data, quest, schema]
---

# Quest Schema

## Purpose
Defines quests as collections of event-driven objectives.

---

## Fields
| Field | Type | Description |
|--------|------|-------------|
| `QuestId` | FName | Unique ID. |
| `Title` | Text | Quest name. |
| `Description` | Text | Overview text. |
| `Objectives` | Array | List of sub-objectives. |
| `Rewards` | Array | XP, items, reputation changes. |
| `Prerequisites` | Array | Required events or quests. |

---

## Objective Format

```cpp
ObjectiveId: string  
Description: string  
Condition: Expression (EventType == "ItemAcquired" && ItemId == "Relic01")
```


---

## Example
```json
{
  "QuestId": "RelicRecovery",
  "Objectives": [
    { "Description": "Retrieve the relic", "Condition": "EventType == ItemAcquired && ItemId == Relic01" }
  ],
  "Rewards": ["XP_500", "Item_Relic01"]
}
```

See also → [[Dialogue and Quest System]], [[Journal System (Technical)]].

