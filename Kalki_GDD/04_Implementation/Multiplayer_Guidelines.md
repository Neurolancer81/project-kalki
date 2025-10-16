---
tags:
  - kalki/multiplayer
  - kalki/networking
  - kalki/replication
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: advanced
---

# Multiplayer Guidelines

## Overview
This document defines networking architecture, replication strategies, and multiplayer best practices for Kalki. All systems are designed with multiplayer in mind from day one, even if single-player ships first.

---

## Purpose

### Why Multiplayer-First Design?

**Problems with retrofitting multiplayer:**
- Fundamental architecture changes required
- Gameplay code assumes single-player
- State management becomes complex
- Bug-prone refactoring

**Benefits of designing for multiplayer first:**
- ✅ Clean architecture (server authority, clear state)
- ✅ Easier to maintain
- ✅ Single-player works "for free" (just no network)
- ✅ Multiplayer mode is an addition, not a rewrite

---

## Core Principles

### 1. Server Authority

**Rule:** Server is always the source of truth.

```cpp
// ❌ BAD: Client modifies gameplay state
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    CurrentHP -= Damage; // Client can cheat!
}

// ✅ GOOD: Server authority
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    if (!HasAuthority())
    {
        KalkiLog::Combat(TEXT("TakeDamage on client - ignored"), EKalkiLogSeverity::Warning);
        return;
    }

    CurrentHP -= Damage; // Only server modifies
    // Automatically replicates to clients
}
````

---

### 2. Deterministic Gameplay

**Rule:** Same inputs produce same outputs on all machines.

```cpp
// ✅ GOOD: Deterministic
int32 CalculateDamage(int32 BaseDamage, int32 ArmorClass)
{
    // Pure function - always same result
    return FMath::Max(0, BaseDamage - ArmorClass);
}

// ❌ BAD: Non-deterministic
int32 CalculateDamage(int32 BaseDamage)
{
    // Random on each machine - desync!
    return BaseDamage + FMath::RandRange(0, 10);
}

// ✅ GOOD: Server-controlled randomness
int32 AKalkiCharacter::CalculateDamageWithRoll(int32 BaseDamage)
{
    if (!HasAuthority())
    {
        return BaseDamage; // Clients use replicated value
    }

    // Server generates random, replicates result
    int32 Roll = FMath::RandRange(0, 10);
    LastDamageRoll = Roll; // Replicated property
    return BaseDamage + Roll;
}
```

---

### 3. Minimal Replication

**Rule:** Only replicate what's necessary.

```cpp
// ❌ BAD: Replicate everything
UPROPERTY(Replicated)
FVector LocalVelocity; // Local calculation, doesn't need replication

UPROPERTY(Replicated)
bool bIsMoving; // Can be computed from velocity

// ✅ GOOD: Replicate only essential state
UPROPERTY(Replicated)
FKalkiGridCoord GridPosition; // Essential position

UPROPERTY(Replicated)
int32 CurrentHP; // Essential health

// Compute locally (no replication needed)
bool IsMoving() const
{
    return GetVelocity().Size() > 0.1f;
}
```

---

### 4. Predictive Client

**Rule:** Clients predict, server validates and corrects.

```cpp
void AKalkiCharacter::RequestMoveTo(const FKalkiGridCoord& Target)
{
    // Client predicts immediately (responsive UI)
    if (!HasAuthority())
    {
        PredictedPosition = Target;
        VisualPosition = Target;
        
        // Request server validation
        ServerMoveTo(Target);
        return;
    }

    // Server validates
    if (CanMoveTo(Target))
    {
        GridPosition = Target; // Replicates
    }
    else
    {
        // Correction RPC
        ClientRejectMove(GridPosition);
    }
}

UFUNCTION(Server, Reliable, WithValidation)
void ServerMoveTo_Implementation(const FKalkiGridCoord& Target)
{
    RequestMoveTo(Target);
}

bool ServerMoveTo_Validate(const FKalkiGridCoord& Target)
{
    // Prevent cheating
    return IsValidCoord(Target) && IsAdjacent(GridPosition, Target);
}

UFUNCTION(Client, Reliable)
void ClientRejectMove_Implementation(const FKalkiGridCoord& ServerPosition)
{
    // Server rejected - correct prediction
    PredictedPosition = ServerPosition;
    GridPosition = ServerPosition;
    VisualPosition = ServerPosition;
}
```

---

## Network Modes

### Understanding Net Modes

```cpp
enum ENetMode
{
    NM_Standalone,      // Single player (no networking)
    NM_DedicatedServer, // Dedicated server (no local player)
    NM_ListenServer,    // Host + server (local player + server)
    NM_Client           // Client only
};
```

**Check Net Mode:**

```cpp
ENetMode NetMode = GetWorld()->GetNetMode();

// Is this the server?
bool bIsServer = (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer);

