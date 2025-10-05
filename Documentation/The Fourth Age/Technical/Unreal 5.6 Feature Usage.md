
---
tags: [technical, unreal, ue5.6]
---

# Unreal 5.6 Feature Usage

## Core Engine Systems
| Feature | Usage |
|----------|--------|
| **Lumen** | Global illumination for stylized realism. |
| **Nanite** | High-detail environment assets. |
| **World Partition** | Optional for exploration maps. |
| **Control Rig** | Companion animation & cutscenes. |
| **Sequencer** | Dialogue & narrative scenes. |
| **Enhanced Input** | Unified input mapping for modes. |
| **GAS** | Abilities, attributes, effects. |
| **Subsystems** | Modular runtime architecture. |

---

## Rendering Setup
- Default lit stylized materials.  
- Dynamic sky & fog for time-of-day.  
- Lumen reflections for interiors.

---

## Optimization Strategy
- Level Instancing for modular reuse.  
- Use Nanite only where needed.  
- Stream cinematics separately.

See also → [[Class Architecture]], [[Subsystem Architecture]].
