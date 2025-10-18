---
tags:
  - kalki/performance
  - kalki/optimization
  - kalki/best-practices
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: advanced
---

# Performance Guidelines

## Overview
This document defines performance targets, optimization strategies, and best practices for maintaining high performance in the Kalki project. Following these guidelines ensures smooth gameplay at 60 FPS on mid-range hardware.

---

## Purpose

### Why Performance Matters

**Problems from poor performance:**
1. **Low Framerate** - Stuttering, unresponsive controls
2. **Long Load Times** - Player frustration
3. **Memory Issues** - Crashes, instability
4. **Network Lag** - Desync, poor multiplayer experience
5. **Poor User Experience** - Players abandon game

**Benefits of good performance:**
- ✅ Smooth 60 FPS gameplay
- ✅ Fast load times (< 3 seconds)
- ✅ Low memory footprint
- ✅ Stable multiplayer
- ✅ Better player retention

---

## Performance Targets

### Frame Rate Targets

**Primary Target:**
```
60 FPS @ 1920×1080 on mid-range PC
- GPU: GTX 1060 / RX 580 equivalent
- CPU: Intel i5-8400 / Ryzen 5 2600
- RAM: 8 GB
- Settings: Medium
```

**Minimum Target:**
```
30 FPS @ 1920×1080 on low-end PC
- GPU: GTX 960 / RX 560
- CPU: Intel i3-7100 / Ryzen 3 1200
- RAM: 8 GB
- Settings: Low
```

**Frame Time Budget:**
- 60 FPS = 16.67ms per frame
- 30 FPS = 33.33ms per frame

**Thread Budget:**
```
Game Thread:  < 10ms  (logic, gameplay, AI)
Render Thread: < 12ms  (draw calls, shaders)
GPU:          < 14ms  (actual rendering)
```

---

### Memory Targets

**RAM Usage:**
```
Idle (Main Menu):     < 500 MB
In Combat (4v4):      < 1.5 GB
Peak Usage:           < 2.0 GB
```

**VRAM Usage:**
```
Low Settings:    < 1 GB
Medium Settings: < 2 GB
High Settings:   < 3 GB
```

**Memory Budget by System:**
```
Grid System:        < 50 MB
Character System:   < 100 MB per 10 characters
UI System:          < 200 MB
Audio System:       < 100 MB
```

---

### Load Time Targets

**Level Loading:**
```
Main Menu:     < 2 seconds
Combat Level:  < 3 seconds
Large Level:   < 5 seconds
```

**Asset Loading:**
```
Character:     < 0.5 seconds
Ability VFX:   < 0.2 seconds
UI Widget:     < 0.1 seconds
```

---

### Network Targets

**Latency:**
```
Acceptable:  < 100ms
Good:        < 50ms
Excellent:   < 30ms
```

**Bandwidth:**
```
Idle:           < 1 KB/s per player
Active Combat:  < 5 KB/s per player
Peak:           < 10 KB/s per player
```

**Replication:**
```
Character State: < 10 updates/second
Grid State:      Only on change
Ability Cast:    Immediate (reliable RPC)
```

---

## CPU Optimization

### Game Thread Optimization

**1. Minimize Tick Frequency**
```cpp
// ❌ BAD - Every frame
void AKalkiCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Expensive calculations every frame!
    UpdateAI();
    CheckLineOfSight();
    UpdateAnimations();
}

// ✅ GOOD - Reduce tick rate or use timers
AKalkiCharacter::AKalkiCharacter()
{
    PrimaryActorTick.bCanEverTick = false;  // Disable tick
}

void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Use timer instead - only when needed
    GetWorld()->GetTimerManager().SetTimer(
        UpdateHandle,
        this,
        &AKalkiCharacter::UpdateState,
        0.1f,  // 10 times per second, not 60!
        true
    );
}

// ✅ BETTER - Event-driven
void AKalkiCharacter::OnHealthChanged(int32 NewHP)
{
    // Only update when actually changes
    UpdateHealthDisplay();
}
```

**Tick Budget Guidelines:**
```
Ticking Actors:      < 50 actors per frame
Tick Interval:       0.1s - 0.5s for most actors
Critical Actors:     0.033s (30 Hz) maximum
Non-Critical:        1.0s or event-driven
```

---

**2. Cache Frequently Accessed Data**
```cpp
// ❌ BAD - Repeated lookups
void AKalkiCharacter::AttackTarget()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    FKalkiGridCoord Coord = GridManager->WorldPositionToCoord(GetActorLocation());
    // ... more GridManager calls
}

// ✅ GOOD - Cache subsystem reference
class AKalkiCharacter : public ACharacter
{
private:
    UPROPERTY()
    UKalkiGridManager* CachedGridManager;

public:
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        CachedGridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    }

    void AttackTarget()
    {
        if (CachedGridManager)  // Fast pointer check
        {
            FKalkiGridCoord Coord = CachedGridManager->WorldPositionToCoord(GetActorLocation());
        }
    }
};
```

