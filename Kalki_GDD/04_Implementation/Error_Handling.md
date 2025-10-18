---
tags:
  - kalki/implementation
  - kalki/error-handling
  - kalki/validation
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Error Handling

## Overview
This document defines error handling strategies, validation patterns, and defensive programming practices for the Kalki project. Proper error handling prevents crashes, aids debugging, and improves code reliability.

---

## Purpose

### Why Error Handling Matters

**Problems without proper error handling:**
1. **Crashes** - Null pointer dereferences, array out of bounds
2. **Silent Failures** - Bugs that go unnoticed
3. **Difficult Debugging** - No information about what went wrong
4. **Poor User Experience** - Unexpected behavior, no feedback
5. **Multiplayer Desyncs** - Inconsistent state across clients

**Benefits of good error handling:**
- ✅ Prevents crashes
- ✅ Clear error messages in logs
- ✅ Easier debugging
- ✅ Graceful degradation
- ✅ Better user feedback

---

## Core Principles

### 1. Fail Fast

**Detect errors as early as possible.**
```cpp
// ❌ BAD: Error propagates, crashes later
void ProcessData(const TArray<int32>& Data)
{
    int32 Value = Data[10]; // Might be out of bounds
    DoSomething(Value);     // Crash happens here (confusing)
}

// ✅ GOOD: Detect and handle immediately
void ProcessData(const TArray<int32>& Data)
{
    if (!Data.IsValidIndex(10))
    {
        KalkiLog::System(
            FString::Printf(TEXT("Data array too small: %d elements, need 11"), Data.Num()),
            EKalkiLogSeverity::Error
        );
        return; // Fail fast
    }

    int32 Value = Data[10];
    DoSomething(Value);
}
```

---

### 2. Validate Early, Validate Often

**Check preconditions at function entry.**
```cpp
void UKalkiGridManager::SetElevation(const FKalkiGridCoord& Coord, float Elevation)
{
    // Validation 1: Authority
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("SetElevation - No authority"), EKalkiLogSeverity::Warning);
        return;
    }

    // Validation 2: Valid coordinate
    if (!IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetElevation - Invalid coord: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Error
        );
        return;
    }

    // Validation 3: Valid elevation range
    if (Elevation < -1000.0f || Elevation > 1000.0f)
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetElevation - Elevation out of range: %.1f"), Elevation),
            EKalkiLogSeverity::Warning
        );
        Elevation = FMath::Clamp(Elevation, -1000.0f, 1000.0f);
    }

    // All validations passed - safe to proceed
    FKalkiGridTile* Tile = GridTiles.Find(Coord);
    if (Tile)
    {
        Tile->Elevation = Elevation;
        OnTileChanged.Broadcast(Coord);
    }
}
```

---

### 3. Early Return Pattern

**Exit function as soon as error detected.**
```cpp
// ❌ BAD: Deep nesting
void ProcessAbility(UKalkiAbility* Ability)
{
    if (Ability)
    {
        if (CanCastAbility(Ability))
        {
            if (HasEnoughResources(Ability))
            {
                if (IsValidTarget(Target))
                {
                    // Actually do work (4 levels deep!)
                    CastAbility(Ability);
                }
            }
        }
    }
}

// ✅ GOOD: Early returns
void ProcessAbility(UKalkiAbility* Ability)
{
    // Check 1
    if (!Ability)
    {
        KalkiLog::Ability(TEXT("Ability is null"), EKalkiLogSeverity::Error);
        return;
    }

    // Check 2
    if (!CanCastAbility(Ability))
    {
        KalkiLog::Ability(TEXT("Cannot cast ability"), EKalkiLogSeverity::Warning);
        return;
    }

    // Check 3
    if (!HasEnoughResources(Ability))
    {
        KalkiLog::Ability(TEXT("Not enough resources"), EKalkiLogSeverity::Warning);
        return;
    }

    // Check 4
    if (!IsValidTarget(Target))
    {
        KalkiLog::Ability(TEXT("Invalid target"), EKalkiLogSeverity::Warning);
        return;
    }

    // All checks passed - do work
    CastAbility(Ability);
}
```

---

### 4. Log Everything Important

**Every error should be logged with context.**
```cpp
void AKalkiCharacter::TakeDamage(int32 DamageAmount)
{
    if (!HasAuthority())
    {
        // Log WHO tried to do WHAT
        KalkiLog::Combat(
            FString::Printf(TEXT("%s - TakeDamage called on client (ignored)"), *GetName()),
            EKalkiLogSeverity::Warning,
            this // Context object
        );
        return;
    }

    if (DamageAmount < 0)
    {
        // Log invalid input
        KalkiLog::Combat(
            FString::Printf(TEXT("%s - Negative damage: %d"), *GetName(), DamageAmount),
            EKalkiLogSeverity::Error,
            this
        );
        return;
    }

    // Log successful action
    CurrentHP -= DamageAmount;
    KalkiLog::Combat(
        FString::Printf(TEXT("%s took %d damage (%d/%d HP)"), 
            *GetName(), DamageAmount, CurrentHP, MaxHP),
        EKalkiLogSeverity::Log,
        this
    );
}
```

