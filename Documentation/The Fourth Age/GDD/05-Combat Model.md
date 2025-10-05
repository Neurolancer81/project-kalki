---
tags: [gdd, combat]
---

# Combat Model — TB / RTwP Hybrid

## Core principles
- **Authoritative Turn-Based core**: deterministic initiative, no ambiguity.  
- **RTwP (single-player)**: global time dilation + Player-AI auto-turns.  
- **Unified AI** controls all combatants via policy profiles.

## Sequence
1. **Encounter start** → `EncounterStarted` event (seed logged).  
2. **Initiative order** determined via attribute rolls.  
3. **Turn cycle** → Ability selection → Execution → Resolution → Events logged.  
4. **RTwP mode** accelerates time; AI executes same cycle automatically.  
5. **End encounter** → Rewards, XP, `EncounterEnded` event.

## Pseudo-Flow
```
StartEncounter()  
seed = RNG.New()  
while not EncounterOver():  
current = TurnQueue.Next()  
current.TakeTurn()  
Journal.Log(Event_TurnEnded)  
EndEncounter()
```

## Key Systems
- [[TurnManager]] (WorldSubsystem)
- [[ActionQueue]]
- [[Deterministic RNG]]
- [[Unified Tactical AI (Technical)]]
- [[Gameplay Ability System Integration]]
- [[Journal as Source of Truth]]

See also → [[Combat System (Technical)]], [[Unified Tactical AI (Technical)]].