**Caching Checklist:**
- ✅ Subsystems (GridManager, CombatManager, etc.)
- ✅ Components (MovementComponent, MeshComponent, etc.)
- ✅ Commonly accessed actors (PlayerController, GameMode)
- ✅ Expensive calculation results (pathfinding, line of sight)

---

**3. Avoid Expensive Operations in Tick**
```cpp
// ❌ BAD - Expensive operations every frame
void AKalkiCharacter::Tick(float DeltaTime)
{
    // Line trace every frame!
    FHitResult Hit;
    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
    
    // Find all actors every frame!
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiCharacter::StaticClass(), FoundActors);
    
    // Sort array every frame!
    FoundActors.Sort([](const AActor& A, const AActor& B) { return true; });
}

// ✅ GOOD - Do these rarely or on-demand
class AKalkiCharacter : public ACharacter
{
private:
    FTimerHandle LineOfSightTimer;
    TArray<AActor*> CachedNearbyActors;
    float ActorCacheRefreshRate = 1.0f;  // Once per second

public:
    void BeginPlay()
    {
        Super::BeginPlay();
        
        // Update line of sight every 0.5 seconds
        GetWorld()->GetTimerManager().SetTimer(
            LineOfSightTimer,
            this,
            &AKalkiCharacter::UpdateLineOfSight,
            0.5f,
            true
        );
        
        // Update nearby actors every second
        GetWorld()->GetTimerManager().SetTimer(
            ActorCacheTimer,
            this,
            &AKalkiCharacter::RefreshNearbyActors,
            ActorCacheRefreshRate,
            true
        );
    }
    
    void UpdateLineOfSight()
    {
        // Only trace twice per second, not 60 times!
    }
    
    void RefreshNearbyActors()
    {
        // Update cache once per second
        CachedNearbyActors.Empty();
        UGameplayStatics::GetAllActorsOfClass(
            GetWorld(),
            AKalkiCharacter::StaticClass(),
            CachedNearbyActors
        );
    }
};
```

**Expensive Operations:**
```
Line Traces:          Max 5 per frame total
Actor Iteration:      Cache and refresh slowly
Array Sorting:        Only when data changes
String Operations:    Minimize, use FName when possible
GetAllActorsOfClass:  Once per second maximum
```

---

**4. Use Object Pooling**
```cpp
// ❌ BAD - Spawn/destroy every time
void UKalkiAbility::SpawnVFX()
{
    AKalkiVFXActor* VFX = GetWorld()->SpawnActor<AKalkiVFXActor>();
    VFX->Play();
    VFX->SetLifeSpan(2.0f);  // Destroyed after 2 seconds
}

// ✅ GOOD - Object pool
class UKalkiVFXPool : public UObject
{
private:
    TArray<AKalkiVFXActor*> AvailableVFX;
    TArray<AKalkiVFXActor*> ActiveVFX;
    
public:
    AKalkiVFXActor* GetVFX()
    {
        AKalkiVFXActor* VFX = nullptr;
        
        if (AvailableVFX.Num() > 0)
        {
            // Reuse existing
            VFX = AvailableVFX.Pop();
        }
        else
        {
            // Spawn new
            VFX = GetWorld()->SpawnActor<AKalkiVFXActor>();
        }
        
        ActiveVFX.Add(VFX);
        return VFX;
    }
    
    void ReturnVFX(AKalkiVFXActor* VFX)
    {
        ActiveVFX.Remove(VFX);
        AvailableVFX.Add(VFX);
        
        // Hide and reset
        VFX->SetActorHiddenInGame(true);
        VFX->Reset();
    }
};
```

**Pool These:**
- ✅ VFX actors (explosions, spell effects)
- ✅ Projectiles (arrows, bullets)
- ✅ UI widgets (damage numbers, tooltips)
- ✅ Audio components
- ✅ Decals

---

### AI Optimization

**1. Stagger AI Updates**
```cpp
// ❌ BAD - All AI update same frame
void AKalkiAIController::Tick(float DeltaTime)
{
    UpdateDecisionTree();  // Expensive!
}

// ✅ GOOD - Stagger updates
class AKalkiAIManager : public UWorldSubsystem
{
private:
    TArray<AKalkiAIController*> AllAI;
    int32 CurrentUpdateIndex = 0;
    int32 AIToUpdatePerFrame = 2;  // Only update 2 AI per frame
    
public:
    void Tick(float DeltaTime)
    {
        // Update only a few AI per frame
        for (int32 i = 0; i < AIToUpdatePerFrame && i < AllAI.Num(); ++i)
        {
            int32 Index = (CurrentUpdateIndex + i) % AllAI.Num();
            AllAI[Index]->UpdateDecisionTree();
        }
        
        CurrentUpdateIndex = (CurrentUpdateIndex + AIToUpdatePerFrame) % AllAI.Num();
    }
};
```

**AI Update Frequency:**
```
Critical AI (player's target): 10 Hz (every 0.1s)
Active AI (nearby):            5 Hz (every 0.2s)
Distant AI (out of view):      1 Hz (every 1.0s)
Idle AI (not in combat):       0.2 Hz (every 5.0s)
```

---

