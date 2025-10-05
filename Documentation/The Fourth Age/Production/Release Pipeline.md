---
tags: [production, release]
---

# Release Pipeline

## Build steps
1. Update version in `Config/DefaultGame.ini`.
2. Run automation script:  
   `RunUAT BuildCookRun -project="TFA.uproject" -platform=Win64 -cook -build -stage -pak -archive`.
3. Package output under `/Builds/<version>/`.

---

## Test procedure
- Run smoke tests on each mode (Combat, Exploration, Overworld).
- Validate Journal logs no errors.
- Verify deterministic replays.

---

## Deployment
- Internal: Itch.io private build.
- Public: Steam Early Access or Epic Private Beta.
- Post-release: use Journal export for bug reproduction.

See also → [[Build and Packaging]].

