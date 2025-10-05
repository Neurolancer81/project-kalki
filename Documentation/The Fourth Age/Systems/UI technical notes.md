---
tags: [systems, ui, technical]
---

# UI Technical Notes

## Framework
All UIs built with **UMG / CommonUI** + DataBindings.

---

## Major Widgets
| Widget | Function |
|---------|-----------|
| `WBP_CombatHUD` | Initiative bar, action bar, pause toggle. |
| `WBP_CombatLog` | Scrollable combat history. |
| `WBP_Journal` | Quest and timeline entries. |
| `WBP_TooltipRenderer` | Hover info from GameplayTags. |

---

## Tooltip System
- `UTFA_TooltipRegistry` maps tag → widget.  
- Hovering over log entries spawns contextual tooltips.

**Example:**
Tag: Event.Damage.Fire  
Tooltip: "Fire damage reduces armor by 10%"