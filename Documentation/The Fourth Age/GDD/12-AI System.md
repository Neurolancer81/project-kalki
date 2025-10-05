---
tags: [gdd, ai]
---

# Unified Tactical AI

## Philosophy
A **single tactical AI** framework controls both enemy and auto-party behavior.

## Components
- Behavior Tree with policy weights (`UTFA_TacticalPolicy` DataAssets).  
- Shared services: Evaluate Actions, Select Target, Safety Check.  
- Blackboard keys: `CurrentTarget`, `BestAction`, `BestMoveTile`, etc.

## Policy Profiles
| Profile | Behavior |
|----------|-----------|
| Enemy-Aggressive | Focus nearest threat |
| Enemy-Cautious | Maintain distance, protect caster |
| PlayerAI-Aggressive | Close in, prefer AoE |
| PlayerAI-Support | Heal, buff allies |

See also → [[Unified Tactical AI (Technical)]], [[Combat System (Technical)]].
