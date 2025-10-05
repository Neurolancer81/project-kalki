---
tags: [technical, networking]
---

# Networking Model

## Multiplayer Philosophy
- Co-op mode supports up to **6 players**, **Turn-Based only**.  
- Server is authoritative for all game logic and Journal writes.

---

## Key Replicated Elements
| Element | Replicated? | Owner |
|----------|--------------|-------|
| Turn state | ✅ | Server |
| Journal events | ✅ (filtered) | Server |
| RNG seed | ✅ | Server |
| Player decisions | ✅ (TurnIntent struct) | Client → Server |

---

## Flow

```
Client selects action  
→ builds FTFA_TurnIntent  
→ sends RPC to Server  
Server validates & executes  
→ emits events to Journal  
→ replicates results to Clients
```


---

## Structures
```cpp
USTRUCT()
struct FTFA_TurnIntent
{
   int32 ActorId;
   FGameplayTag ActionTag;
   FVector TargetPos;
};
```
---

## Late Join

- Server sends last snapshot + subsequent event deltas.
    
- Client rebuilds state using JaSoT replay.
    

---

## RTwP Disabled

Multiplayer always Turn-Based to ensure sync determinism.

See also → [[Journal System (Technical)]], [[Combat System (Technical)]].