// Or use helper:
bool bHasAuthority = HasAuthority(); // true on server, false on client
```

---

### Authority Checks

**Pattern: Check authority before modifying state**

```cpp
void AKalkiCharacter::Heal(int32 Amount)
{
    // Authority check
    if (!HasAuthority())
    {
        KalkiLog::Character(TEXT("Heal on client - ignored"), EKalkiLogSeverity::Warning);
        return;
    }

    // Server-only logic
    CurrentHP = FMath::Min(CurrentHP + Amount, MaxHP);
    
    KalkiLog::Combat(
        FString::Printf(TEXT("%s healed %d HP"), *GetName(), Amount)
    );
}
```

**Helper function:**

```cpp
bool UKalkiGridManager::HasGridAuthority() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    ENetMode NetMode = World->GetNetMode();
    return (NetMode == NM_Standalone || 
            NetMode == NM_ListenServer || 
            NetMode == NM_DedicatedServer);
}
```

---

## Replication Patterns

### Pattern 1: Simple Replication

**When to Use:**

- Property changes infrequently
- Doesn't need notification on change
- No special logic needed

**Implementation:**

```cpp
// Header
UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Character")
int32 CurrentHP;

UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Character")
int32 MaxHP;

// CPP
void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKalkiCharacter, CurrentHP);
    DOREPLIFETIME(AKalkiCharacter, MaxHP);
}
```

---

### Pattern 2: RepNotify (Replication with Notification)

**When to Use:**

- Need to react when property changes
- Update visuals/UI on replication
- Common for important state changes

**Implementation:**

```cpp
// Header
UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, BlueprintReadOnly)
int32 CurrentHP;

UFUNCTION()
void OnRep_CurrentHP();

// CPP
void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKalkiCharacter, CurrentHP);
}

void AKalkiCharacter::OnRep_CurrentHP()
{
    // Called on clients when CurrentHP changes
    KalkiLog::Character(
        FString::Printf(TEXT("HP changed to %d"), CurrentHP),
        EKalkiLogSeverity::Log,
        this
    );

    // Update health bar
    UpdateHealthDisplay();

    // Play damaged effect if HP decreased
    if (CurrentHP < LastHP)
    {
        PlayDamagedEffect();
    }

    LastHP = CurrentHP;
}

// Server modifies normally
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    if (!HasAuthority()) return;

    CurrentHP -= Damage; // OnRep_CurrentHP fires on clients automatically
}
```

---

### Pattern 3: Conditional Replication

**When to Use:**

- Property only relevant to owner
- Save bandwidth
- Player-specific data

**Implementation:**

```cpp
// Replicate only to owner
UPROPERTY(ReplicatedUsing = OnRep_ActionPoints)
int32 ActionPoints;

void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Only replicate to owning client
    DOREPLIFETIME_CONDITION(AKalkiCharacter, ActionPoints, COND_OwnerOnly);
}
```

**Replication Conditions:**

```cpp
COND_None               // Always replicate
COND_InitialOnly        // Only on initial replication
COND_OwnerOnly          // Only to owner
COND_SkipOwner          // To everyone except owner
COND_SimulatedOnly      // Only to simulated proxies
COND_AutonomousOnly     // Only to autonomous proxy
COND_SimulatedOrPhysics // Simulated or physics
COND_InitialOrOwner     // Initial or owner
COND_Custom             // Custom condition
```

---

### Pattern 4: Struct Replication

**When to Use:**

- Multiple related properties
- Atomic updates (all or nothing)
- Cleaner organization

**Implementation:**

```cpp
// Struct
USTRUCT(BlueprintType)
struct FKalkiCharacterStats
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Strength = 10;

    UPROPERTY()
    int32 Dexterity = 10;

    UPROPERTY()
    int32 Constitution = 10;

    UPROPERTY()
    int32 Intelligence = 10;

    UPROPERTY()
    int32 Wisdom = 10;

    UPROPERTY()
    int32 Charisma = 10;
};

// Character
UPROPERTY(ReplicatedUsing = OnRep_Stats)
FKalkiCharacterStats Stats;

UFUNCTION()
void OnRep_Stats();

void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKalkiCharacter, Stats);
}

void AKalkiCharacter::OnRep_Stats()
{
    // All stats replicated atomically
    RecalculateDerivedStats();
    UpdateCharacterSheet();
}
```

---

## RPC (Remote Procedure Call) Patterns

### RPC Types

```cpp
UFUNCTION(Server, Reliable)              // Client → Server
void ServerDoSomething();

UFUNCTION(Client, Reliable)              // Server → Specific Client
void ClientDoSomething();

UFUNCTION(NetMulticast, Reliable)        // Server → All Clients
void MulticastDoSomething();
```

**Reliability:**

- `Reliable` - Guaranteed delivery, order preserved (important data)
- `Unreliable` - Best effort, may drop (visual effects, frequent updates)

---

### Pattern: Server RPC (Client → Server)

**When to Use:**

- Client requests action
- Server validates and executes
- Prevent cheating

**Implementation:**

```cpp
// Header
UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
void ServerCastAbility(int32 AbilityIndex, const FKalkiGridCoord& TargetCoord);