---

## Null Pointer Checks

### Pattern: Check Before Use

**Always check pointers before dereferencing.**
```cpp
void AKalkiCharacter::AttackTarget()
{
    // Check 1: TargetActor exists
    if (!TargetActor)
    {
        KalkiLog::Combat(TEXT("No target selected"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Check 2: TargetActor is valid (not pending kill)
    if (!IsValid(TargetActor))
    {
        KalkiLog::Combat(TEXT("Target is invalid"), EKalkiLogSeverity::Warning, this);
        TargetActor = nullptr; // Clear invalid reference
        return;
    }

    // Check 3: WeaponComponent exists
    if (!WeaponComponent)
    {
        KalkiLog::Combat(TEXT("No weapon component"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Safe to use
    WeaponComponent->Attack(TargetActor);
}
```

---

### Pattern: IsValid() Check

**Use IsValid() for UObject pointers.**
```cpp
// IsValid() checks:
// - Pointer is not null
// - Object is not pending kill
// - Object is valid for use

void UpdateCharacterUI(AKalkiCharacter* Character)
{
    if (!IsValid(Character))
    {
        HideCharacterUI();
        return;
    }

    // Safe to access Character
    UpdateHealthBar(Character->GetCurrentHP(), Character->GetMaxHP());
}

// For weak pointers
void UpdateTargetInfo()
{
    if (!TargetCharacter.IsValid())
    {
        HideTargetInfo();
        return;
    }

    // Get strong pointer
    AKalkiCharacter* Target = TargetCharacter.Get();
    ShowTargetInfo(Target);
}
```

---

### Pattern: Component Validation

**Check components in BeginPlay.**
```cpp
void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Validate required components
    bool bHasErrors = false;

    if (!StatsComponent)
    {
        KalkiLog::Character(TEXT("Missing StatsComponent!"), EKalkiLogSeverity::Error, this);
        bHasErrors = true;
    }

    if (!AbilityComponent)
    {
        KalkiLog::Character(TEXT("Missing AbilityComponent!"), EKalkiLogSeverity::Error, this);
        bHasErrors = true;
    }

    if (!MovementComponent)
    {
        KalkiLog::Character(TEXT("Missing MovementComponent!"), EKalkiLogSeverity::Error, this);
        bHasErrors = true;
    }

    if (bHasErrors)
    {
        KalkiLog::Character(TEXT("Character has missing components - functionality will be limited"), EKalkiLogSeverity::Error, this);
        // Consider destroying actor or disabling functionality
    }
}
```

---

## Subsystem Checks

### Pattern: GetSubsystem with Validation

**Always validate subsystem retrieval.**
```cpp
void ProcessGridAction()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        KalkiLog::System(TEXT("GetWorld() returned null"), EKalkiLogSeverity::Error);
        return;
    }

    UKalkiGridManager* GridManager = World->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("GridManager subsystem not found"), EKalkiLogSeverity::Error);
        return;
    }

    // Safe to use GridManager
    GridManager->DoSomething();
}
```

---

### Pattern: Subsystem Initialization

**Check if subsystem is initialized.**
```cpp
UCLASS()
class UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    bool IsGridCreated() const { return bGridCreated; }

    void CreateGrid(...)
    {
        // ... create grid
        bGridCreated = true;
    }

    void GetTile(const FKalkiGridCoord& Coord)
    {
        if (!bGridCreated)
        {
            KalkiLog::Grid(TEXT("GetTile - Grid not created yet"), EKalkiLogSeverity::Error);
            return FKalkiGridTile(); // Return default
        }

        // Safe to access grid
        return GridTiles.FindRef(Coord);
    }

private:
    bool bGridCreated = false;
};
```

---

## Array and Collection Checks

### Pattern: Index Validation

**Check array bounds before access.**
```cpp
void ProcessAbilities(const TArray<UKalkiAbility*>& Abilities, int32 Index)
{
    // Validate array
    if (Abilities.Num() == 0)
    {
        KalkiLog::Ability(TEXT("Ability array is empty"), EKalkiLogSeverity::Warning);
        return;
    }

    // Validate index
    if (!Abilities.IsValidIndex(Index))
    {
        KalkiLog::Ability(
            FString::Printf(TEXT("Invalid index: %d (array size: %d)"), Index, Abilities.Num()),
            EKalkiLogSeverity::Error
        );
        return;
    }

    // Safe to access
    UKalkiAbility* Ability = Abilities[Index];
    if (!IsValid(Ability))
    {
        KalkiLog::Ability(
            FString::Printf(TEXT("Ability at index %d is null"), Index),
            EKalkiLogSeverity::Error
        );
        return;
    }

    // Use ability
    Ability->Execute();
}
```

---

### Pattern: TMap Safety