**2. Simplify Pathfinding**
```cpp
// ❌ BAD - Full A* every frame
void AKalkiAIController::Tick(float DeltaTime)
{
    TArray<FKalkiGridCoord> Path = Pathfinding->FindPath(Start, Goal);
}

// ✅ GOOD - Cache and reuse
class AKalkiAIController : public AAIController
{
private:
    TArray<FKalkiGridCoord> CachedPath;
    FKalkiGridCoord CachedGoal;
    float PathRefreshInterval = 1.0f;
    
public:
    TArray<FKalkiGridCoord> GetPathToGoal(const FKalkiGridCoord& Goal)
    {
        // Only recalculate if goal changed or interval elapsed
        if (Goal != CachedGoal || ShouldRefreshPath())
        {
            CachedPath = Pathfinding->FindPath(CurrentPosition, Goal);
            CachedGoal = Goal;
            LastPathRefreshTime = GetWorld()->GetTimeSeconds();
        }
        
        return CachedPath;
    }
    
    bool ShouldRefreshPath() const
    {
        return (GetWorld()->GetTimeSeconds() - LastPathRefreshTime) > PathRefreshInterval;
    }
};
```

**Pathfinding Budget:**
```
Max Pathfinding Operations: 5 per frame
Path Cache Time:            1-5 seconds
Max Path Length:            50 tiles (break into segments)
Hierarchical Pathfinding:   Use for > 50 tiles
```

---

## GPU Optimization

### Draw Call Reduction

**1. Use Instanced Static Meshes**
```cpp
// ❌ BAD - Individual actors (400 draw calls for 20x20 grid)
void CreateGrid()
{
    for (int32 Y = 0; Y < 20; ++Y)
    {
        for (int32 X = 0; X < 20; ++X)
        {
            AKalkiTileActor* Tile = GetWorld()->SpawnActor<AKalkiTileActor>();
        }
    }
}

// ✅ GOOD - Instanced mesh (1 draw call for entire grid)
class AKalkiGridVisualizer : public AActor
{
private:
    UPROPERTY()
    UInstancedStaticMeshComponent* TileInstances;
    
public:
    void CreateGrid()
    {
        for (int32 Y = 0; Y < 20; ++Y)
        {
            for (int32 X = 0; X < 20; ++X)
            {
                FTransform Transform;
                Transform.SetLocation(FVector(X * 100, Y * 100, 0));
                TileInstances->AddInstance(Transform);
            }
        }
    }
};
```

**Draw Call Budget:**
```
Target:    < 500 draw calls per frame
Good:      < 1000 draw calls per frame
Limit:     < 2000 draw calls per frame
```

**Use Instancing For:**
- ✅ Grid tiles
- ✅ Trees, rocks, props
- ✅ Particles
- ✅ UI elements (when many identical)

---

**2. Merge Static Meshes**
```cpp
// Editor utility for merging meshes
void MergeLevelProps()
{
    // Select all static props
    TArray<AActor*> PropsToMerge;
    
    // Merge into single mesh
    FMeshMergingSettings Settings;
    Settings.bMergeMaterials = true;
    Settings.bGenerateLightmapUV = true;
    
    UStaticMesh* MergedMesh = FMeshMerge::MergeStaticMeshes(
        PropsToMerge,
        Settings
    );
}
```

**Merge Candidates:**
- ✅ Background architecture
- ✅ Decorative props
- ✅ Static environment pieces
- ❌ Don't merge: Characters, interactive objects, moving parts

---

**3. LOD (Level of Detail)**
```cpp
// Set up LODs in mesh import settings
LOD0 (0-500 units):    Full detail (5000 tris)
LOD1 (500-1000 units): Medium (2000 tris)
LOD2 (1000-2000 units): Low (500 tris)
LOD3 (2000+ units):     Impostor (100 tris or billboard)
```

**LOD Strategy:**
```
Characters:        4 LODs (aggressive)
Environment:       2-3 LODs (moderate)
Props:            2 LODs (minimal)
Effects:          No LOD (already optimized)
```

---

### Material Optimization

**1. Reduce Material Complexity**
```cpp
// ❌ BAD - Expensive material
Material Instructions: 500+
- Multiple texture samples: 10
- Complex math: Heavy fresnel, reflection
- Multiple lights: 20+ dynamic lights
- Translucency: Expensive blend mode

// ✅ GOOD - Optimized material
Material Instructions: < 100
- Texture samples: 2-3 (base color, normal, mask)
- Simple math: Basic operations only
- Light limit: 4 dynamic lights max
- Opaque: Use masked instead of translucent when possible
```

**Material Instruction Budget:**
```
Character:      < 200 instructions
Environment:    < 150 instructions
Props:          < 100 instructions
UI:             < 50 instructions
```

---

**2. Texture Memory**
```cpp
// Texture resolution guidelines
Character Diffuse:     2048×2048 (high detail)
Environment Diffuse:   1024×1024 (medium)
Props Diffuse:         512×512 (low)
UI Icons:             256×256 (very low)

// Use texture compression
BC1 (DXT1):  Diffuse without alpha
BC3 (DXT5):  Diffuse with alpha
BC5 (ATI2):  Normal maps
BC7:         High quality (use sparingly)
```