// CPP
void AKalkiCharacter::ServerCastAbility_Implementation(int32 AbilityIndex, const FKalkiGridCoord& TargetCoord)
{
    // Runs on server only

    // Validate
    if (!CanCastAbility(AbilityIndex))
    {
        KalkiLog::Ability(TEXT("Cannot cast ability"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (!IsValidTarget(TargetCoord))
    {
        KalkiLog::Ability(TEXT("Invalid target"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Execute
    UKalkiAbility* Ability = GetAbility(AbilityIndex);
    Ability->Execute(TargetCoord);

    // Replicated properties update automatically
}

bool AKalkiCharacter::ServerCastAbility_Validate(int32 AbilityIndex, const FKalkiGridCoord& TargetCoord)
{
    // Prevent malicious input
    // Return false to disconnect cheating client

    // Check index bounds
    if (AbilityIndex < 0 || AbilityIndex >= 10)
    {
        return false; // Invalid index
    }

    // Check coordinate bounds
    if (!IsValidCoord(TargetCoord))
    {
        return false; // Invalid coordinate
    }

    return true;
}

// Usage (from client)
void AKalkiPlayerController::OnAbilityCast(int32 AbilityIndex, FKalkiGridCoord Target)
{
    AKalkiCharacter* Character = GetPawn<AKalkiCharacter>();
    if (Character)
    {
        Character->ServerCastAbility(AbilityIndex, Target);
    }
}
```

---

### Pattern: Client RPC (Server → Specific Client)

**When to Use:**

- Player-specific feedback
- UI updates
- Sound/VFX only that player should see

**Implementation:**

```cpp
// Header
UFUNCTION(Client, Reliable)
void ClientShowAbilityFailed(const FText& Reason);

UFUNCTION(Client, Reliable)
void ClientShowDamageNumber(int32 Damage, FVector Location);

// CPP
void AKalkiCharacter::ClientShowAbilityFailed_Implementation(const FText& Reason)
{
    // Runs on specific client only
    ShowNotification(Reason);
    PlayErrorSound();
}

void AKalkiCharacter::ClientShowDamageNumber_Implementation(int32 Damage, FVector Location)
{
    // Runs on specific client only
    SpawnDamageNumberWidget(Damage, Location);
}

// Server calls
void AKalkiCharacter::TryUseAbility(int32 AbilityIndex)
{
    if (!HasAuthority()) return;

    if (!CanUseAbility(AbilityIndex))
    {
        // Tell owning client it failed
        ClientShowAbilityFailed(INVTEXT("Not enough action points"));
        return;
    }

    // Use ability...
}
```

---

### Pattern: Multicast RPC (Server → All Clients)

**When to Use:**

- Visual/audio effects all players see
- Cosmetic only (no gameplay state)
- Announcements

**Implementation:**

```cpp
// Header
UFUNCTION(NetMulticast, Reliable)
void MulticastPlayHitEffect(FVector HitLocation);

UFUNCTION(NetMulticast, Unreliable) // Unreliable for frequent effects
void MulticastPlayFootstep(FVector Location);

// CPP
void AKalkiCharacter::MulticastPlayHitEffect_Implementation(FVector HitLocation)
{
    // Runs on ALL clients + server

    // Spawn particles
    if (HitParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitParticles,
            HitLocation
        );
    }

    // Play sound
    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            HitSound,
            HitLocation
        );
    }

    // Screen shake (only for nearby players)
    float Distance = FVector::Dist(HitLocation, GetActorLocation());
    if (Distance < 1000.0f)
    {
        PlayCameraShake();
    }
}

// Server triggers
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    if (!HasAuthority()) return;

    CurrentHP -= Damage;

    // Show hit effect on all clients
    MulticastPlayHitEffect(GetActorLocation());
}
```

---

## Deterministic Systems

### Grid System (Deterministic by Design)

**Why it works:**

- Same grid config on all machines
- Server creates, clients recreate identically
- No tile data replication needed

```cpp
// Server creates grid
void AKalkiLevelManager::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        // Server creates
        UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
        GridManager->CreateGrid(GridSizeX, GridSizeY, TileSize, GridOrigin);
    }
}

// Clients receive config via replication
void AKalkiLevelManager::OnRep_GridConfig()
{
    // Client recreates identical grid
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    GridManager->CreateGrid(GridSizeX, GridSizeY, TileSize, GridOrigin);
}

// Result: Same grid on all machines, no per-tile replication
```

---

### Turn-Based Combat (Inherently Multiplayer-Friendly)

**Benefits:**

- No real-time synchronization
- Server validates each action
- No prediction needed (turn-based)
- Clear authority model

```cpp
void AKalkiGameMode::ProcessPlayerAction(AKalkiCharacter* Character, const FKalkiAction& Action)
{
    if (!HasAuthority()) return;

    // Check: Is it this player's turn?
    if (CurrentTurnCharacter != Character)
    {
        KalkiLog::Combat(TEXT("Not your turn!"), EKalkiLogSeverity::Warning);
        return;
    }

    // Validate action
    if (!Action.IsValid())
    {
        KalkiLog::Combat(TEXT("Invalid action"), EKalkiLogSeverity::Error);
        return;
    }

    // Execute (state replicates automatically)
    ExecuteAction(Character, Action);

    // Next turn
    AdvanceTurn();
}
```

---

## Subsystem Replication

### Pattern: Deterministic Subsystems

**Subsystems don't replicate - they recreate deterministically**

```cpp
// GridManager doesn't replicate
// Instead: Same inputs → same grid on all machines

