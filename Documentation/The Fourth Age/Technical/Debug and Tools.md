---
tags: [technical, tools]
---

# Debug and Developer Tools

## Core Utilities
| Tool | Function |
|------|-----------|
| **Event Browser** | Inspect Journal events in-editor. |
| **Tag Visualizer** | Filter and color GameplayTags. |
| **Snapshot Inspector** | Compare snapshots for diffs. |
| **Encounter Builder** | Place enemies, assign seeds. |
| **Ability Editor** | Create and edit Ability DataAssets. |

---

## Event Browser Layout

```
Filters Panel ──────────────┐  
Event Table (sortable) │  
Payload Inspector (JSON) │  
Replay Button │

```


---

## Debug Commands
| Command | Purpose |
|----------|----------|
| `tfa.log.toggle` | Toggle event logging. |
| `tfa.journal.dump` | Write current log to file. |
| `tfa.rng.seed <id>` | Force RNG seed. |
| `tfa.snapshot.save` | Manual snapshot. |
| `tfa.snapshot.replay` | Replay from last snapshot. |

---

## QA Workflow
1. Run combat with `Event Browser` open.  
2. Export event segment when anomaly detected.  
3. Re-import in debug mode for replay.  

See also → [[Journal System (Technical)]], [[Production/Backlog]].