**Texture Streaming:**
```cpp
// Enable texture streaming
r.Streaming.PoolSize 2000  // 2000 MB texture pool
r.Streaming.MaxEffectiveScreenSize 4096
```

---

**3. Material Instances**
```cpp
// ❌ BAD - Multiple unique materials
Material_Tile_Red:    Full material
Material_Tile_Blue:   Full material
Material_Tile_Green:  Full material
// Each is separate draw call!

// ✅ GOOD - Material instances
Material_Tile_Master:         Base material
Material_Tile_Red_Inst:       Instance (color parameter)
Material_Tile_Blue_Inst:      Instance (color parameter)
Material_Tile_Green_Inst:     Instance (color parameter)
// All batched into fewer draw calls!
```

**Material Instance Benefits:**
- Faster to change parameters
- Better batching
- Lower memory usage
- Faster iteration

---

## Memory Optimization

### Asset Loading

**1. Lazy Loading**
```cpp
// ❌ BAD - Load everything at startup
void AGameMode::BeginPlay()
{
    // Load all 100 character meshes!
    for (const FString& CharacterPath : AllCharacterPaths)
    {
        LoadObject<USkeletalMesh>(nullptr, *CharacterPath);
    }
}

// ✅ GOOD - Load on demand
class UKalkiAssetManager : public UObject
{
private:
    TMap<FName, TWeakObjectPtr<UObject>> LoadedAssets;
    
public:
    UObject* GetAsset(const FSoftObjectPath& AssetPath)
    {
        // Check if already loaded
        TWeakObjectPtr<UObject>* Cached = LoadedAssets.Find(AssetPath.GetAssetFName());
        if (Cached && Cached->IsValid())
        {
            return Cached->Get();
        }
        
        // Load synchronously (or async with callback)
        UObject* Asset = AssetPath.TryLoad();
        LoadedAssets.Add(AssetPath.GetAssetFName(), Asset);
        
        return Asset;
    }
    
    void UnloadUnusedAssets()
    {
        // Remove assets no longer referenced
        for (auto It = LoadedAssets.CreateIterator(); It; ++It)
        {
            if (!It.Value().IsValid())
            {
                It.RemoveCurrent();
            }
        }
    }
};
```

---

**2. Async Loading**
```cpp
// ❌ BAD - Synchronous loading (blocks game thread)
void LoadCharacter()
{
    USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(
        nullptr,
        TEXT("/Game/Characters/Warrior.Warrior")
    );
    // Game freezes until loaded!
    Character->SetMesh(Mesh);
}

// ✅ GOOD - Asynchronous loading
void LoadCharacterAsync()
{
    FSoftObjectPath MeshPath(TEXT("/Game/Characters/Warrior.Warrior"));
    
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        MeshPath,
        FStreamableDelegate::CreateUObject(this, &AMyClass::OnCharacterLoaded)
    );
}

void OnCharacterLoaded()
{
    // Mesh loaded, game never froze!
    FSoftObjectPath MeshPath(TEXT("/Game/Characters/Warrior.Warrior"));
    USkeletalMesh* Mesh = Cast<USkeletalMesh>(MeshPath.ResolveObject());
    Character->SetMesh(Mesh);
}
```

---

**3. Asset Unloading**
```cpp
// Unload assets when no longer needed
void UKalkiAssetManager::OnLevelTransition()
{
    // Unload old level assets
    UnloadOldLevelAssets();
    
    // Force garbage collection
    GetWorld()->ForceGarbageCollection(true);
    
    // Load new level assets asynchronously
    LoadNewLevelAssets();
}

void UnloadOldLevelAssets()
{
    // Clear references to old assets
    OldLevelActors.Empty();
    OldLevelMeshes.Empty();
    OldLevelMaterials.Empty();
    
    // They will be garbage collected
}
```

---

### Memory Leaks

**1. Weak Pointers**
```cpp
// ❌ BAD - Strong reference prevents GC
class AKalkiCharacter : public ACharacter
{
private:
    UPROPERTY()
    AKalkiCharacter* TargetCharacter;  // Strong reference!
    
    // If TargetCharacter destroyed, this still holds reference
    // Prevents garbage collection = memory leak
};

// ✅ GOOD - Weak pointer
class AKalkiCharacter : public ACharacter
{
private:
    TWeakObjectPtr<AKalkiCharacter> TargetCharacter;  // Weak reference
    
public:
    void AttackTarget()
    {
        if (TargetCharacter.IsValid())
        {
            // Safe to use
            TargetCharacter->TakeDamage(Damage);
        }
        else
        {
            // Target was destroyed, weak pointer knows!
            TargetCharacter = nullptr;
        }
    }
};
```

**Use Weak Pointers For:**
- ✅ Cached actor references
- ✅ Target tracking
- ✅ Event listeners
- ✅ UI references to gameplay actors

**Use Strong Pointers (UPROPERTY) For:**
- ✅ Components you own
- ✅ Subobjects you created
- ✅ Assets you need to keep loaded

---