void UKalkiGridManager::CreateGrid(int32 SizeX, int32 SizeY, float TileSize, FVector Origin)
{
    // This function is deterministic
    // Called with same parameters on all machines
    // Result: Identical grid everywhere

    for (int32 Y = 0; Y < SizeY; ++Y)
    {
        for (int32 X = 0; X < SizeX; ++X)
        {
            FKalkiGridCoord Coord(X, Y);
            FKalkiGridTile Tile;
            
            // Deterministic calculation
            Tile.WorldPosition = Origin + FVector(
                X * TileSize + TileSize * 0.5f,
                Y * TileSize + TileSize * 0.5f,
                0.0f
            );
            
            GridTiles.Add(Coord, Tile);
        }
    }
}
```

---

## State Replication Examples

### Example 1: Character State

```cpp
UCLASS()
class AKalkiCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // ========================================
    // REPLICATED STATE
    // ========================================

    // Position (replicated by engine)
    UPROPERTY(Replicated)
    FKalkiGridCoord GridPosition;

    // Health
    UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
    int32 CurrentHP;

    UPROPERTY(Replicated)
    int32 MaxHP;

    // Status effects
    UPROPERTY(Replicated)
    TArray<EKalkiStatusEffect> ActiveEffects;

    // Turn state
    UPROPERTY(ReplicatedUsing = OnRep_HasActed)
    bool bHasActed;

    // ========================================
    // LOCAL STATE (NOT REPLICATED)
    // ========================================

    // Visual position (interpolated locally)
    FVector VisualPosition;

    // UI state
    bool bIsShowingAbilityMenu;

    // Cached calculations
    int32 CachedArmorClass;
};
```

---

### Example 2: Game State

```cpp
UCLASS()
class AKalkiGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    // Current turn
    UPROPERTY(ReplicatedUsing = OnRep_CurrentTurn)
    int32 CurrentTurnNumber;

    UPROPERTY(Replicated)
    TObjectPtr<AKalkiCharacter> CurrentTurnCharacter;

    // Combat participants
    UPROPERTY(Replicated)
    TArray<TObjectPtr<AKalkiCharacter>> PlayerCharacters;

    UPROPERTY(Replicated)
    TArray<TObjectPtr<AKalkiCharacter>> EnemyCharacters;

    // Combat state
    UPROPERTY(Replicated)
    EKalkiCombatPhase CombatPhase;

    UFUNCTION()
    void OnRep_CurrentTurn()
    {
        // Update UI for all players
        UpdateTurnIndicator();
        HighlightCurrentCharacter();
    }
};
```

---

### Example 3: Player State

```cpp
UCLASS()
class AKalkiPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    // Character roster (persistent across levels)
    UPROPERTY(Replicated)
    TArray<FKalkiCharacterData> CharacterRoster;

    // Resources
    UPROPERTY(Replicated)
    int32 Gold;

    UPROPERTY(Replicated)
    TArray<FKalkiItemData> Inventory;

    // Progression
    UPROPERTY(Replicated)
    int32 Experience;

    UPROPERTY(Replicated)
    int32 Level;
};
```

---

## Bandwidth Optimization

### Pattern: Update Frequency

**Control how often properties replicate:**

```cpp
// Header
UPROPERTY(Replicated)
FVector WorldPosition;

// CPP - Constructor
AKalkiCharacter::AKalkiCharacter()
{
    // Control replication frequency
    NetUpdateFrequency = 10.0f; // 10 updates per second
    MinNetUpdateFrequency = 2.0f; // Minimum 2 updates per second

    // Movement replication
    SetReplicateMovement(true);
}
```

**Frequency Guidelines:**

- **High Priority** (30-60 Hz): Player character, projectiles
- **Medium Priority** (10-20 Hz): NPCs, interactive objects
- **Low Priority** (2-5 Hz): Static objects, distant actors

---

### Pattern: Relevancy

**Only replicate to relevant clients:**

```cpp
bool AKalkiCharacter::IsNetRelevantFor(
    const AActor* RealViewer,
    const AActor* ViewTarget,
    const FVector& SrcLocation
) const
{
    // Don't replicate if too far away
    float DistanceSq = (SrcLocation - GetActorLocation()).SizeSquared();
    float MaxDistanceSq = 5000.0f * 5000.0f;  // 50 meters

    if (DistanceSq > MaxDistanceSq)
    {
        return false; // Not relevant - don't replicate
    }

    return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
}
```

---

### Pattern: Dormancy

**Stop replication when actor is dormant:**

```cpp
// Make actor dormant (stops replication)
void AKalkiChest::OnChestOpened()
{
    bIsOpen = true;
    
    // Stop replicating (chest won't change)
    SetNetDormancy(DORM_DormantAll);
}

// Wake up actor (resume replication)
void AKalkiChest::OnPlayerNearby()
{
    if (NetDormancy > DORM_Awake)
    {
        FlushNetDormancy(); // Resume replication
    }
}
```

---

## Testing Multiplayer

### PIE (Play In Editor) Settings

**Test multiplayer locally:**

1. **Editor Preferences → Play**
    
    - Number of Players: 2-4
    - Net Mode: Play As Listen Server
    - Run Dedicated Server: Optional
2. **Test Scenarios:**
    
    - Client 1 performs action
    - Check: Client 2 sees result?
    - Check: Server logs correct?
    - Check: State synchronized?

---

### Console Commands for Testing

```cpp
// Simulate network lag
net PktLag=100        // 100ms lag
net PktLagVariance=20 // Random ±20ms

// Simulate packet loss
net PktLoss=5         // 5% packet loss

