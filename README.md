# Project Kalki

## Vision
Tactical RPG (Four Epochs) leading to story-driven CRPG (Kalki: The Fourth Age)
Set in ancient India. D&D 5.2 SRD rules with future PF2e support.
Co-op PvE + competitive PvP.

## Technical Goals
- Swappable ruleset architecture
- GAS-based ability system
- Networked from day one
- C++ backbone, Blueprint for content

## Current Milestone: Prototype 1
- [ ] Project setup (Unreal 5.4, Rider, GAS enabled)
- [ ] AKalkiCharacter base class
- [ ] Basic grid movement
- [ ] One test ability using GAS
- [ ] Compiles and runs

## Key Decisions
- Using single AKalkiCharacter class for all combatants
- Control via Controller, not class hierarchy
- Data-driven character definitions (FKalkiCharacterData)

## Open Questions
- Grid system: 2D array? Spatial hash?
- Turn order: Initiative-based or player choice?
- Camera: Fixed isometric? Player-controlled?