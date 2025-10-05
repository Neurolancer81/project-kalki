---
tags: [data, ability, schema]
---

# Ability Schema

## Purpose
Defines the structure for abilities and powers, stored as **DataAssets** and referenced by the Gameplay Ability System (GAS).

---

## Fields
| Field | Type | Description |
|--------|------|-------------|
| `AbilityId` | FName | Unique identifier. |
| `DisplayName` | Text | Localized name. |
| `Description` | Text | Summary for tooltips. |
| `Tags` | GameplayTagContainer | Categories (e.g., Ability.Attack.Melee). |
| `Icon` | Texture2D | UI icon. |
| `Range` | Float | Max range in cm. |
| `TargetingMode` | Enum | Self / Ally / Enemy / AoE. |
| `BaseCost` | Float | Resource cost. |
| `Cooldown` | Float | Turns or seconds. |
| `RuleFormula` | FString | Math expression (used by Rules Adapter). |
| `Animation` | AnimMontage | Associated animation. |
| `Effect` | GameplayEffect | Buff/debuff reference. |
| `SoundCue` | SoundBase | SFX when cast. |
| `Particle` | NiagaraSystem | Optional VFX. |

---

## Example JSON
```json
{
  "AbilityId": "Firebolt",
  "DisplayName": "Firebolt",
  "Tags": ["Ability.Attack.Fire"],
  "Range": 900,
  "Cooldown": 1,
  "RuleFormula": "RollD20 + IntMod vs Target.AC"
}
```

See also → [[Rules Adapter (Technical)]], [[Journal System (Technical)]].