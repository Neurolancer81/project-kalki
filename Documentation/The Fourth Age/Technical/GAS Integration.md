---
tags: [technical, gas, ue5.6]
---

# Gameplay Ability System (GAS) Integration

## Overview
GAS provides attribute, ability, and effect management for combat.  
All abilities and effects must **emit Journal events** to maintain determinism.

---

## Core Components
| Class | Role |
|--------|------|
| `UAbilitySystemComponent` | Central ability executor on each actor. |
| `UGameplayAbility_TFA` | Base class for all abilities (extends GAS Ability). |
| `UAttributeSet_Stats` | Primary attributes (HP, STR, DEX, WIS, etc.). |
| `UGameplayEffect_TFA` | Buffs/Debuffs with metadata. |

---

## Ability Lifecycle

```
InputPressed → CanActivate? → ActivateAbility → Execute → CommitCost → ApplyEffect → EndAbility
```


At each step, the ability fires a delegate captured by the Journal Router.

---

## Example Base Class (pseudo-code)
```cpp
void UGameplayAbility_TFA::OnAbilityExecuted(const FGameplayAbilityTargetDataHandle& Targets)
{
    FTFA_LogEvent E;
    E.EventType = EEventType::AbilityActivated;
    E.SourceRef = GetAvatarActor();
    E.TargetRefs = Targets.Actors;
    Journal->Log(E);
}
```

---

## Attribute Management

Attributes are defined in `UAttributeSet_Stats`:
```
Health, Mana, Strength, Dexterity, Wisdom, Initiative, ArmorClass
```

Each change triggers `OnAttributeChanged` → Journal `AttributeModified` event.

---

## Gameplay Tags

- Define every ability and effect category.
    
- Example: `Ability.Attack.Melee`, `Status.Buff.Shield`, `Event.Heal`.
    
- Tags drive UI color coding and filtering in the Journal.

---

## Determinism

- Use **UTFA_DeterministicRNG** for all roll logic.
    
- Pass seed and result to Journal for replay fidelity.
    

See also → [[Combat System (Technical)]], [[Rules Adapter (Technical)]].