// Show network stats
stat net
stat netplayermovement

// Show replication
net.Replication.DebugDraw 1
```

---

### Common Multiplayer Bugs

**Bug: State desync**

```
Symptom: Client sees different state than server
Cause: Client modifying replicated property
Fix: Ensure only server modifies state
```

**Bug: RPC not firing**

```
Symptom: Server/Client RPC doesn't execute
Cause: Wrong owner, missing NetConnection
Fix: Check HasAuthority(), ensure valid owner
```

**Bug: Replication not working**

```
Symptom: Property doesn't replicate
Cause: Forgot GetLifetimeReplicatedProps()
Fix: Add DOREPLIFETIME in GetLifetimeReplicatedProps()
```

**Bug: Cheating possible**

```
Symptom: Client can cheat (infinite HP, etc.)
Cause: Client can modify gameplay state
Fix: Add authority checks, use RPCs
```

---

## Cheating Prevention

### Pattern: Server Validation

**Always validate client input:**

```cpp
void AKalkiCharacter::ServerMoveTo_Implementation(const FKalkiGridCoord& Target)
{
    // Validate: Is move possible?
    if (!CanMoveTo(Target))
    {
        KalkiLog::Network(
            FString::Printf(TEXT("Client %s attempted invalid move"), *GetName()),
            EKalkiLogSeverity::Warning
        );
        
        // Correct client
        ClientCorrectPosition(GridPosition);
        return;
    }

    // Valid - execute
    GridPosition = Target;
}

bool AKalkiCharacter::ServerMoveTo_Validate(const FKalkiGridCoord& Target)
{
    // Sanity checks (disconnect if fails)
    
    // Check: In grid bounds?
    if (!IsValidCoord(Target))
    {
        return false; // Disconnect client
    }

    // Check: Adjacent to current position?
    if (!IsAdjacent(GridPosition, Target))
    {
        return false; // Teleport hack attempt
    }

    return true;
}
```

---

### Pattern: Rate Limiting

**Prevent spam/flooding:**

```cpp
UCLASS()
class AKalkiCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    float LastAbilityCastTime = 0.0f;
    const float MinAbilityCastInterval = 0.5f; // 500ms cooldown
};

void AKalkiCharacter::ServerCastAbility_Implementation(int32 AbilityIndex)
{
    // Rate limit check
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastAbilityCastTime < MinAbilityCastInterval)
    {
        KalkiLog::Network(
            FString::Printf(TEXT("Client %s spamming abilities"), *GetName()),
            EKalkiLogSeverity::Warning
        );
        return; // Reject
    }

    LastAbilityCastTime = CurrentTime;

    // Process ability...
}
```

---

## Best Practices Summary

### ✅ Do This:

1. **Check Authority** - Always check HasAuthority() before modifying state
2. **Replicate Minimally** - Only replicate essential state
3. **Validate Input** - Server validates all client requests
4. **Use RepNotify** - React to replication with OnRep functions
5. **Deterministic Systems** - Same inputs → same outputs
6. **Client Prediction** - Predict locally, server corrects
7. **Test Early** - Test multiplayer from day one
8. **Log Network Events** - Use KalkiLog::Network channel

---

### ❌ Don't Do This:

1. **Client Modifies State** - Never let clients change gameplay state
2. **Trust Client** - Always assume clients can cheat
3. **Replicate Everything** - Bandwidth waste
4. **Assume Sync** - Network has latency, handle gracefully
5. **Forget Validation** - Always validate RPC parameters
6. **Use Unreliable for Important Data** - Use Reliable for critical RPCs
7. **Ignore Net Modes** - Code must work in all modes
8. **Hardcode Single-Player** - Design multiplayer-first

---

## Checklist for New Multiplayer Features

**Before implementing:**

- [ ] Identified what needs replication
- [ ] Determined server authority points
- [ ] Planned RPC flow (Client→Server→Clients)
- [ ] Designed client prediction (if needed)
- [ ] Planned validation logic

**During implementation:**

- [ ] Added authority checks
- [ ] Implemented GetLifetimeReplicatedProps()
- [ ] Added RepNotify functions
- [ ] Implemented Server/Client RPCs
- [ ] Added Validate functions
- [ ] Added network logging

**After implementation:**

- [ ] Tested in PIE with 2+ clients
- [ ] Tested with simulated lag
- [ ] Tested with packet loss
- [ ] Verified no desyncs
- [ ] Verified cheating not possible
- [ ] Updated documentation

---

## Related Documents

### Foundation

- [[00_Overview/Technical_Stack]] - Network architecture
- [[04_Implementation/Common_Patterns]] - Multiplayer patterns

### Systems

- [[03_Systems/Grid_System]] - Deterministic grid replication
- [[03_Systems/Logging_System]] - Network logging

### Implementation

- [[04_Implementation/Error_Handling]] - Network error handling
- [[04_Implementation/Testing_Procedures]] - Multiplayer testing

### Examples

- [[03_Systems/Grid_System]] - Multiplayer grid example
- Character Movement (future) - Multiplayer movement example
- Combat System (future) - Multiplayer combat example

---

## Quick Reference

### Common Multiplayer Code
### Common Multiplayer Code Snippets

**Authority Check:**
```cpp
if (!HasAuthority())
{
    KalkiLog::Network(TEXT("Operation requires authority"), EKalkiLogSeverity::Warning);
    return;
}
```

**Simple Replication:**
```cpp
UPROPERTY(Replicated)
int32 MyProperty;

