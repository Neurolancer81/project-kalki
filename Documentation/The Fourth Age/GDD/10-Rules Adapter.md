---
tags: [gdd, rules]
---

# Rules Adapter

## Purpose
A data-driven mathematical layer abstracting the RPG ruleset so different systems (5e-like, PF2e-like, custom) can coexist.

## Structure
| Component | Implementation |
|------------|----------------|
| **Attributes** | `UAttributeSet_Stats` (GAS) |
| **Formulas** | `UTFA_RulesAdapter` static math library |
| **Tags** | GameplayTags define categories (`Action.Attack`, `Damage.Fire`, etc.) |

## Example pseudo-code
```
int CalculateHitRoll(Actor attacker, Actor target)  
{  
	roll = RNG.D20()  
	return roll + attacker.ATT_Bonus - target.DEF  
}
```


## Extensibility
- Swap rule packs via DataAssets at campaign start.  
- All formulas referenced by ID from abilities.

See also → [[Gameplay Ability System Integration]], [[Combat System (Technical)]].