**2. Unbind Delegates**
```cpp
// ❌ BAD - Never unbound = memory leak
void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    GridManager->OnTileChanged.AddDynamic(this, &AKalkiCharacter::OnTileChanged);
    // Never removed! GridManager holds strong reference to this character forever!
}

// ✅ GOOD - Unbind in EndPlay
void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (GridManager)
    {
        GridManager->OnTileChanged.AddDynamic(this, &AKalkiCharacter::OnTileChanged);
    }
}

void AKalkiCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (GridManager)
    {
        GridManager->OnTileChanged.RemoveDynamic(this, &AKalkiCharacter::OnTileChanged);
    }
    
    Super::EndPlay(EndPlayReason);
}
```

---

**3. Clear Arrays and Maps**
```cpp
// Memory management
class AKalkiManager : public AActor
{
private:
    TArray<AKalkiCharacter*> Characters;
    TMap<int32, FKalkiData> DataMap;
    
public:
    void CleanUp()
    {
        // Clear arrays
        Characters.Empty();  // Releases memory
        
        // Clear maps
        DataMap.Empty();  // Releases memory
        
        // Or reset without releasing memory (for reuse)
        Characters.Reset();  // Keeps capacity
        DataMap.Reset();  // Keeps capacity
    }
};
```

---

## Network Optimization

### Replication Optimization

**1. Replicate Only What's Needed**
```cpp
// ❌ BAD - Replicate everything
class AKalkiCharacter : public ACharacter
{
    UPROPERTY(Replicated)
    int32 CurrentHP;  // OK - needed
    
    UPROPERTY(Replicated)
    int32 MaxHP;  // Unnecessary - rarely changes
    
    UPROPERTY(Replicated)
    FString CharacterName;  // Unnecessary - never changes
    
    UPROPERTY(Replicated)
    float LastDamageTime;  // Unnecessary - server-only
    
    UPROPERTY(Replicated)
    TArray<UKalkiAbility*> AllAbilities;  // Expensive - large array
};

// ✅ GOOD - Replicate selectively
class AKalkiCharacter : public ACharacter
{
    UPROPERTY(Replicated)
    int32 CurrentHP;  // Replicated - changes often, clients need
    
    UPROPERTY()
    int32 MaxHP;  // Not replicated - set once in constructor
    
    UPROPERTY()
    FString CharacterName;  // Not replicated - set once
    
    float LastDamageTime;  // Not replicated - server-only logic
    
    UPROPERTY(Replicated)
    int32 ActiveAbilityIndex;  // Replicate index, not array
};
```

**Replication Checklist:**
- ❌ Don't replicate: Constants, server-only data, cosmetic info
- ✅ Do replicate: Health, position, state, active ability
- ✅ Use RepNotify for important changes
- ✅ Use replication conditions (COND_SkipOwner, etc.)

---

**2. Reduce Replication Frequency**
```cpp
// Set update frequency per actor
AKalkiCharacter::AKalkiCharacter()
{
    // Default is every frame (expensive!)
    NetUpdateFrequency = 10.0f;  // 10 updates per second
    MinNetUpdateFrequency = 2.0f;  // Minimum 2 per second when not moving
}

// Adjust dynamically
void AKalkiCharacter::SetReplicationPriority(bool bHighPriority)
{
    if (bHighPriority)
    {
        NetUpdateFrequency = 20.0f;  // Player's target - update faster
    }
    else
    {
        NetUpdateFrequency = 5.0f;  // Background character - update slower
    }
}
```

**Update Frequency Guidelines:**
```
Player Character:           20 Hz (every 0.05s)
Enemy (in combat):          10 Hz (every 0.1s)
Enemy (out of combat):      5 Hz (every 0.2s)
Background Character:        2 Hz (every 0.5s)
Static Objects:             1 Hz (every 1.0s)
```

---

**3. Use RPC Wisely**
```cpp
// ❌ BAD - RPC spam
void AKalkiCharacter::Tick(float DeltaTime)
{
    if (HasAuthority())
    {
        // Sends RPC every frame!
        MulticastUpdatePosition(GetActorLocation());
    }
}

// ✅ GOOD - Replicate property instead
class AKalkiCharacter : public ACharacter
{
    UPROPERTY(Replicated)
    FVector_NetQuantize ReplicatedLocation;  // Automatically replicated
    
    // Only use RPC for events
    UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayAbilityVFX(int32 AbilityID);
};
```

**RPC Usage:**
- ✅ Use for: One-time events (ability cast, death, etc.)
- ❌ Don't use for: Continuous data (position, rotation, HP)
- ✅ Prefer property replication over RPCs
- ✅ Make RPCs reliable only when necessary

---

### Bandwidth Optimization

**1. Compress Network Data**
```cpp
// Use quantized types for less bandwidth
class AKalkiCharacter : public ACharacter
{
    // ❌ BAD - Full precision (12 bytes)
    UPROPERTY(Replicated)
    FVector Position;
    
    // ✅ GOOD - Quantized (6 bytes)
    UPROPERTY(Replicated)
    FVector_NetQuantize Position;
    
    // ❌ BAD - Full precision (4 bytes)
    UPROPERTY(Replicated)
    float Health;
    
    // ✅ GOOD - Quantized to 1/100 (2 bytes)
    UPROPERTY(Replicated)
    FVector_NetQuantize100 Health;  // 0.01 precision
};
```