void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyClass, MyProperty);
}
```

**RepNotify:**
```cpp
UPROPERTY(ReplicatedUsing = OnRep_MyProperty)
int32 MyProperty;

UFUNCTION()
void OnRep_MyProperty()
{
    // React to change
}
```

**Server RPC:**
```cpp
UFUNCTION(Server, Reliable, WithValidation)
void ServerDoAction(int32 Parameter);

void ServerDoAction_Implementation(int32 Parameter)
{
    // Server logic
}

bool ServerDoAction_Validate(int32 Parameter)
{
    return Parameter >= 0 && Parameter < 100; // Validate input
}
```

**Client RPC:**
```cpp
UFUNCTION(Client, Reliable)
void ClientShowFeedback(const FText& Message);

void ClientShowFeedback_Implementation(const FText& Message)
{
    // Client-only UI update
}
```

**Multicast RPC:**
```cpp
UFUNCTION(NetMulticast, Reliable)
void MulticastPlayEffect();

void MulticastPlayEffect_Implementation()
{
    // Visual/audio on all clients
}
```

---

## Network Flow Diagrams

### Player Action Flow
```
┌──────────────────────────────────────────────────────┐
│                  CLIENT (Player)                      │
│  1. Player presses ability button                     │
│  2. Client predicts visual effect                     │
│  3. Calls ServerCastAbility()                         │
└────────────────┬─────────────────────────────────────┘
                 │ RPC (Server, Reliable)
                 ↓
┌──────────────────────────────────────────────────────┐
│                  SERVER (Authority)                   │
│  4. Receives ServerCastAbility RPC                    │
│  5. Validates request (range, resources, cooldown)    │
│  6. Executes ability (modifies replicated state)      │
│  7. Calls MulticastPlayEffect()                       │
└────────────────┬─────────────────────────────────────┘
                 │ Replication + Multicast RPC
                 ↓
┌──────────────────────────────────────────────────────┐
│              ALL CLIENTS (Including Initiator)        │
│  8. Receive replicated state change                   │
│  9. OnRep functions fire                              │
│  10. MulticastPlayEffect executes                     │
│  11. UI/VFX update based on new state                 │
└──────────────────────────────────────────────────────┘
```

---

### Movement Flow (with Prediction & Correction)
```
┌─────────────────────────────────────────────────────┐
│              CLIENT (Initiator)                      │
│  1. Player clicks tile                               │
│  2. Immediately predict move (visual feedback)       │
│  3. Call ServerMoveTo(Target)                        │
│  4. Local state: PredictedPosition = Target          │
└────────────────┬────────────────────────────────────┘
                 │ RPC
                 ↓
┌─────────────────────────────────────────────────────┐
│              SERVER (Authority)                      │
│  5. Validate move (range, walkable, etc.)           │
│  6. VALID? → Update GridPosition (replicates)       │
│  7. INVALID? → Call ClientRejectMove(ServerPos)     │
└────────────────┬────────────────────────────────────┘
                 │ Replication or RPC
                 ↓
┌─────────────────────────────────────────────────────┐
│              CLIENT (Initiator)                      │
│  If VALID:                                           │
│    8. OnRep_GridPosition confirms prediction         │
│    9. Visual position matches predicted              │
│  If INVALID:                                         │
│    8. ClientRejectMove received                      │
│    9. Snap back to ServerPosition                    │
│    10. Show "Invalid move" feedback                  │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│           OTHER CLIENTS (Observers)                  │
│  8. Receive GridPosition replication                 │
│  9. OnRep_GridPosition fires                         │
│  10. Update character visual position                │
│  11. Play movement animation                         │
└─────────────────────────────────────────────────────┘
```

---

### Combat Damage Flow
```
┌─────────────────────────────────────────────────────┐
│              CLIENT (Attacker)                       │
│  1. Select attack action                             │
│  2. Call ServerAttack(Target)                        │
└────────────────┬────────────────────────────────────┘
                 │ Server RPC
                 ↓
┌─────────────────────────────────────────────────────┐
│              SERVER (Authority)                      │
│  3. Validate attack (range, turn, resources)        │
│  4. Calculate damage (server-side randomness)       │
│  5. Target->TakeDamage(Damage)                      │
│  6. Log to combat log                               │
│  7. Call MulticastPlayHitEffect(Location)           │
└────────────────┬────────────────────────────────────┘
                 │ Replication + Multicast
                 ↓
