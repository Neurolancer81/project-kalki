---
tags: [systems, ai, technical]
---
# Unified Tactical AI (Technical)

## Philosophy
One AI framework handles both enemies and auto-party behavior.  
It relies on **policy-based decision-making** via DataAssets and the **Behavior Tree system**.

---


## Core Components
| Element | Implementation |
|----------|----------------|
| **UTFA_TacticalAIComponent** | Attached to any controllable pawn. |
| **BT_TacticalDecision** | Shared Behavior Tree template. |
| **UTFA_TacticalPolicy** (DataAsset) | Defines preference weights. |
| **UTFA_ActionEvaluator** | Utility scoring for possible actions. |
| **Blackboard** | Stores tactical context (target, safe tile, etc.). |

---

## Behavior Tree Outline

```
Root  
├─ Service: EvaluateActions  
├─ Selector  
│ ├─ Sequence: UseBestAbility  
│ ├─ Sequence: MoveToSafety  
│ ├─ Sequence: Idle
```


---

## Policy DataAsset

```cpp
UTFA_TacticalPolicy  
{  
	AggressionWeight: 0.8  
	SelfPreservation: 0.2  
	PreferredRange: 600  
	PriorityTags: [Damage, Disable]  
}
```


AI reads this at runtime to weight possible moves.

---

## EvaluateActions Service
Iterates available abilities from the actor’s `AbilitySystemComponent`, scoring each with policy-modified utility.

**Pseudo-code:**
```cpp
void UBTService_EvaluateActions::Tick()
{
  for (Ability in Actor.Abilities)
  {
      score = Evaluate(Ability, Policy);
      if (score > BestScore) Best = Ability;
  }
  Blackboard->SetValue("BestAction", Best);
}
```


Player-AI Mode

When RTwP is active:

    Player characters temporarily adopt AI with PlayerPolicy.

    PlayerPolicy prioritizes fast execution, low deliberation.

Logging & Debug

    Every AI choice emits a DecisionMade event to the Journal.

    Includes reason weights, selected ability, and target ID.

    Dev overlay visualizes chosen actions (colored markers).

See also → [[Combat System (Technical)]], [[Journal System (Technical)]], [[Rules Adapter (Technical)]].