**Check if key exists before access.**
```cpp
void ProcessTile(const FKalkiGridCoord& Coord)
{
    // ❌ BAD: Unchecked access
    FKalkiGridTile Tile = GridTiles[Coord]; // Creates entry if doesn't exist!

    // ✅ GOOD: Check existence first
    if (!GridTiles.Contains(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Tile not found: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Warning
        );
        return;
    }

    FKalkiGridTile Tile = GridTiles[Coord]; // Safe

    // ✅ BETTER: Use Find (returns pointer)
    FKalkiGridTile* Tile = GridTiles.Find(Coord);
    if (!Tile)
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Tile not found: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Warning
        );
        return;
    }

    // Use *Tile

    // ✅ BEST: Use FindRef (returns copy or default)
    FKalkiGridTile Tile = GridTiles.FindRef(Coord);
    if (!Tile.IsValid()) // Custom validation
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Tile not found: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Warning
        );
        return;
    }
}
```

---

### Pattern: Safe Iteration

**Handle empty collections and modifications during iteration.**
```cpp
void ProcessAllCharacters(TArray<AKalkiCharacter*>& Characters)
{
    // Check empty
    if (Characters.Num() == 0)
    {
        KalkiLog::Character(TEXT("No characters to process"), EKalkiLogSeverity::Log);
        return;
    }

    // ❌ BAD: Modifying array during iteration
    for (AKalkiCharacter* Character : Characters)
    {
        if (Character->ShouldRemove())
        {
            Characters.Remove(Character); // Invalidates iterator!
        }
    }

    // ✅ GOOD: Mark for removal, then remove
    TArray<AKalkiCharacter*> ToRemove;
    for (AKalkiCharacter* Character : Characters)
    {
        if (!IsValid(Character))
        {
            ToRemove.Add(Character);
            continue;
        }

        if (Character->ShouldRemove())
        {
            ToRemove.Add(Character);
        }
        else
        {
            Character->Process();
        }
    }

    // Remove marked characters
    for (AKalkiCharacter* Character : ToRemove)
    {
        Characters.Remove(Character);
    }

    KalkiLog::Character(
        FString::Printf(TEXT("Processed %d characters, removed %d"), 
            Characters.Num(), ToRemove.Num())
    );
}
```

---

## Range and Bound Checks

### Pattern: Clamp Values

**Ensure values stay within valid ranges.**
```cpp
void AKalkiCharacter::SetHealth(int32 NewHP)
{
    // Validate input
    if (NewHP < 0)
    {
        KalkiLog::Character(
            FString::Printf(TEXT("Negative HP: %d, clamping to 0"), NewHP),
            EKalkiLogSeverity::Warning,
            this
        );
    }

    // Clamp to valid range
    CurrentHP = FMath::Clamp(NewHP, 0, MaxHP);

    KalkiLog::Character(
        FString::Printf(TEXT("HP set to %d (input was %d)"), CurrentHP, NewHP),
        EKalkiLogSeverity::Log,
        this
    );
}

void SetMovementSpeed(float Speed)
{
    // Validate range
    const float MinSpeed = 0.0f;
    const float MaxSpeed = 1000.0f;

    if (Speed < MinSpeed || Speed > MaxSpeed)
    {
        KalkiLog::Character(
            FString::Printf(TEXT("Speed out of range: %.1f, clamping to [%.1f, %.1f]"), 
                Speed, MinSpeed, MaxSpeed),
            EKalkiLogSeverity::Warning
        );
        Speed = FMath::Clamp(Speed, MinSpeed, MaxSpeed);
    }

    MovementSpeed = Speed;
}
```

---

### Pattern: Grid Coordinate Validation

**Specific pattern for grid bounds.**
```cpp
bool UKalkiGridManager::IsValidCoord(const FKalkiGridCoord& Coord) const
{
    return Coord.X >= 0 && Coord.X < GridSizeX &&
           Coord.Y >= 0 && Coord.Y < GridSizeY;
}

void ProcessTile(const FKalkiGridCoord& Coord)
{
    if (!GridManager->IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(
                TEXT("Coordinate out of bounds: (%d, %d), grid size: (%d, %d)"),
                Coord.X, Coord.Y, GridManager->GetGridSizeX(), GridManager->GetGridSizeY()
            ),
            EKalkiLogSeverity::Error
        );
        return;
    }

    // Coordinate is valid
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
}
```

---

## Multiplayer Error Handling

### Pattern: Authority Checks

**Validate authority before modifying state.**
```cpp
void AKalkiCharacter::TakeDamage(int32 DamageAmount)
{
    // Authority check
    if (!HasAuthority())
    {
        KalkiLog::Combat(
            FString::Printf(TEXT("%s - TakeDamage called on client (Role: %s)"),
                *GetName(),
                *UEnum::GetValueAsString(GetLocalRole())
            ),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    // Server-only logic
    CurrentHP -= DamageAmount;
}
```

---

### Pattern: RPC Validation

