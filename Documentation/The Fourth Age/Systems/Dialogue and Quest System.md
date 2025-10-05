---
tags: [systems, dialogue, quests]
---

# Dialogue and Quest System

## Dialogue
- Stored as `UDialogueDataAsset` with nodes and conditions.
- Sequencer cutscenes optionally linked to dialogue nodes.
- Skill checks handled via Rules Adapter.
- Every choice and outcome logs a Journal event.

**Event Types**
- `DialogueStarted`
- `DialogueChoice`
- `DialogueOutcome`

---

## Quests
Event-driven; no direct variable mutation.

| Component | Description |
|------------|-------------|
| `UTFA_QuestManager` | Listens to Journal events to update objectives. |
| `UQuestDataAsset` | Defines objectives and conditions. |
| `UTFA_JournalView` | Displays progress. |

**Example:**
```json
Objective: "Retrieve relic"
Condition: EventType == ItemAcquired && ItemId == Relic_01
```

When the condition is satisfied, the quest auto-updates.

---

## Debugging

Use Dev Quest Log viewer with filters per quest and objective.

See also → [[Journal System (Technical)]], [[Unified Tactical AI (Technical)]].