**Quantized Types:**
```
FVector_NetQuantize:      1 cm precision
FVector_NetQuantize10:    0.1 cm precision
FVector_NetQuantize100:   0.01 cm precision
FVector_NetQuantizeNormal: For normalized vectors (directions)
```

---

**2. Conditional Replication**
```cpp
void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(AKalkiCharacter, CurrentHP);

    // Don't replicate to owner (they already know)
    DOREPLIFETIME_CONDITION(AKalkiCharacter, ServerPosition, COND_SkipOwner);

    // Only repl```cpp
    // Only replicate to owner
    DOREPLIFETIME_CONDITION(AKalkiCharacter, ActionPoints, COND_OwnerOnly);

    // Only replicate when changed (not every update)
    DOREPLIFETIME_CONDITION(AKalkiCharacter, TeamID, COND_InitialOnly);

    // Custom condition
    DOREPLIFETIME_CONDITION(AKalkiCharacter, TargetActor, COND_Custom);
}

void AKalkiCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
    Super::PreReplication(ChangedPropertyTracker);

    // Custom replication condition
    DOREPLIFETIME_ACTIVE_OVERRIDE(AKalkiCharacter, TargetActor, bIsInCombat);
}
```

**Replication Conditions:**

```
COND_None:              Always replicate
COND_InitialOnly:       Only on spawn (team, name)
COND_OwnerOnly:         Only to owning client (input, camera)
COND_SkipOwner:         To everyone except owner
COND_SimulatedOnly:     Only to simulated proxies
COND_AutonomousOnly:    Only to autonomous proxy
COND_ReplayOrOwner:     Owner or replay
COND_ReplayOnly:        Only during replay
COND_Custom:            Custom condition
```

---

**3. Delta Serialization**

```cpp
// Use delta serialization for large structures
USTRUCT()
struct FKalkiCharacterState
{
    GENERATED_BODY()

    UPROPERTY()
    int32 HP;

    UPROPERTY()
    int32 MaxHP;

    UPROPERTY()
    int32 ActionPoints;

    UPROPERTY()
    TArray<int32> AbilityIDs;

    // Enable delta serialization
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<int32>(
            AbilityIDs,
            DeltaParms,
            *this
        );
    }
};

template<>
struct TStructOpsTypeTraits<FKalkiCharacterState> : public TStructOpsTypeTraitsBase2<FKalkiCharacterState>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};
```

**Delta Serialization:**

- Only sends changes, not entire array
- Ideal for large arrays (inventory, abilities)
- Reduces bandwidth significantly
- Slightly more CPU overhead

---

## Profiling and Debugging

### Essential Console Commands

**Frame Rate:**

```
stat fps               // Show FPS
stat unit              // Show frame time breakdown
stat game              // Game thread time
stat scenerendering    // Render thread time
stat gpu               // GPU time
```

**Memory:**

```
stat memory            // Memory overview
memreport              // Detailed memory report
obj list               // List all objects
obj list class=Actor   // List specific class
obj gc                 // Force garbage collection
```

**Network:**

```
stat net               // Network stats
stat netplayermovement // Player movement replication
netprofile             // Network profiling
```

**Draw Calls:**

```
stat rhi               // RHI stats (draw calls)
stat scenerendering    // Scene rendering stats
r.ScreenPercentage 50  // Reduce resolution for testing
```

---

### Profiling Tools

**1. Unreal Insights**

```
// Enable tracing
-trace=cpu,gpu,frame,log

// Or in code
FTraceAuxiliary::Start(FTraceAuxiliary::EConnectionType::Network);

// View in Unreal Insights
UnrealInsights.exe
```

**What to Profile:**

- Game thread bottlenecks
- Render thread bottlenecks
- GPU bottlenecks
- Memory allocations
- Network traffic

---

**2. CPU Profiler**

```cpp
// Profile specific function
{
    SCOPE_CYCLE_COUNTER(STAT_MyExpensiveFunction);
    MyExpensiveFunction();
}

// Define stat
DECLARE_CYCLE_STAT(TEXT("My Expensive Function"), STAT_MyExpensiveFunction, STATGROUP_Game);

// View with
stat game
stat startfile       // Start recording
stat stopfile        // Stop and save
```

---

**3. GPU Profiler**

```
// In editor
Ctrl + Shift + ,     // Open GPU Visualizer