**Validate RPC parameters to prevent cheating.**
```cpp
UFUNCTION(Server, Reliable, WithValidation)
void ServerMoveTo(const FKalkiGridCoord& TargetCoord);

void AKalkiCharacter::ServerMoveTo_Implementation(const FKalkiGridCoord& TargetCoord)
{
    // Double-check validation (belt and suspenders)
    if (!IsValidMoveTarget(TargetCoord))
    {
        KalkiLog::Network(
            FString::Printf(TEXT("%s attempted invalid move to (%d, %d)"),
                *GetName(), TargetCoord.X, TargetCoord.Y),
            EKalkiLogSeverity::Warning,
            this
        );

        // Correct client
        ClientCorrectPosition(GridPosition);
        return;
    }

    // Valid move
    GridPosition = TargetCoord;
}

bool AKalkiCharacter::ServerMoveTo_Validate(const FKalkiGridCoord& TargetCoord)
{
    // Basic sanity checks (disconnect if fails)
    
    // Check: In grid bounds?
    if (!IsValidCoord(TargetCoord))
    {
        KalkiLog::Network(
            FString::Printf(TEXT("%s - Invalid coord in RPC: (%d, %d)"),
                *GetName(), TargetCoord.X, TargetCoord.Y),
            EKalkiLogSeverity::Error,
            this
        );
        return false; // Disconnect client
    }

    // Check: Not too far from current position (teleport hack)
    int32 Distance = ManhattanDistance(GridPosition, TargetCoord);
    if (Distance > MaxMovementRange * 2)
    {
        KalkiLog::Network(
            FString::Printf(TEXT("%s - Suspicious move distance: %d"),
                *GetName(), Distance),
            EKalkiLogSeverity::Error,
            this
        );
        return false; // Disconnect client
    }

    return true;
}
```

---

## File I/O Error Handling

### Pattern: File Operations

**Handle file read/write failures.**
```cpp
void UKalkiCombatLogViewModel::StartNewSession()
{
    // Create directory
    FString LogDir = FPaths::ProjectSavedDir() / TEXT("Logs/CombatLog/");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*LogDir))
    {
        if (!PlatformFile.CreateDirectoryTree(*LogDir))
        {
            KalkiLog::UI(
                FString::Printf(TEXT("Failed to create log directory: %s"), *LogDir),
                EKalkiLogSeverity::Error
            );
            bFileLoggingEnabled = false;
            return;
        }
    }

    // Generate filename
    FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    CurrentSessionFile = LogDir / FString::Printf(TEXT("CombatLog_%s.txt"), *Timestamp);

    // Write header
    FString Header = TEXT("=== Combat Log Session ===\n");
    if (!FFileHelper::SaveStringToFile(Header, *CurrentSessionFile))
    {
        KalkiLog::UI(
            FString::Printf(TEXT("Failed to create log file: %s"), *CurrentSessionFile),
            EKalkiLogSeverity::Error
        );
        bFileLoggingEnabled = false;
        CurrentSessionFile.Empty();
        return;
    }

    bFileLoggingEnabled = true;
    KalkiLog::UI(FString::Printf(TEXT("Combat log session started: %s"), *CurrentSessionFile));
}

void UKalkiCombatLogViewModel::WriteToFile(const FString& Message)
{
    if (!bFileLoggingEnabled || CurrentSessionFile.IsEmpty())
    {
        return;
    }

    // Attempt write
    bool bSuccess = FFileHelper::SaveStringToFile(
        Message + TEXT("\n"),
        *CurrentSessionFile,
        FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(),
        FILEWRITE_Append
    );

    if (!bSuccess)
    {
        // Log failure (only once to avoid spam)
        static bool bLoggedError = false;
        if (!bLoggedError)
        {
            KalkiLog::UI(
                FString::Printf(TEXT("Failed to write to log file: %s"), *CurrentSessionFile),
                EKalkiLogSeverity::Error
            );
            bLoggedError = true;
        }
    }
}
```

---

## Configuration/Asset Loading

### Pattern: Asset Loading with Fallback

**Handle missing assets gracefully.**
```cpp
void AKalkiCharacter::LoadCharacterMesh()
{
    // Try to load mesh
    UStaticMesh* Mesh = LoadObject<UStaticMesh>(
        nullptr,
        TEXT("/Game/Meshes/Characters/SK_Character.SK_Character")
    );

    if (!Mesh)
    {
        KalkiLog::Character(
            TEXT("Failed to load character mesh, using default"),
            EKalkiLogSeverity::Warning,
            this
        );

        // Fallback to default mesh
        Mesh = LoadObject<UStaticMesh>(
            nullptr,
            TEXT("/Engine/BasicShapes/Cube.Cube")
        );

        if (!Mesh)
        {
            KalkiLog::Character(
                TEXT("Failed to load default mesh!"),
                EKalkiLogSeverity::Error,
                this
            );
            // Continue without mesh (degrade gracefully)
            return;
        }
    }

    // Set mesh
    GetMesh()->SetStaticMesh(Mesh);
}
```

---

## Assertions and Checks

### Pattern: check() Macro

**Use check() for programmer errors (debug only).**
```cpp
void ProcessCriticalData(const TArray<int32>& Data)
{
    // check() fires in Debug/Development, crashes in all builds if false
    // Use for "should never happen" conditions
    check(Data.Num() > 0); // Should have been validated earlier

    // Process data
}
```

