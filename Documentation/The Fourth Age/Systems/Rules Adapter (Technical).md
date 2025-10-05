---
tags: [systems, rules, technical]
---

# Rules Adapter (Technical)

## Purpose
Provide a modular formula system to abstract various tabletop-style rulesets (5e, PF2e, custom).

---

## Architecture
| Element | Type | Description |
|----------|------|-------------|
| **UTFA_RulesAdapter** | Blueprintable class | Hosts formula methods. |
| **UDataAsset_RulePack** | DataAsset | Stores formula identifiers. |
| **UAttributeSet_Stats** | GAS class | Core attributes. |

---

## Example Functions
```cpp
int URulesAdapter::RollCheck(ECheckType Type, Actor* Source)
{
    int Roll = RNG.D20();
    return Roll + Source->GetModifier(Type);
}

float URulesAdapter::ApplyDamage(float Base, Actor* Attacker, Actor* Target)
{
    float Reduction = Target->GetResistance(DamageType);
    return Base * (1 - Reduction);
}
```

All random rolls use the deterministic RNG seeded in the Journal.

---

## Data-driven formulas

Each `RulePack` DataAsset defines overrides for key functions:
```python
RulePack_5e:
  HitFormula: "RollCheck + STR + Proficiency"
  DamageFormula: "Base * (1 - Target.Resistance)"

```
Parsed by the Rules Adapter at runtime.

---

## Integration

- GAS references `RulesAdapter` for hit, save, damage.
    
- Combat logs output raw roll + modifiers.
    
- Easy swapping between rule sets in campaign options.
    

See also → [[Combat System (Technical)]], [[Journal System (Technical)]].