// Console commands
profilegpu           // Detailed GPU breakdown
stat gpu             // GPU time
r.ProfileGPU.Root    // Profile specific pass
```

**GPU Bottlenecks:**

- Shadow rendering
- Translucency
- Post-processing
- Particle systems
- Material complexity

---

### Performance Testing

**1. Test Scenarios**

```cpp
// Worst case scenario tests
void UKalkiPerformanceTest::TestWorstCase()
{
    // Spawn maximum characters
    for (int32 i = 0; i < 20; ++i)
    {
        SpawnCharacter();
    }

    // Spawn maximum VFX
    for (int32 i = 0; i < 50; ++i)
    {
        SpawnVFX();
    }

    // Start combat (all systems active)
    StartCombat();

    // Measure performance
    float AvgFPS = MeasureAverageFPS(60.0f);  // 60 seconds
    float MinFPS = MeasureMinimumFPS(60.0f);
    float AvgFrameTime = MeasureAverageFrameTime(60.0f);

    // Log results
    UE_LOG(LogTemp, Log, TEXT("Worst Case: Avg %.1f FPS, Min %.1f FPS, Avg %.2fms"), 
        AvgFPS, MinFPS, AvgFrameTime);
}
```

**Test Cases:**

- Idle (menu)
- Light combat (2v2)
- Heavy combat (4v4)
- Maximum chaos (all VFX, abilities)
- Long session (memory leaks)

---

**2. Automated Performance Tests**

```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiPerformanceTest,
    "Kalki.Performance.FrameRate",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::PerfFilter
)

bool FKalkiPerformanceTest::RunTest(const FString& Parameters)
{
    UWorld* World = GetTestWorld();
    
    // Setup test scenario
    SetupWorstCaseScenario(World);

    // Measure performance
    float StartTime = FPlatformTime::Seconds();
    int32 FrameCount = 0;
    float MinFrameTime = FLT_MAX;
    float MaxFrameTime = 0.0f;

    // Run for 10 seconds
    while ((FPlatformTime::Seconds() - StartTime) < 10.0f)
    {
        World->Tick(LEVELTICK_All, 0.016f);
        
        float FrameTime = World->GetDeltaSeconds();
        MinFrameTime = FMath::Min(MinFrameTime, FrameTime);
        MaxFrameTime = FMath::Max(MaxFrameTime, FrameTime);
        
        FrameCount++;
    }

    float TotalTime = FPlatformTime::Seconds() - StartTime;
    float AvgFPS = FrameCount / TotalTime;
    float MinFPS = 1.0f / MaxFrameTime;

    // Verify meets target
    TestTrue(TEXT("Average FPS >= 60"), AvgFPS >= 60.0f);
    TestTrue(TEXT("Minimum FPS >= 30"), MinFPS >= 30.0f);

    return true;
}
```

---

## Platform-Specific Optimization

### Windows PC

**Graphics Settings:**

```cpp
// Scalability settings
r.SetRes 1920x1080w      // Windowed resolution
r.VSync 0                // Disable VSync for testing
r.MaxFPS 0               // Unlimited (for testing)

// Quality settings
sg.ViewDistanceQuality 3  // 0=Low, 3=Epic
sg.ShadowQuality 2        // Medium shadows
sg.PostProcessQuality 2   // Medium post
sg.TextureQuality 3       // High textures
sg.EffectsQuality 2       // Medium effects
```

**Hardware Tiers:**

```
Low (GTX 960):
- sg.ViewDistanceQuality 1
- sg.ShadowQuality 0
- sg.PostProcessQuality 0
- sg.TextureQuality 1
- sg.EffectsQuality 1
- Target: 30 FPS @ 1080p

Medium (GTX 1060):
- sg.ViewDistanceQuality 2
- sg.ShadowQuality 1
- sg.PostProcessQuality 1
- sg.TextureQuality 2
- sg.EffectsQuality 2
- Target: 60 FPS @ 1080p

High (RTX 2060):
- sg.ViewDistanceQuality 3
- sg.ShadowQuality 2
- sg.PostProcessQuality 2
- sg.TextureQuality 3
- sg.EffectsQuality 3
- Target: 60 FPS @ 1080p
```

---

## Optimization Checklist

### Pre-Release Checklist

**CPU:**

- [ ] Tick rate minimized (< 50 ticking actors)
- [ ] Subsystems cached
- [ ] Expensive operations moved to timers
- [ ] Object pooling implemented
- [ ] AI updates staggered
- [ ] Pathfinding cached

**GPU:**

- [ ] Draw calls < 1000
- [ ] Instanced meshes used
- [ ] LODs set up (3+ levels)
- [ ] Material complexity < 200 instructions
- [ ] Texture streaming enabled
- [ ] Material instances used

**Memory:**

- [ ] Asset loading lazy/async
- [ ] Weak pointers for caches
- [ ] Delegates unbound in EndPlay
- [ ] No memory leaks (tested)
- [ ] Texture sizes reasonable
- [ ] Unused assets unloaded

**Network:**

- [ ] Replication frequency appropriate
- [ ] Only necessary properties replicated
- [ ] RPCs used sparingly
- [ ] Quantized types used
- [ ] Conditional replication set up
- [ ] Bandwidth < 10 KB/s per player

**Testing:**

- [ ] 60 FPS on target hardware
- [ ] No frame drops in worst case
- [ ] Memory stable over 1 hour session
- [ ] Load times < 3 seconds
- [ ] No crashes after 2 hour session
- [ ] Multiplayer stable with 4+ players

---

## Quick Wins

### Immediate Performance Gains

**1. Disable Tick on Actors**

```cpp
// Default: Tick enabled (expensive)
// Change to: Tick disabled, use timers
PrimaryActorTick.bCanEverTick = false;
```

**Impact:** -5-10ms game thread time

---

**2. Use Instanced Static Meshes**

```cpp
// Change from: Individual static mesh actors
// Change to: Single instanced static mesh component
```

**Impact:** 400 draw calls → 1 draw call

---

**3. Reduce Material Complexity**

```cpp
// Audit materials > 200 instructions
// Simplify: Remove extra texture samples, complex math
```

**Impact:** +5-10 FPS

---

**4. Enable Texture Streaming**

```cpp
r.Streaming.PoolSize 2000
```

**Impact:** -500 MB VRAM usage

---

**5. Set Appropriate Net Update Frequencies**

```cpp
// Default: 100 Hz (every frame)
// Change to: 10 Hz for most actors
NetUpdateFrequency = 10.0f;
```

**Impact:** -50% network bandwidth

---

## Common Performance Mistakes

### Mistake 1: GetAllActorsOfClass Every Frame

```cpp
// ❌ BAD - O(n) search every frame
void Tick(float DeltaTime)
{
    TArray<AActor*> AllCharacters;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AKalkiCharacter::StaticClass(),
        AllCharacters
    );
}