---

### Pattern: ensure() Macro

**Use ensure() for recoverable errors (logs in shipping).**
```cpp
void ProcessData(UObject* DataObject)
{
    // ensure() logs error but continues execution
    // Good for "shouldn't happen but we can recover"
    if (!ensure(DataObject != nullptr))
    {
        // Log will appear in output
        // Can continue with fallback
        DataObject = CreateDefaultObject();
    }

    DataObject->Process();
}

// ensureMsgf() with custom message
void ValidateState()
{
    if (!ensureMsgf(
        bIsInitialized,
        TEXT("ValidateState called before initialization!")
    ))
    {
        // Handle error
        Initialize();
    }
}
```

---

### Pattern: checkSlow() for Expensive Checks

**Use checkSlow() for checks that are expensive.**
```cpp
void ProcessLargeArray(const TArray<int32>& Data)
{
    // checkSlow() only active in Debug builds
    // Use for expensive validation
    checkSlow(IsDataSorted(Data)); // Expensive O(n) check

    // Process assuming sorted
}
```

---

### When to Use Each
```cpp
// check() - Programmer error, should never happen, crash is acceptable
check(GridManager != nullptr);

// ensure() - Shouldn't happen but can recover, log and continue
if (!ensure(Character->IsAlive()))
{
    // Handle dead character case
}

// checkSlow() - Expensive check, Debug only
checkSlow(ValidateInternalState());

// Manual if + log - Expected error conditions, handle gracefully
if (!IsValidCoord(Coord))
{
    KalkiLog::Grid(TEXT("Invalid coord"), EKalkiLogSeverity::Warning);
    return;
}
```

---

## Error Recovery Strategies

### Pattern: Fallback Values

**Provide sensible defaults when errors occur.**
```cpp
FKalkiGridTile UKalkiGridManager::GetTile(const FKalkiGridCoord& Coord) const
{
    if (!IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("GetTile - Invalid coord: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Warning
        );

        // Return default tile
        FKalkiGridTile DefaultTile;
        DefaultTile.Coordinate = Coord;
        DefaultTile.bWalkable = false; // Safe default
        DefaultTile.Elevation = 0.0f;
        return DefaultTile;
    }

    return GridTiles.FindRef(Coord);
}
```

---

### Pattern: Retry Logic

**Retry transient failures.**
```cpp
bool LoadDataWithRetry(const FString& Filename, int32 MaxRetries = 3)
{
    for (int32 Attempt = 0; Attempt < MaxRetries; ++Attempt)
    {
        if (FFileHelper::LoadFileToString(DataString, *Filename))
        {
            // Success
            KalkiLog::System(
                FString::Printf(TEXT("Loaded file on attempt %d: %s"), Attempt + 1, *Filename)
            );
            return true;
        }

        // Failed, log and retry
        KalkiLog::System(
            FString::Printf(TEXT("Failed to load file (attempt %d/%d): %s"),
                Attempt + 1, MaxRetries, *Filename),
            EKalkiLogSeverity::Warning
        );

        // Wait before retry
        if (Attempt < MaxRetries - 1)
        {
            FPlatformProcess::Sleep(0.1f); // 100ms delay
        }
    }

    // All retries failed
    KalkiLog::System(
        FString::Printf(TEXT("Failed to load file after %d attempts: %s"), MaxRetries, *Filename),
        EKalkiLogSeverity::Error
    );
    return false;
}
```

---

### Pattern: Graceful Degradation

**Continue with reduced functionality.**
```cpp
void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Try to initialize all features
    bool bFullyFunctional = true;

    if (!InitializeAbilities())
    {
        KalkiLog::Character(TEXT("Failed to initialize abilities - combat disabled"), EKalkiLogSeverity::Warning, this);
        bFullyFunctional = false;
    }

    if (!InitializeInventory())
    {
        KalkiLog::Character(TEXT("Failed to initialize inventory - loot disabled"), EKalkiLogSeverity::Warning, this);
        bFullyFunctional = false;
    }

    if (!InitializeStats())
    {
        KalkiLog::Character(TEXT("Failed to initialize stats - CRITICAL ERROR"), EKalkiLogSeverity::Error, this);
        // Stats are critical - can't continue
        Destroy();
        return;
    }

    if (bFullyFunctional)
    {
        KalkiLog::Character(TEXT("Character fully initialized"), EKalkiLogSeverity::Log, this);
    }
    else
    {
        KalkiLog::Character(TEXT("Character partially initialized - some features disabled"), EKalkiLogSeverity::Warning, this);
    }
}
```

---

## Common Error Patterns

### Pattern: Network Timeout

