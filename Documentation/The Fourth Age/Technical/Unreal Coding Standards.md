---
tags: [technical, coding, standards, ue5.6]
title: Unreal Coding Standards — Kalki Project
---

# Unreal Coding Standards (UE 5.6) — **Kalki: The Fourth Age**

> This document defines **project-wide coding and naming practices** aligned with Unreal Engine 5.6 conventions.  
> It supersedes any earlier prefixes and ensures consistency across C++, Blueprints, assets, folders, and Gameplay Tags.

## 0) Project Prefix & Module Names
- **Project/Class prefix:** `Kalki` (e.g., `AKalkiGameMode`, `UKalkiTurnManager`), **not** `TFA_`.
- **Modules:** `Kalki` (runtime), `KalkiEditor` (editor tools), optional `KalkiDev` (internal experiments).
- **Gameplay Tags root:** `Kalki.` (e.g., `Kalki.Event.Damage.Fire`).

See also → [[Technical/Subsystem Architecture]], [[Technical/Class Architecture]], [[Systems/Journal System (Technical)]].

---

## 1) C++ Types & Files (Unreal style)

### 1.1 Class & type prefixes
| Type | Prefix | Example |
|------|--------|---------|
| UObject-derived | `U` | `UKalkiTurnManager` |
| Actor | `A` | `AKalkiCharacter` |
| Actor Component | `U` | `UKalkiTacticalAIComponent` |
| Struct | `F` | `FKalkiLogEvent` |
| Enum | `E` | `EKalkiDamageType` |
| Interface (native) | `U` + `I` | `UKalkiInteractable` + `IKalkiInteractable` |

**No underscores in class names.** Use PascalCase.

### 1.2 Files & folders

- **Header/Source file names match class names** exactly: `KalkiTurnManager.h/.cpp`.
- **One public class per file.**
- **Folders** mirror modules and subsystems:
```
/Source/Kalki/  
Core/ (GameMode, GameState, PlayerController, PlayerState)  
Systems/ (Combat, Journal, Exploration, Overworld, UI)  
AI/  
Rules/  
Data/  
DevTools/

```

### 1.3 Functions & variables
- **Functions:** `PascalCase` → `StartEncounter()`, `AdvanceTurn()`
- **UPROPERTY members:** `PascalCase` (Unreal style).  
- **Booleans:** prefix `b` + `PascalCase` → `bIsAlive`, `bRTwPEnabled`
- **Locals/params:** `camelCase` preferred for readability → `rollValue`, `bestScore`
- **Enums:** `enum class EThing { Thing_A, Thing_B }` with **values prefixed** by the enum name when useful.
- **Constants/Macros:** `ALL_CAPS_SNAKE_CASE`

**Example:**
```cpp
UCLASS()
class KALKI_API UKalkiTurnManager : public UObject
{
  GENERATED_BODY()

public:
  void AdvanceTurn();

private:
  UPROPERTY()
  TArray<TWeakObjectPtr<AActor>> InitiativeOrder;

  UPROPERTY()
  int32 CurrentIndex = 0;

  UPROPERTY()
  bool bEncounterActive = false;
};
```

## 2) Blueprint & Asset Naming

Use **categorical prefixes** + descriptive names. Keep names **short, searchable, and unique**.

### 2.1 Blueprints / UI / Levels

|Asset Type|Prefix|Example|
|---|---|---|
|Blueprint (Gameplay)|`BP_`|`BP_Character_Player`, `BP_Enemy_Bandit`|
|Widget Blueprint|`WBP_`|`WBP_CombatHUD`, `WBP_Journal`|
|Animation Montage|`AM_`|`AM_Sword_AttackA`|
|Animation Sequence|`AN_`|`AN_Idle_Loop`|
|Level / Map|`L_`|`L_Hub_Town`, `L_Arena_Forest`|
|Level Streaming Layer|`LS_`|`LS_Town_Interiors`|
|Data Table|`DT_`|`DT_Abilities`, `DT_Quests`|
|Curve (Float/Vector)|`Curve_Float_` / `Curve_Vector_`|`Curve_Float_DamageFalloff`|