// ✅ GOOD - Cache and update rarely
void BeginPlay()
{
    RefreshCharacterCache();
    
    GetWorld()->GetTimerManager().SetTimer(
        CacheRefreshTimer,
        this,
        &AMyClass::RefreshCharacterCache,
        1.0f,  // Once per second
        true
    );
}
```

---

### Mistake 2: String Operations in Hot Paths

```cpp
// ❌ BAD - String construction every frame
void Tick(float DeltaTime)
{
    FString Message = FString::Printf(TEXT("HP: %d"), CurrentHP);
    // String allocation and formatting is expensive!
}

// ✅ GOOD - Only update when needed
void OnHealthChanged(int32 NewHP)
{
    // Only once when HP actually changes
    UpdateHealthText(NewHP);
}
```

---

### Mistake 3: Not Using Const References

```cpp
// ❌ BAD - Copies entire array
void ProcessTiles(TArray<FKalkiGridCoord> Coords)
{
    // Array copied on function call!
}

// ✅ GOOD - Pass by const reference
void ProcessTiles(const TArray<FKalkiGridCoord>& Coords)
{
    // No copy, just reference
}
```

---

### Mistake 4: Unnecessary Blueprint Ticks

```
Blueprint Actor Tick Event
  → (Empty or simple operations)

// Disable in Blueprint:
Class Defaults → Actor Tick → Start with Tick Enabled = False
```

---

### Mistake 5: Translucent Materials

```cpp
// ❌ BAD - Translucent blend mode
Material Blend Mode: Translucent
// Very expensive! Rendered in separate pass, no depth pre-pass

// ✅ GOOD - Masked blend mode
Material Blend Mode: Masked
// Much cheaper! Uses alpha testing instead
```

---

## Performance Budget Template

### Frame Budget Breakdown

```
Total Frame Time:    16.67ms (60 FPS)

Game Thread:         10ms
  - Tick:            4ms
  - AI:              2ms
  - Physics:         2ms
  - Gameplay:        2ms

Render Thread:       12ms
  - Scene Setup:     2ms
  - Shadow Rendering: 3ms
  - Base Pass:       4ms
  - Post Process:    2ms
  - UI:              1ms

GPU:                 14ms
  - Shadows:         4ms
  - Base Pass:       5ms
  - Lighting:        2ms
  - Translucency:    1ms
  - Post Process:    2ms
```

---

## Related Documents

### Foundation

- [[02_Conventions/Coding_Standards]] - Code optimization
- [[04_Implementation/Common_Patterns]] - Efficient patterns
- [[04_Implementation/Error_Handling]] - Performance validation

### Implementation

- [[04_Implementation/Multiplayer_Guidelines]] - Network optimization
- [[04_Implementation/Testing_Procedures]] - Performance testing
- [[03_Systems/Grid_System]] - Grid optimization examples

### Systems

- [[03_Systems/Grid_Visualizer]] - Instanced mesh optimization
- [[03_Systems/Combat_Log_System]] - UI optimization

---

## Quick Reference

### Essential Console Commands

```
// Performance
stat fps
stat unit
stat game
stat gpu
stat memory

// Profiling
stat startfile
stat stopfile
profilegpu

// Network
stat net

// Optimization Testing
r.ScreenPercentage 50
t.MaxFPS 30
r.VSync 0
```

### Performance Targets

```
Frame Rate:    60 FPS @ 1080p (16.67ms)
Game Thread:   < 10ms
Render Thread: < 12ms
GPU:           < 14ms
Memory:        < 2 GB
VRAM:          < 2 GB
Load Time:     < 3 seconds
Network:       < 10 KB/s per player
```

---

## Document Info

**File Path:** `Kalki_GDD/04_Implementation/Performance_Guidelines.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Advanced  
**Note:** Comprehensive performance optimization guide with targets and best practices