**Handle network operations that may timeout.**
```cpp
void AKalkiPlayerController::RequestServerAction()
{
    // Start timeout timer
    GetWorldTimerManager().SetTimer(
        TimeoutHandle,
        this,
        &AKalkiPlayerController::OnServerActionTimeout,
        5.0f, // 5 second timeout
        false
    );

    // Send RPC
    ServerDoAction();
}

void AKalkiPlayerController::OnServerActionComplete()
{
    // Clear timeout
    GetWorldTimerManager().ClearTimer(TimeoutHandle);

    // Handle success
}

void AKalkiPlayerController::OnServerActionTimeout()
{
    KalkiLog::Network(
        TEXT("Server action timed out after 5 seconds"),
        EKalkiLogSeverity::Warning,
        this
    );

    // Show error to player
    ShowErrorMessage(INVTEXT("Connection timeout - please try again"));

    // Reset state
    ResetActionState();
}
```

---

### Pattern: Division by Zero

**Protect against division by zero.**
```cpp
float CalculateHealthPercent(int32 CurrentHP, int32 MaxHP)
{
    if (MaxHP == 0)
    {
        KalkiLog::Character(
            TEXT("CalculateHealthPercent - MaxHP is zero"),
            EKalkiLogSeverity::Warning
        );
        return 0.0f; // Safe default
    }

    return (float)CurrentHP / (float)MaxHP;
}

// Or use FMath::Max
float CalculateDamageMultiplier(float Distance, float MaxDistance)
{
    // Ensure denominator is never zero
    float SafeMaxDistance = FMath::Max(MaxDistance, 0.01f);
    return 1.0f - (Distance / SafeMaxDistance);
}
```

---

### Pattern: Infinite Loop Protection

**Prevent infinite loops in pathfinding, iteration, etc.**

```cpp
TArray<FKalkiGridCoord> FindPath(const FKalkiGridCoord& Start, const FKalkiGridCoord& End)
{
    const int32 MaxIterations = 10000; // Safety limit
    int32 Iterations = 0;

    TArray<FKalkiGridCoord> OpenSet;
    OpenSet.Add(Start);

    while (OpenSet.Num() > 0)
    {
        // Safety check
        Iterations++;
        if (Iterations > MaxIterations)
        {
            KalkiLog::Grid(
                FString::Printf(
                    TEXT("FindPath exceeded max iterations (%d) from (%d,%d) to (%d,%d)"),
                    MaxIterations, Start.X, Start.Y, End.X, End.Y
                ),
                EKalkiLogSeverity::Error
            );
            return TArray<FKalkiGridCoord>(); // Return empty path
        }

        // Process pathfinding
        FKalkiGridCoord Current = GetLowestFScore(OpenSet);
        
        if (Current == End)
        {
            KalkiLog::Grid(
                FString::Printf(TEXT("Path found in %d iterations"), Iterations)
            );
            return ReconstructPath(Current);
        }

        // ... continue pathfinding
    }

    return TArray<FKalkiGridCoord>();
}
```

---

### Pattern: Recursive Function Protection

**Prevent stack overflow from deep recursion.**

```cpp
void ProcessHierarchy(UObject* Node, int32 Depth = 0)
{
    // Max recursion depth
    const int32 MaxDepth = 100;
    
    if (Depth > MaxDepth)
    {
        KalkiLog::System(
            FString::Printf(
                TEXT("ProcessHierarchy exceeded max depth (%d) - possible circular reference"),
                MaxDepth
            ),
            EKalkiLogSeverity::Error
        );
        return;
    }

    if (!IsValid(Node))
    {
        return;
    }

    // Process current node
    ProcessNode(Node);

    // Recurse to children
    for (UObject* Child : GetChildren(Node))
    {
        ProcessHierarchy(Child, Depth + 1);
    }
}
```

---

## User-Facing Error Messages

### Pattern: Player Feedback

**Show clear, actionable messages to players.**

```cpp
void AKalkiCharacter::TryCastAbility(int32 AbilityIndex)
{
    // Internal logging (detailed)
    KalkiLog::Ability(
        FString::Printf(TEXT("%s attempting to cast ability %d"), *GetName(), AbilityIndex),
        EKalkiLogSeverity::Log,
        this
    );

    // Check ability exists
    if (!Abilities.IsValidIndex(AbilityIndex))
    {
        KalkiLog::Ability(
            FString::Printf(TEXT("Invalid ability index: %d"), AbilityIndex),
            EKalkiLogSeverity::Error,
            this
        );
        
        // User-facing message (simple and clear)
        ShowErrorMessage(INVTEXT("Ability not found"));
        return;
    }

    UKalkiAbility* Ability = Abilities[AbilityIndex];
    
    // Check can cast
    FText FailureReason;
    if (!Ability->CanCast(FailureReason))
    {
        KalkiLog::Ability(
            FString::Printf(TEXT("Cannot cast ability: %s"), *FailureReason.ToString()),
            EKalkiLogSeverity::Log,
            this
        );

        // User-facing message (explains why)
        ShowErrorMessage(FailureReason);
        return;
    }

    // Cast ability
    Ability->Cast();
}

// Example CanCast implementation
bool UKalkiAbility::CanCast(FText& OutFailureReason) const
{
    // Check action points
    if (Character->GetActionPoints() < ActionPointCost)
    {
        OutFailureReason = FText::Format(
            INVTEXT("Not enough action points ({0}/{1})"),
            Character->GetActionPoints(),
            ActionPointCost
        );
        return false;
    }

    // Check cooldown
    if (IsOnCooldown())
    {
        OutFailureReason = FText::Format(
            INVTEXT("Ability on cooldown ({0}s remaining)"),
            FMath::CeilToInt(GetCooldownRemaining())
        );
        return false;
    }

    // Check range
    if (!IsTargetInRange())
    {
        OutFailureReason = INVTEXT("Target out of range");
        return false;
    }

    return true;
}
```