┌─────────────────────────────────────────────────────┐
│              ALL CLIENTS                             │
│  8. Target's CurrentHP replicates                   │
│  9. OnRep_CurrentHP fires on target                 │
│  10. MulticastPlayHitEffect executes               │
│  11. Update health bars                             │
│  12. Play damage number                             │
│  13. Play hit sound/VFX                             │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│         TARGET'S CLIENT (Specific)                   │
│  14. Additional client-specific feedback            │
│  15. Camera shake                                    │
│  16. Screen flash                                    │
└─────────────────────────────────────────────────────┘
```

---

## Advanced Topics

### Relevancy and Priority

**Custom relevancy for optimization:**
```cpp
class AKalkiCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Override relevancy
    virtual bool IsNetRelevantFor(
        const AActor* RealViewer,
        const AActor* ViewTarget,
        const FVector& SrcLocation
    ) const override
    {
        // Always relevant to owner
        if (GetOwner() == RealViewer)
        {
            return true;
        }

        // Not relevant if too far
        float Distance = FVector::Dist(SrcLocation, GetActorLocation());
        if (Distance > MaxRelevancyDistance)
        {
            return false;
        }

        // Not relevant if behind walls (optional - expensive)
        if (bCheckLineOfSight)
        {
            if (!HasLineOfSight(SrcLocation, GetActorLocation()))
            {
                return false;
            }
        }

        return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
    }

    // Override priority (higher = more frequent updates)
    virtual float GetNetPriority(
        const FVector& ViewPos,
        const FVector& ViewDir,
        AActor* Viewer,
        AActor* ViewTarget,
        UActorChannel* InChannel,
        float Time,
        bool bLowBandwidth
    ) override
    {
        float Priority = Super::GetNetPriority(ViewPos, ViewDir, Viewer, ViewTarget, InChannel, Time, bLowBandwidth);

        // Higher priority if:
        
        // Owner's character (always high priority)
        if (Viewer && Viewer->GetOwner() == this)
        {
            Priority *= 4.0f;
        }

        // In combat
        if (bIsInCombat)
        {
            Priority *= 2.0f;
        }

        // Close to viewer
        float Distance = FVector::Dist(ViewPos, GetActorLocation());
        if (Distance < 500.0f)
        {
            Priority *= 2.0f;
        }

        return Priority;
    }

private:
    float MaxRelevancyDistance = 5000.0f;
    bool bCheckLineOfSight = false;
    bool bIsInCombat = false;
};
```

---

### Delta Serialization

**Only send changed data:**
```cpp
// Automatic for most replicated properties
// For arrays/structs, ensure proper comparison

USTRUCT()
struct FKalkiCharacterStats
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Strength;

    UPROPERTY()
    int32 Dexterity;

    // ... other stats

    // Enable delta serialization
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FKalkiCharacterStats>(
            DeltaParms,
            *this
        );
    }
};

template<>
struct TStructOpsTypeTraits<FKalkiCharacterStats> : public TStructOpsTypeTraitsBase2<FKalkiCharacterStats>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};
```

---

### Connection Management

**Handle player join/leave:**
```cpp
class AKalkiGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Player joins
    virtual void PostLogin(APlayerController* NewPlayer) override
    {
        Super::PostLogin(NewPlayer);

        KalkiLog::Network(
            FString::Printf(TEXT("Player joined: %s"), *NewPlayer->GetName())
        );

        // Initialize player state
        InitializePlayerState(NewPlayer);

        // Notify other players
        BroadcastPlayerJoined(NewPlayer);

        // Update UI for all players
        UpdatePlayerList();
    }

    // Player leaves
    virtual void Logout(AController* Exiting) override
    {
        KalkiLog::Network(
            FString::Printf(TEXT("Player left: %s"), *Exiting->GetName())
        );

        // Handle mid-combat disconnect
        if (bInCombat)
        {
            HandleCombatDisconnect(Exiting);
        }

        // Notify other players
        BroadcastPlayerLeft(Exiting);

        // Update UI
        UpdatePlayerList();

        Super::Logout(Exiting);
    }

    // Handle connection loss
    virtual void HandleSeamlessTravelPlayer(AController*& C) override
    {
        Super::HandleSeamlessTravelPlayer(C);

        // Restore player state after travel
        RestorePlayerState(C);
    }
};
```

---

### Seamless Travel

**Level transitions without disconnecting:**
```cpp
// Enable in GameMode
class AKalkiGameMode : public AGameModeBase
{
public:
    AKalkiGameMode()
    {
        bUseSeamlessTravel = true;
    }

    // Which actors persist across travel
    virtual void GetSeamlessTravelActorList(
        bool bToTransition,
        TArray<AActor*>& ActorList
    ) override
    {
        Super::GetSeamlessTravelActorList(bToTransition, ActorList);

        // Add player characters (persist inventory, stats)
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (PC && PC->GetPawn())
            {
                ActorList.Add(PC->GetPawn());
            }
        }
    }
};

// Trigger travel
void AKalkiGameMode::TransitionToNextLevel()
{
    if (!HasAuthority())
    {
        return;
    }

    FString NextLevel = TEXT("/Game/Levels/Chapter2");
    GetWorld()->ServerTravel(NextLevel, true); // true = seamless
}
```

---

### Voice Chat Integration

**Future feature - integration points:**
```cpp
// Placeholder for voice chat
class AKalkiPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // Enable voice for party
    UFUNCTION(BlueprintCallable, Category = "Kalki|Voice")
    void EnableVoiceChat(bool bEnable)
    {
        // Future: Integrate with Vivox, Steam Voice, or similar
        // For now: Log intent
        KalkiLog::Network(
            FString::Printf(TEXT("Voice chat %s"), bEnable ? TEXT("enabled") : TEXT("disabled"))
        );
    }

    // Mute specific player
    UFUNCTION(BlueprintCallable, Category = "Kalki|Voice")
    void MutePlayer(APlayerState* Player, bool bMute)
    {
        // Future implementation
        KalkiLog::Network(
            FString::Printf(TEXT("Player %s %s"), 
                *Player->GetPlayerName(),
                bMute ? TEXT("muted") : TEXT("unmuted")
            )
        );
    }
};
```

---

## Debugging Tools

### Network Profiling

**Enable network statistics:**
```cpp
// Console commands
stat net                    // Network stats
stat netplayermovement      // Movement replication
stat game                   // Game thread time
stat fps                    // Frame rate

