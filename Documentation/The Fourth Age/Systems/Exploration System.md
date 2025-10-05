---
tags: [systems, exploration]
---

# Exploration System

## Overview
Third-person exploration with interactables, perception checks, and environmental events logged to Journal.

---

## Key Classes
| Class | Role |
|-------|------|
| `UTFA_WorldSubsystem_Exploration` | Handles exploration mode transitions. |
| `ATFA_InteractableActor` | Base for objects and triggers. |
| `UTFA_PerceptionComponent` | Detects points of interest. |

---

## Flow
1. Player moves via NavMesh pathfinding.  
2. On overlap → trigger `InteractionStarted` event.  
3. Skill check (deterministic roll).  
4. Outcome logged (`InteractionSuccess/Failure`).  
5. Possible state change or reward.

---

## Pseudo-code
```cpp
void ATFA_InteractableActor::Interact(AActor* Interactor)
{
    bool success = RulesAdapter->SkillCheck("Perception", Interactor);
    Journal->LogEvent(success ? "InteractionSuccess" : "InteractionFailure");
}
```

---

Debugging

Use Highlight Mode overlay to visualize interactables and perception radii.

See also → [[Overworld System]], [[Journal System (Technical)]].