---

## Debugging Aids

### Pattern: Debug-Only Validation

**Extra validation in development builds.**

```cpp
void AKalkiCharacter::TakeDamage(int32 DamageAmount)
{
#if !UE_BUILD_SHIPPING
    // Debug-only validation
    if (DamageAmount > 9999)
    {
        KalkiLog::Combat(
            FString::Printf(TEXT("Suspiciously high damage: %d"), DamageAmount),
            EKalkiLogSeverity::Warning,
            this
        );
    }

    if (CurrentHP < 0)
    {
        KalkiLog::Combat(
            FString::Printf(TEXT("HP went negative: %d"), CurrentHP),
            EKalkiLogSeverity::Error,
            this
        );
    }
#endif

    // Production code
    if (!HasAuthority()) return;
    CurrentHP = FMath::Max(0, CurrentHP - DamageAmount);
}
```

---

### Pattern: Error Context

**Provide context for debugging.**

```cpp
void ProcessComplexOperation()
{
    KalkiLog::System(TEXT("=== ProcessComplexOperation START ==="));

    // Step 1
    KalkiLog::System(TEXT("Step 1: Loading data..."));
    if (!LoadData())
    {
        KalkiLog::System(TEXT("Step 1 FAILED: Could not load data"), EKalkiLogSeverity::Error);
        return;
    }
    KalkiLog::System(TEXT("Step 1 SUCCESS"));

    // Step 2
    KalkiLog::System(TEXT("Step 2: Processing data..."));
    if (!ProcessData())
    {
        KalkiLog::System(TEXT("Step 2 FAILED: Processing error"), EKalkiLogSeverity::Error);
        return;
    }
    KalkiLog::System(TEXT("Step 2 SUCCESS"));

    // Step 3
    KalkiLog::System(TEXT("Step 3: Saving results..."));
    if (!SaveResults())
    {
        KalkiLog::System(TEXT("Step 3 FAILED: Could not save"), EKalkiLogSeverity::Error);
        return;
    }
    KalkiLog::System(TEXT("Step 3 SUCCESS"));

    KalkiLog::System(TEXT("=== ProcessComplexOperation END (SUCCESS) ==="));
}
```

---

### Pattern: Stack Traces

**Log call stack for debugging.**

```cpp
void CriticalError()
{
    KalkiLog::System(TEXT("CRITICAL ERROR DETECTED"), EKalkiLogSeverity::Error);

#if !UE_BUILD_SHIPPING
    // Log stack trace (Debug/Development only)
    const FString StackTrace = FPlatformStackWalk::GetStackTrace();
    KalkiLog::System(
        FString::Printf(TEXT("Stack trace:\n%s"), *StackTrace),
        EKalkiLogSeverity::Error
    );
#endif
}
```

---

## Best Practices Summary

### ✅ Do This:

1. **Validate Early** - Check preconditions at function entry
2. **Early Return** - Exit as soon as error detected
3. **Log Everything** - Every error should be logged with context
4. **Check Nulls** - Always validate pointers before use
5. **Check Bounds** - Validate array indices and ranges
6. **Check Authority** - Validate server authority for multiplayer
7. **Fail Gracefully** - Provide fallbacks and defaults
8. **User Feedback** - Show clear, actionable error messages
9. **Debug Aids** - Extra validation in development builds
10. **Test Error Paths** - Test failure cases, not just success

---

### ❌ Don't Do This:

1. **Silent Failures** - Never fail without logging
2. **Assume Validity** - Don't assume pointers/indices are valid
3. **Deep Nesting** - Use early returns instead
4. **Ignore Errors** - Always handle error cases
5. **Crash in Production** - Use ensure() instead of check() for recoverable errors
6. **Generic Messages** - Provide specific, detailed error messages
7. **Hide Errors** - Don't catch and ignore exceptions
8. **Magic Numbers** - Use named constants for limits
9. **Skip Validation** - Never skip checks for performance
10. **Trust Client** - Always validate client input in multiplayer

---

## Error Handling Checklist

**For every function:**

- [ ] Check pointer parameters for null
- [ ] Validate array indices
- [ ] Check subsystem availability
- [ ] Validate input ranges
- [ ] Check multiplayer authority (if needed)
- [ ] Log errors with context
- [ ] Provide fallback/default behavior
- [ ] Early return on error
- [ ] Add debug-only validation
- [ ] Test error paths

---

## Common Mistakes

### Mistake 1: Not Checking GetWorld()