// Show replication
net.Replication.DebugDraw 1

// Packet simulation
net PktLag=100             // 100ms lag
net PktLoss=5              // 5% packet loss
net PktDup=2               // 2% duplicate packets
```

---

### Custom Network Logging

**Track replication events:**
```cpp
class AKalkiCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Log when property replicates
    virtual void OnRep_CurrentHP() override
    {
        KalkiLog::Network(
            FString::Printf(
                TEXT("HP replicated: %d → %d (Role: %s)"),
                LastHP,
                CurrentHP,
                *UEnum::GetValueAsString(GetLocalRole())
            )
        );

        LastHP = CurrentHP;
        UpdateHealthDisplay();
    }

    // Log RPC calls
    void ServerCastAbility_Implementation(int32 AbilityIndex) override
    {
        KalkiLog::Network(
            FString::Printf(
                TEXT("ServerCastAbility RPC: Ability %d from %s"),
                AbilityIndex,
                *GetName()
            )
        );

        // ... rest of implementation
    }
};
```

---

### Visual Debug Indicators

**Show network state in-game:**
```cpp
void AKalkiCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if !UE_BUILD_SHIPPING
    // Show role above character
    if (GEngine)
    {
        FString RoleString;
        switch (GetLocalRole())
        {
            case ROLE_Authority:
                RoleString = TEXT("Authority");
                break;
            case ROLE_AutonomousProxy:
                RoleString = TEXT("Autonomous");
                break;
            case ROLE_SimulatedProxy:
                RoleString = TEXT("Simulated");
                break;
            default:
                RoleString = TEXT("None");
                break;
        }

        DrawDebugString(
            GetWorld(),
            GetActorLocation() + FVector(0, 0, 100),
            RoleString,
            nullptr,
            FColor::White,
            0.0f
        );
    }
#endif
}
```

---

## Common Pitfalls and Solutions

### Pitfall: Forgetting GetLifetimeReplicatedProps

**Symptom:** Property doesn't replicate

**Solution:**
```cpp
void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DON'T FORGET THIS!
    DOREPLIFETIME(AKalkiCharacter, CurrentHP);
    DOREPLIFETIME(AKalkiCharacter, MaxHP);
}
```

---

### Pitfall: RepNotify Not Firing on Server

**Symptom:** OnRep function only fires on clients

**Explanation:** RepNotify functions only fire on clients when replicated. Server must call manually if needed.

**Solution:**
```cpp
void AKalkiCharacter::SetCurrentHP(int32 NewHP)
{
    if (!HasAuthority())
    {
        return;
    }

    CurrentHP = NewHP; // Replicates to clients, OnRep fires there

    // Manually call on server
    OnRep_CurrentHP();
}

void AKalkiCharacter::OnRep_CurrentHP()
{
    // Runs on clients (automatically)
    // And on server (if called manually)
    UpdateHealthDisplay();
}
```

---

### Pitfall: RPC Ordering

**Symptom:** RPC arrives before replicated property

**Explanation:** No guaranteed order between replication and RPCs

**Solution:**
```cpp
// ❌ BAD: Assume order
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    CurrentHP -= Damage; // Replicates
    ClientShowDamage(Damage); // RPC might arrive first!
}

void AKalkiCharacter::ClientShowDamage_Implementation(int32 Damage)
{
    // CurrentHP might not be updated yet!
    ShowDamageUI(CurrentHP, Damage);
}

// ✅ GOOD: Pass all needed data in RPC
void AKalkiCharacter::TakeDamage(int32 Damage)
{
    CurrentHP -= Damage;
    ClientShowDamage(Damage, CurrentHP); // Send current HP
}

void AKalkiCharacter::ClientShowDamage_Implementation(int32 Damage, int32 NewHP)
{
    // Have all data needed
    ShowDamageUI(NewHP, Damage);
}
```

---

### Pitfall: Multicast From Client

**Symptom:** Multicast RPC doesn't execute

**Explanation:** Only server can call multicast RPCs

**Solution:**
```cpp
// ❌ BAD: Client calls multicast
void AKalkiCharacter::PlayEmote()
{
    MulticastPlayEmote(); // Won't work from client!
}

// ✅ GOOD: Client requests, server multicasts
void AKalkiCharacter::PlayEmote()
{
    if (HasAuthority())
    {
        MulticastPlayEmote();
    }
    else
    {
        ServerPlayEmote(); // Request server to multicast
    }
}

UFUNCTION(Server, Reliable)
void ServerPlayEmote();

void AKalkiCharacter::ServerPlayEmote_Implementation()
{
    MulticastPlayEmote(); // Server multicasts to all
}
```

---

## Document Info
**File Path:** `Kalki_GDD/04_Implementation/Multiplayer_Guidelines.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Advanced  
**Note:** Reference for all multiplayer implementation