```cpp
// ❌ BAD
UWorld* World = GetWorld();
World->GetSubsystem<UKalkiGridManager>(); // Crash if World is null!

// ✅ GOOD
UWorld* World = GetWorld();
if (!World)
{
    KalkiLog::System(TEXT("GetWorld() returned null"), EKalkiLogSeverity::Error);
    return;
}

UKalkiGridManager* GridManager = World->GetSubsystem<UKalkiGridManager>();
if (!GridManager)
{
    KalkiLog::System(TEXT("GridManager not found"), EKalkiLogSeverity::Error);
    return;
}
```

---

### Mistake 2: Ignoring Return Values

```cpp
// ❌ BAD
FFileHelper::LoadFileToString(DataString, *Filename); // Ignores failure!

// ✅ GOOD
if (!FFileHelper::LoadFileToString(DataString, *Filename))
{
    KalkiLog::System(
        FString::Printf(TEXT("Failed to load file: %s"), *Filename),
        EKalkiLogSeverity::Error
    );
    return false;
}
```

---

### Mistake 3: Not Clearing Invalid References

```cpp
// ❌ BAD
if (!IsValid(TargetActor))
{
    return; // TargetActor still holds invalid pointer!
}

// ✅ GOOD
if (!IsValid(TargetActor))
{
    TargetActor = nullptr; // Clear invalid reference
    return;
}
```

---

### Mistake 4: Assuming Array Size

```cpp
// ❌ BAD
int32 FirstValue = MyArray[0]; // Crash if empty!

// ✅ GOOD
if (MyArray.Num() == 0)
{
    KalkiLog::System(TEXT("Array is empty"), EKalkiLogSeverity::Warning);
    return;
}
int32 FirstValue = MyArray[0];
```

---

### Mistake 5: Not Validating RPC Parameters

```cpp
// ❌ BAD
void ServerMoveTo_Implementation(const FKalkiGridCoord& Target)
{
    GridPosition = Target; // Trust client input - can cheat!
}

// ✅ GOOD
void ServerMoveTo_Implementation(const FKalkiGridCoord& Target)
{
    if (!IsValidCoord(Target))
    {
        KalkiLog::Network(TEXT("Invalid target coord"), EKalkiLogSeverity::Warning);
        return;
    }

    if (ManhattanDistance(GridPosition, Target) > MovementRange)
    {
        KalkiLog::Network(TEXT("Target out of movement range"), EKalkiLogSeverity::Warning);
        return;
    }

    GridPosition = Target;
}
```

---

## Testing Error Handling

### Pattern: Unit Tests for Error Cases

```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiGridManagerErrorTest,
    "Kalki.Grid.GridManager.ErrorHandling",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiGridManagerErrorTest::RunTest(const FString& Parameters)
{
    UKalkiGridManager* GridManager = NewObject<UKalkiGridManager>();

    // Test: Invalid coordinate
    FKalkiGridCoord InvalidCoord(-1, -1);
    TestFalse(
        TEXT("Invalid coordinate rejected"),
        GridManager->IsValidCoord(InvalidCoord)
    );

    // Test: Coordinate out of bounds
    FKalkiGridCoord OutOfBounds(1000, 1000);
    TestFalse(
        TEXT("Out of bounds coordinate rejected"),
        GridManager->IsValidCoord(OutOfBounds)
    );

    // Test: GetTile with invalid coord returns default
    FKalkiGridTile DefaultTile = GridManager->GetTile(InvalidCoord);
    TestEqual(
        TEXT("GetTile returns default for invalid coord"),
        DefaultTile.bWalkable,
        false
    );

    return true;
}
```

---

## Related Documents

### Foundation

- [[02_Conventions/Coding_Standards]] - Code quality standards
- [[03_Systems/Logging_System]] - Logging errors
- [[04_Implementation/Common_Patterns]] - Validation patterns

### Implementation

- [[04_Implementation/Multiplayer_Guidelines]] - Network error handling
- [[04_Implementation/Testing_Procedures]] - Testing error cases
- [[04_Implementation/Class_Templates]] - Error handling in templates

### Systems

- [[03_Systems/Grid_System]] - Error handling examples
- [[03_Systems/Combat_Log_System]] - User-facing errors

---

## Quick Reference

### Common Validation Pattern

```cpp
void YourFunction(UObject* Object, int32 Value)
{
    // Check 1: Null pointer
    if (!IsValid(Object))
    {
        KalkiLog::System(TEXT("Object is null"), EKalkiLogSeverity::Error);
        return;
    }

    // Check 2: Range validation
    if (Value < 0 || Value > 100)
    {
        KalkiLog::System(
            FString::Printf(TEXT("Value out of range: %d"), Value),
            EKalkiLogSeverity::Warning
        );
        Value = FMath::Clamp(Value, 0, 100);
    }

    // Check 3: Authority (if multiplayer)
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("No authority"), EKalkiLogSeverity::Warning);
        return;
    }

    // All checks passed - do work
    DoWork(Object, Value);
}
```

---

## Document Info

**File Path:** `Kalki_GDD/04_Implementation/Error_Handling.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate  
**Note:** Comprehensive error handling and validation guide