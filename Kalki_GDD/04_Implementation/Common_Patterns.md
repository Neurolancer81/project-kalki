---
tags:
  - kalki/implementation
  - kalki/patterns
  - kalki/best-practices
  - type: reference
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Common Patterns

## Overview
This document catalogs recurring design patterns, code structures, and implementation approaches used throughout the Kalki project. Following these patterns ensures consistency, maintainability, and reduces bugs.

---

## Purpose

### Why Patterns Matter
1. **Consistency** - Same problems solved the same way
2. **Maintainability** - Familiar patterns are easier to understand
3. **Reliability** - Proven solutions reduce bugs
4. **Onboarding** - New developers learn patterns once, apply everywhere
5. **Quality** - Best practices baked into patterns

---

## Pattern Categories

### Organization
1. [Subsystem Patterns](#subsystem-patterns)
2. [Actor Patterns](#actor-patterns)
3. [Component Patterns](#component-patterns)
4. [UI Patterns](#ui-patterns)
5. [Multiplayer Patterns](#multiplayer-patterns)
6. [Event Patterns](#event-patterns)
7. [Error Handling Patterns](#error-handling-patterns)
8. [Utility Patterns](#utility-patterns)

---

## Subsystem Patterns

### Pattern: World Subsystem Manager

**When to Use:**
- Per-world singleton needed
- System manages global state
- Multiple actors need to access it
- Lifetime tied to world

**Implementation:**
```cpp
// KalkiGridManager.h
UCLASS()
class KALKI_API UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // ========================================
    // SUBSYSTEM LIFECYCLE
    // ========================================

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Authority check
    bool HasGridAuthority() const;

    // ========================================
    // PUBLIC API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void CreateGrid(int32 SizeX, int32 SizeY, float TileSize, FVector Origin);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsValidCoord(const FKalkiGridCoord& Coord) const;

    // ========================================
    // EVENTS
    // ========================================

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridCreated);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Grid")
    FOnGridCreated OnGridCreated;

private:
    // Internal state
    TMap<FKalkiGridCoord, FKalkiGridTile> GridTiles;
    int32 GridSizeX = 0;
    int32 GridSizeY = 0;
};
```

**Usage:**
```cpp
// Access from anywhere
UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
if (GridManager)
{
    GridManager->CreateGrid(20, 20, 100.0f, FVector::ZeroVector);
}
```

**Benefits:**
- Automatic lifetime management
- Easy to access (GetWorld()->GetSubsystem)
- Blueprint-friendly
- No manual singleton management

---

### Pattern: Game Instance Subsystem

**When to Use:**
- State persists across level changes
- Settings management
- Save game handling
- Global configuration

**Implementation:**
```cpp
// KalkiSettingsManager.h
UCLASS()
class KALKI_API UKalkiSettingsManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Settings")
    void LoadSettings();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Settings")
    void SaveSettings();

    // Settings data
    UPROPERTY(BlueprintReadWrite, Category = "Kalki|Settings")
    float MasterVolume = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Kalki|Settings")
    bool bShowDamageNumbers = true;
};
```

**Benefits:**
- Survives level transitions
- Accessible throughout game lifetime
- Ideal for settings, save data, persistent state

---

## Actor Patterns

### Pattern: Replicated Actor

**When to Use:**
- Actor needs multiplayer support
- State must sync across clients
- Server authority required

**Implementation:**
```cpp
// KalkiCharacter.h
UCLASS()
class KALKI_API AKalkiCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AKalkiCharacter();

    // ========================================
    // REPLICATION
    // ========================================

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Replicated properties
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Character")
    int32 CurrentHP;

    UPROPERTY(ReplicatedUsing = OnRep_MaxHP, BlueprintReadOnly, Category = "Kalki|Character")
    int32 MaxHP;

    // RepNotify function
    UFUNCTION()
    void OnRep_MaxHP();

    // ========================================
    // SERVER FUNCTIONS (Authority Only)
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|Character")
    void TakeDamage(int32 DamageAmount);

    // ========================================
    // RPC FUNCTIONS
    // ========================================

    // Server RPC
    UFUNCTION(Server, Reliable, WithValidation, Category = "Kalki|Character")
    void ServerCastAbility(int32 AbilityIndex);

    // Client RPC
    UFUNCTION(Client, Reliable, Category = "Kalki|Character")
    void ClientShowDamageNumber(int32 Damage);

    // Multicast RPC
    UFUNCTION(NetMulticast, Reliable, Category = "Kalki|Character")
    void MulticastPlayHitEffect();
};
```

**Implementation (.cpp):**
```cpp
// KalkiCharacter.cpp

AKalkiCharacter::AKalkiCharacter()
{
    // Enable replication
    bReplicates = true;
    SetReplicateMovement(true);
}

void AKalkiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to all clients
    DOREPLIFETIME(AKalkiCharacter, CurrentHP);
    DOREPLIFETIME(AKalkiCharacter, MaxHP);
}

void AKalkiCharacter::OnRep_MaxHP()
{
    // Called on clients when MaxHP changes
    KalkiLog::Character(
        FString::Printf(TEXT("MaxHP replicated: %d"), MaxHP),
        EKalkiLogSeverity::Log,
        this
    );

    // Update UI
    UpdateHealthDisplay();
}

void AKalkiCharacter::TakeDamage(int32 DamageAmount)
{
    // Server authority check
    if (!HasAuthority())
    {
        KalkiLog::Character(TEXT("TakeDamage called on client - ignored"), EKalkiLogSeverity::Warning);
        return;
    }

    // Apply damage (automatically replicates)
    CurrentHP = FMath::Max(0, CurrentHP - DamageAmount);

    // Show damage on all clients
    MulticastPlayHitEffect();

    KalkiLog::Combat(
        FString::Printf(TEXT("%s took %d damage (%d/%d HP)"), 
            *GetName(), DamageAmount, CurrentHP, MaxHP)
    );
}

void AKalkiCharacter::ServerCastAbility_Implementation(int32 AbilityIndex)
{
    // Server validates and executes
    if (CanCastAbility(AbilityIndex))
    {
        CastAbility(AbilityIndex);
    }
}

bool AKalkiCharacter::ServerCastAbility_Validate(int32 AbilityIndex)
{
    // Validate input (prevent cheating)
    return AbilityIndex >= 0 && AbilityIndex < 10;
}

void AKalkiCharacter::ClientShowDamageNumber_Implementation(int32 Damage)
{
    // Runs only on specific client
    ShowDamageNumberWidget(Damage);
}

void AKalkiCharacter::MulticastPlayHitEffect_Implementation()
{
    // Runs on all clients (including server)
    PlayHitSound();
    SpawnHitParticles();
}
```

**See:** [[04_Implementation/Multiplayer_Guidelines]] for detailed multiplayer patterns

---

### Pattern: Spawnable Actor

**When to Use:**
- Actor spawned at runtime
- Needs initialization data
- May be pooled for performance

**Implementation:**
```cpp
// KalkiProjectile.h
UCLASS()
class KALKI_API AKalkiProjectile : public AActor
{
    GENERATED_BODY()

public:
    // ========================================
    // INITIALIZATION
    // ========================================

    // Initialize after spawning
    UFUNCTION(BlueprintCallable, Category = "Kalki|Projectile")
    void Initialize(AActor* InOwner, const FVector& Direction, float Speed);

    // Is initialized?
    bool IsInitialized() const { return bIsInitialized; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    bool bIsInitialized = false;
    FVector MoveDirection;
    float MoveSpeed;
};
```

**Usage:**
```cpp
// Spawn and initialize
FActorSpawnParameters SpawnParams;
SpawnParams.Owner = this;
SpawnParams.Instigator = GetInstigator();

AKalkiProjectile* Projectile = GetWorld()->SpawnActor<AKalkiProjectile>(
    ProjectileClass,
    SpawnLocation,
    SpawnRotation,
    SpawnParams
);

if (Projectile)
{
    Projectile->Initialize(this, Direction, 1000.0f);
}
```

---

## Component Patterns

### Pattern: Actor Component with Owner Access

**When to Use:**
- Reusable functionality for actors
- Needs to access owner actor
- Stateful behavior

**Implementation:**
```cpp
// KalkiStatsComponent.h
UCLASS(ClassGroup = (Kalki), meta = (BlueprintSpawnableComponent))
class KALKI_API UKalkiStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UKalkiStatsComponent();

    // ========================================
    // LIFECYCLE
    // ========================================

    virtual void BeginPlay() override;

    // ========================================
    // STATS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|Stats")
    int32 GetCurrentHP() const { return CurrentHP; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Stats")
    int32 GetMaxHP() const { return MaxHP; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Stats")
    void SetMaxHP(int32 NewMaxHP);

    // ========================================
    // ACTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|Stats")
    void TakeDamage(int32 DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Stats")
    void Heal(int32 HealAmount);

    // ========================================
    // EVENTS
    // ========================================

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, NewHP, int32, Delta);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Stats")
    FOnHealthChanged OnHealthChanged;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Stats")
    FOnDeath OnDeath;

private:
    UPROPERTY(EditAnywhere, Category = "Kalki|Stats")
    int32 MaxHP = 100;

    UPROPERTY()
    int32 CurrentHP = 100;

    // Helper to get owner as specific type
    template<typename T>
    T* GetOwnerAs() const
    {
        return Cast<T>(GetOwner());
    }
};
```

**Usage:**
```cpp
// In AKalkiCharacter
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Character")
TObjectPtr<UKalkiStatsComponent> StatsComponent;

// In Constructor
AKalkiCharacter::AKalkiCharacter()
{
    StatsComponent = CreateDefaultSubobject<UKalkiStatsComponent>(TEXT("StatsComponent"));
}

// Access
int32 HP = StatsComponent->GetCurrentHP();
StatsComponent->TakeDamage(10);
```

---

## UI Patterns

### Pattern: MVVM Widget Setup

**When to Use:**
- Complex UI with state management
- Need separation of data and display
- Multiple widgets showing same data

**See [[03_Systems/MVVM_Pattern]] for complete details.**

**Quick Template:**
```cpp
// 1. ViewModel
UCLASS(BlueprintType)
class UKalkiYourViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // Data
    UPROPERTY(BlueprintReadOnly)
    int32 SomeValue;

    // Methods
    UFUNCTION(BlueprintCallable)
    void UpdateValue(int32 NewValue)
    {
        SomeValue = NewValue;
        NotifyDataChanged();
    }
};

// 2. Widget
UCLASS()
class UKalkiYourWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ValueText;

protected:
    virtual void OnViewModelDataChanged_Implementation() override
    {
        UKalkiYourViewModel* VM = Cast<UKalkiYourViewModel>(ViewModel);
        if (VM)
        {
            ValueText->SetText(FText::AsNumber(VM->SomeValue));
        }
    }
};

// 3. Usage
ViewModel = NewObject<UKalkiYourViewModel>(this);
Widget = CreateWidget<UKalkiYourWidget>(...);
Widget->SetViewModel(ViewModel);
Widget->AddToViewport();
```

---

### Pattern: Simple Widget (No ViewModel)

**When to Use:**
- Simple display (health bar, damage number)
- No complex state
- Direct data binding

**Implementation:**
```cpp
// KalkiHealthBarWidget.h
UCLASS()
class KALKI_API UKalkiHealthBarWidget : public UKalkiSimpleWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HealthText;

    UFUNCTION(BlueprintCallable, Category = "Kalki|UI")
    void UpdateHealth(int32 CurrentHP, int32 MaxHP)
    {
        if (!HealthBar || !HealthText)
        {
            return;
        }

        float Percent = MaxHP > 0 ? (float)CurrentHP / (float)MaxHP : 0.0f;
        HealthBar->SetPercent(Percent);

        FText HealthString = FText::FromString(
            FString::Printf(TEXT("%d / %d"), CurrentHP, MaxHP)
        );
        HealthText->SetText(HealthString);

        // Color based on health
        FLinearColor BarColor = GetHealthColor(Percent);
        HealthBar->SetFillColorAndOpacity(BarColor);
    }

private:
    FLinearColor GetHealthColor(float Percent) const
    {
        if (Percent < 0.25f) return FLinearColor::Red;
        if (Percent < 0.5f) return FLinearColor::Yellow;
        return FLinearColor::Green;
    }
};
```

---

## Multiplayer Patterns

### Pattern: Server Authority with Client Prediction

**When to Use:**
- Action needs server validation
- Want responsive client feedback
- Preventing cheating

**Implementation:**
```cpp
// Character movement on grid
void AKalkiCharacter::RequestMoveTo(const FKalkiGridCoord& TargetCoord)
{
    // Client predicts immediately (responsive)
    if (!HasAuthority())
    {
        // Visual prediction only
        PredictedPosition = TargetCoord;
        UpdateVisualPosition();

        // Request server to validate
        ServerMoveTo(TargetCoord);
        return;
    }

    // Server validates and executes
    if (CanMoveTo(TargetCoord))
    {
        GridPosition = TargetCoord;
        // Replicates to all clients
    }
    else
    {
        // Tell client prediction failed
        ClientCorrectPosition(GridPosition);
    }
}

void AKalkiCharacter::ServerMoveTo_Implementation(const FKalkiGridCoord& TargetCoord)
{
    RequestMoveTo(TargetCoord);
}

bool AKalkiCharacter::ServerMoveTo_Validate(const FKalkiGridCoord& TargetCoord)
{
    // Validate input
    return IsValid Coord(TargetCoord) && IsAdjacent(GridPosition, TargetCoord);
}

void AKalkiCharacter::ClientCorrectPosition_Implementation(const FKalkiGridCoord& ServerPosition)
{
    // Server rejected, correct prediction
    PredictedPosition = ServerPosition;
    GridPosition = ServerPosition;
    UpdateVisualPosition();
}
```

---

### Pattern: Multicast Visual Effects

**When to Use:**
- Visual/audio effect needs to play on all clients
- No gameplay state change
- Cosmetic only

**Implementation:**
```cpp
void AKalkiCharacter::PlayHitEffect()
{
    if (HasAuthority())
    {
        // Server triggers multicast
        MulticastPlayHitEffect();
    }
}

void AKalkiCharacter::MulticastPlayHitEffect_Implementation()
{
    // Runs on all clients + server
    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
    }

    if (HitParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitParticles,
            GetActorLocation()
        );
    }
}
```

---

## Event Patterns

### Pattern: Delegate Declaration and Usage

**When to Use:**
- Need to notify other systems of events
- Loose coupling between systems
- Multiple listeners possible

**Implementation:**
```cpp
// Declaration (in .h)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnDamageDealt,
    AActor*, DamagedActor,
    float, DamageAmount
);

UPROPERTY(BlueprintAssignable, Category = "Kalki|Combat")
FOnDamageDealt OnDamageDealt;

// Broadcasting (in .cpp)
void AKalkiWeapon::DealDamage(AActor* Target, float Damage)
{
    // Apply damage...
    
    // Notify listeners
    OnDamageDealt.Broadcast(Target, Damage);
}

// Binding (in another class)
void AKalkiCombatManager::BeginPlay()
{
    Super::BeginPlay();

    // Find weapon
    AKalkiWeapon* Weapon = FindWeapon();
    if (Weapon)
    {
        Weapon->OnDamageDealt.AddDynamic(this, &AKalkiCombatManager::HandleDamageDealt);
    }
}

void AKalkiCombatManager::HandleDamageDealt(AActor* DamagedActor, float DamageAmount)
{
    // React to damage event
    KalkiLog::Combat(
        FString::Printf(TEXT("%s dealt %.1f damage to %s"),
            *GetName(), DamageAmount, *DamagedActor->GetName())
    );
}

// Unbinding (cleanup)
void AKalkiCombatManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (Weapon)
    {
        Weapon->OnDamageDealt.RemoveDynamic(this, &AKalkiCombatManager::HandleDamageDealt);
    }

    Super::EndPlay(EndPlayReason);
}
```

---

### Pattern: Event-Driven Updates

**When to Use:**
- Avoid polling in Tick
- React only when data changes
- Performance optimization

**Implementation:**
```cpp
// ❌ Bad: Polling in Tick
void AKalkiHealthBar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Checking every frame (expensive!)
    int32 HP = Character->GetHP();
    if (HP != LastHP)
    {
        UpdateHealthBar(HP);
        LastHP = HP;
    }
}

// ✅ Good: Event-driven
void AKalkiHealthBar::BeginPlay()
{
    Super::BeginPlay();

    // Bind once
    if (Character)
    {
        Character->OnHealthChanged.AddDynamic(this, &AKalkiHealthBar::OnHealthChanged);
    }
}

void AKalkiHealthBar::OnHealthChanged(int32 NewHP, int32 Delta)
{
    // Called only when HP actually changes
    UpdateHealthBar(NewHP);
}
```

---

## Error Handling Patterns

### Pattern: Validation with Early Return

**When to Use:**
- Function has prerequisites
- Multiple validation steps
- Clear failure points

**Implementation:**
```cpp
void UKalkiGridManager::SetElevation(const FKalkiGridCoord& Coord, float Elevation)
{
    // Check 1: Authority
    if (!HasGridAuthority())
    {
        KalkiLog::Grid(TEXT("SetElevation - No authority"), EKalkiLogSeverity::Warning);
        return;
    }

    // Check 2: Valid coordinate
    if (!IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetElevation - Invalid coord: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Error
        );
        return;
    }

    // Check 3: Tile exists
    FKalkiGridTile* Tile = GridTiles.Find(Coord);
    if (!Tile)
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetElevation - Tile not found: (%d, %d)"), Coord.X, Coord.Y),
            EKalkiLogSeverity::Error
        );
        return;
    }

    // All checks passed - do the work
    Tile->Elevation = Elevation;
    OnTileChanged.Broadcast(Coord);

    KalkiLog::Grid(
        FString::Printf(TEXT("Tile (%d, %d) elevation set to %.1f"), Coord.X, Coord.Y, Elevation)
    );
}
```

---

### Pattern: Null Pointer Checks

**When to Use:**
- Working with pointers
- Getting subsystems/actors
- Component access

**Implementation:**
```cpp
void AKalkiCharacter::CastAbility(int32 AbilityIndex)
{
    // Check: Component exists
    if (!AbilityComponent)
    {
        KalkiLog::Ability(TEXT("AbilityComponent is null"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Check: Ability exists
    UKalkiAbility* Ability = AbilityComponent->GetAbility(AbilityIndex);
    if (!Ability)
    {
        KalkiLog::Ability(
            FString::Printf(TEXT("Ability %d not found"), AbilityIndex),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    // Check: Can cast
    if (!Ability->CanCast())
    {
        KalkiLog::Ability(
            FString::Printf(TEXT("Cannot cast %s"), *Ability->GetName()),
            EKalkiLogSeverity::Log,
            this
        );
        return;
    }

    // All checks passed
    Ability->Cast();
}
```

---

### Pattern: IsValid() Check

**When to Use:**
- Checking UObject pointers
- Weak pointers
- Replicated actors

**Implementation:**
```cpp
void AKalkiCharacter::AttackTarget()
{
    // IsValid checks:
    // - Pointer is not null
    // - Object is not pending kill
    // - Object is valid for use
    if (!IsValid(TargetActor))
    {
        KalkiLog::Combat(TEXT("Invalid target"), EKalkiLogSeverity::Warning, this);
        TargetActor = nullptr;
        return;
    }

    // Safe to use
    TargetActor->TakeDamage(...);
}

// For weak pointers
void AKalkiUI::UpdateTargetInfo()
{
    if (!TargetCharacter.IsValid())
    {
        HideTargetInfo();
        return;
    }

    // Weak pointer is valid
    AActor* Target = TargetCharacter.Get();
    ShowTargetInfo(Target);
}
```

---

## Utility Patterns

### Pattern: Static Utility Class

**When to Use:**
- Pure functions (no state)
- Math helpers
- Conversion utilities
- Blueprint function libraries

**Implementation:**
```cpp
// KalkiMathLibrary.h
UCLASS()
class KALKI_API UKalkiMathLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Manhattan distance
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Math")
    static int32 ManhattanDistance(const FKalkiGridCoord& A, const FKalkiGridCoord& B)
    {
        return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
    }

    // Check if adjacent
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Math")
    static bool IsAdjacent(const FKalkiGridCoord& A, const FKalkiGridCoord& B, bool bIncludeDiagonals = false)
    {
        int32 Distance = ManhattanDistance(A, B);
        
        if (bIncludeDiagonals)
        {
            // Chebyshev distance
            int32 DX = FMath::Abs(A.X - B.X);
            int32 DY = FMath::Abs(A.Y - B.Y);
            return FMath::Max(DX, DY) == 1;
        }
        
        return Distance == 1;
    }

    // D&D 5e ability modifier
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Math|D&D")
    static int32 GetAbilityModifier(int32 AbilityScore)
    {
        return (AbilityScore - 10) / 2;
    }

    // Clamp to grid bounds
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Math")
    static FKalkiGridCoord ClampToGrid(const FKalkiGridCoord& Coord, int32 MaxX, int32 MaxY)
    {
        return FKalkiGridCoord(
            FMath::Clamp(Coord.X, 0, MaxX - 1),
            FMath::Clamp(Coord.Y, 0, MaxY - 1)
        );
    }
};
```

**Usage:**
```cpp
// In C++
int32 Distance = UKalkiMathLibrary::ManhattanDistance(CoordA, CoordB);

// In Blueprint
// Can call directly as pure function
```

---

### Pattern: Cheat Manager Commands

**When to Use:**
- Testing/debugging
- Designer tools
- Development shortcuts

**Implementation:**
```cpp
// KalkiCheatManager.h
UCLASS()
class KALKI_API UKalkiCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    // Grid commands
    UFUNCTION(Exec, Category = "Kalki|Grid")
    void CreateTestGrid(int32 SizeX, int32 SizeY);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void SetTileElevation(int32 X, int32 Y, float Elevation);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void TeleportToTile(int32 X, int32 Y);

    // Character commands
    UFUNCTION(Exec, Category = "Kalki|Character")
    void GodMode();

    UFUNCTION(Exec, Category = "Kalki|Character")
    void SetHP(int32 HP);

    UFUNCTION(Exec, Category = "Kalki|Character")
    void LevelUp();

    // Combat commands
    UFUNCTION(Exec, Category = "Kalki|Combat")
    void KillAllEnemies();

    UFUNCTION(Exec, Category = "Kalki|Combat")
    void ShowCombatDebug();
};
```

**Register in PlayerController:**
```cpp
// KalkiPlayerController.cpp
AKalkiPlayerController::AKalkiPlayerController()
{
    CheatClass = UKalkiCheatManager::StaticClass();
}
```

**Usage:**
```
// In console (~):
CreateTestGrid 20 20
SetTileElevation 10 10 200
GodMode
```

---

## Anti-Patterns

### ❌ Anti-Pattern: God Object

**Don't Do This:**
```cpp
// One class that does everything
class AKalkiGameManager : public AActor
{
    // Grid management
    void CreateGrid();
    void GetTile();
    
    // Combat management
    void StartCombat();
    void DealDamage();
    
    // Character management
    void SpawnCharacter();
    void LevelUpCharacter();
    
    // UI management
    void ShowWidget();
    void UpdateHUD();
    
    // ... 5000 lines of code ...
};
```

**Do This Instead:**
```cpp
// Separate subsystems
UKalkiGridManager - Grid only
UKalkiCombatManager - Combat only
UKalkiCharacterManager - Character spawning
AKalkiHUD - UI only
```

---

### ❌ Anti-Pattern: Magic Numbers

**Don't Do This:**
```cpp
void CalculateDamage()
{
    float Damage = BaseDamage * 1.5f; // What is 1.5?
    if (Distance > 10.0f) // What is 10?
    {
        Damage *= 0.```markdown
        Damage *= 0.5f; // What is 0.5?
    }
}
```

**Do This Instead:**

```cpp
// Constants with descriptive names
namespace KalkiCombat
{
    constexpr float CRITICAL_DAMAGE_MULTIPLIER = 1.5f;
    constexpr float MAX_EFFECTIVE_RANGE = 10.0f;
    constexpr float RANGE_FALLOFF_MULTIPLIER = 0.5f;
}

void CalculateDamage()
{
    float Damage = BaseDamage * KalkiCombat::CRITICAL_DAMAGE_MULTIPLIER;
    if (Distance > KalkiCombat::MAX_EFFECTIVE_RANGE)
    {
        Damage *= KalkiCombat::RANGE_FALLOFF_MULTIPLIER;
    }
}

// Or use config properties
UPROPERTY(EditDefaultsOnly, Category = "Kalki|Combat")
float CriticalDamageMultiplier = 1.5f;
```

---

### ❌ Anti-Pattern: Deep Nesting

**Don't Do This:**

```cpp
void ProcessAbility(UKalkiAbility* Ability)
{
    if (Ability)
    {
        if (CanCastAbility(Ability))
        {
            if (HasEnoughResources(Ability))
            {
                if (IsValidTarget(Ability->GetTarget()))
                {
                    if (!IsOnCooldown(Ability))
                    {
                        // Actually cast ability (5 levels deep!)
                        CastAbility(Ability);
                    }
                }
            }
        }
    }
}
```

**Do This Instead:**

```cpp
void ProcessAbility(UKalkiAbility* Ability)
{
    // Early returns - flat structure
    if (!Ability)
    {
        return;
    }

    if (!CanCastAbility(Ability))
    {
        return;
    }

    if (!HasEnoughResources(Ability))
    {
        return;
    }

    if (!IsValidTarget(Ability->GetTarget()))
    {
        return;
    }

    if (IsOnCooldown(Ability))
    {
        return;
    }

    // All checks passed - cast ability
    CastAbility(Ability);
}
```

---

### ❌ Anti-Pattern: Stringly-Typed Code

**Don't Do This:**

```cpp
// Using strings for types/states
FString CharacterClass = "Fighter"; // Typos! No autocomplete!

if (CharacterClass == "figher") // Typo - bug!
{
    // ...
}
```

**Do This Instead:**

```cpp
// Use enums
UENUM(BlueprintType)
enum class EKalkiCharacterClass : uint8
{
    Fighter,
    Wizard,
    Rogue,
    Cleric
};

EKalkiCharacterClass CharacterClass = EKalkiCharacterClass::Fighter;

if (CharacterClass == EKalkiCharacterClass::Fighter) // Type-safe, autocomplete!
{
    // ...
}
```

---

### ❌ Anti-Pattern: Ignoring Const Correctness

**Don't Do This:**

```cpp
// All non-const - can accidentally modify
FKalkiGridCoord GetTileCoord(FKalkiGridTile Tile) // Pass by value (copy!)
{
    Tile.bWalkable = false; // Accidentally modifying!
    return Tile.Coordinate;
}
```

**Do This Instead:**

```cpp
// Const reference - can't modify, no copy
FKalkiGridCoord GetTileCoord(const FKalkiGridTile& Tile) const
{
    // Tile.bWalkable = false; // Compiler error - good!
    return Tile.Coordinate;
}
```

---

## Decision Trees

### Should I Use a Subsystem?

```
Does it manage global/world state?
    ├─ YES → Does it persist across levels?
    │        ├─ YES → Use GameInstanceSubsystem
    │        └─ NO → Use WorldSubsystem
    └─ NO → Is it per-actor functionality?
             ├─ YES → Use ActorComponent
             └─ NO → Is it a utility?
                      ├─ YES → Static utility class
                      └─ NO → Regular class
```

---

### Should I Use MVVM for This UI?

```
Is the UI complex?
    ├─ YES → Use MVVM
    └─ NO → Does it share data with other widgets?
             ├─ YES → Use MVVM
             └─ NO → Will you need to unit test logic?
                      ├─ YES → Use MVVM
                      └─ NO → Use simple widget
```

---

### How Should I Replicate This?

```
Does it need multiplayer?
    ├─ NO → Don't replicate
    └─ YES → Is it player-specific?
             ├─ YES → PlayerController or PlayerState
             └─ NO → Is it per-character?
                      ├─ YES → Character (replicated)
                      └─ NO → GameState (replicated)
```

---

## Pattern Combinations

### Pattern: Grid-Based Character Movement

**Combines:**

- Subsystem (GridManager)
- Replication (Character position)
- Events (OnMoved)
- Component (MovementComponent)

**Implementation:**

```cpp
// 1. GridManager (Subsystem)
UCLASS()
class UKalkiGridManager : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    bool CanMoveTo(const FKalkiGridCoord& From, const FKalkiGridCoord& To) const;
    void MoveOccupant(const FKalkiGridCoord& From, const FKalkiGridCoord& To, AActor* Occupant);
};

// 2. Character (Replicated Actor)
UCLASS()
class AKalkiCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    // Replicated position
    UPROPERTY(ReplicatedUsing = OnRep_GridPosition)
    FKalkiGridCoord GridPosition;

    UFUNCTION()
    void OnRep_GridPosition();

    // Movement event
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterMoved, FKalkiGridCoord, OldPos, FKalkiGridCoord, NewPos);
    UPROPERTY(BlueprintAssignable)
    FOnCharacterMoved OnCharacterMoved;

    // Request move (with validation)
    UFUNCTION(BlueprintCallable)
    void RequestMoveTo(const FKalkiGridCoord& Target);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerMoveTo(const FKalkiGridCoord& Target);
};

// 3. MovementComponent (Component)
UCLASS()
class UKalkiGridMovementComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    void MoveAlongPath(const TArray<FKalkiGridCoord>& Path);
    void SmoothMoveTo(const FKalkiGridCoord& Target);
};

// Usage combines all patterns
void AKalkiCharacter::RequestMoveTo(const FKalkiGridCoord& Target)
{
    // Client prediction
    if (!HasAuthority())
    {
        MovementComponent->SmoothMoveTo(Target);
        ServerMoveTo(Target);
        return;
    }

    // Server validation (Subsystem)
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager || !GridManager->CanMoveTo(GridPosition, Target))
    {
        return;
    }

    // Update grid (Subsystem)
    GridManager->MoveOccupant(GridPosition, Target, this);

    // Update character (Replicated)
    FKalkiGridCoord OldPos = GridPosition;
    GridPosition = Target;

    // Notify (Event)
    OnCharacterMoved.Broadcast(OldPos, Target);

    // Visual movement (Component)
    MovementComponent->SmoothMoveTo(Target);
}
```

---

### Pattern: Combat Action with Feedback

**Combines:**

- RPC (Server validation)
- Multicast (Visual effects)
- Events (Combat log)
- Subsystem (Combat manager)

**Implementation:**

```cpp
void AKalkiCharacter::Attack(AKalkiCharacter* Target)
{
    if (!HasAuthority())
    {
        ServerAttack(Target);
        return;
    }

    // Server: Validate and calculate damage
    int32 Damage = CalculateDamage(Target);
    Target->TakeDamage(Damage);

    // Log to combat system (Subsystem)
    UKalkiCombatManager* CombatManager = GetWorld()->GetSubsystem<UKalkiCombatManager>();
    if (CombatManager)
    {
        CombatManager->LogCombatAction(this, Target, Damage);
    }

    // Visual effects on all clients (Multicast)
    MulticastPlayAttackEffect(Target->GetActorLocation());

    // Notify (Events)
    OnDamageDealt.Broadcast(Target, Damage);
}

UFUNCTION(Server, Reliable, WithValidation)
void ServerAttack_Implementation(AKalkiCharacter* Target)
{
    Attack(Target);
}

UFUNCTION(NetMulticast, Reliable)
void MulticastPlayAttackEffect_Implementation(FVector HitLocation)
{
    // Play on all clients
    PlayAttackSound();
    SpawnHitParticles(HitLocation);
}
```

---

## Testing Patterns

### Pattern: Unit Testable ViewModel

**When to Use:**

- Testing UI logic without widgets
- Automated testing
- CI/CD integration

**Implementation:**

```cpp
// Test file: KalkiCombatLogViewModel.spec.cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiCombatLogTest,
    "Kalki.UI.CombatLog.AddMessage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiCombatLogTest::RunTest(const FString& Parameters)
{
    // Arrange
    UKalkiCombatLogViewModel* ViewModel = NewObject<UKalkiCombatLogViewModel>();
    ViewModel->Initialize();

    FKalkiLogMessage TestMessage;
    TestMessage.Message = TEXT("Test damage");
    TestMessage.Severity = EKalkiLogSeverity::Log;
    TestMessage.Channel = EKalkiLogChannel::Combat;

    // Act
    ViewModel->AddLogMessage(TestMessage);

    // Assert
    TestEqual(TEXT("Has one message"), ViewModel->DisplayEntries.Num(), 1);
    TestEqual(
        TEXT("Message text matches"),
        ViewModel->DisplayEntries[0].DisplayText.ToString(),
        TEXT("Test damage")
    );
    TestEqual(
        TEXT("Severity matches"),
        ViewModel->DisplayEntries[0].Severity,
        EKalkiLogSeverity::Log
    );

    // Cleanup
    ViewModel->Shutdown();

    return true;
}
```

---

### Pattern: Mock Objects for Testing

**When to Use:**

- Testing without dependencies
- Isolating system under test

**Implementation:**

```cpp
// Mock GridManager for testing
class UMockGridManager : public UKalkiGridManager
{
public:
    virtual bool IsValidCoord(const FKalkiGridCoord& Coord) const override
    {
        // Always return true for testing
        return true;
    }

    virtual FKalkiGridTile GetTile(const FKalkiGridCoord& Coord) const override
    {
        // Return test data
        FKalkiGridTile TestTile;
        TestTile.Coordinate = Coord;
        TestTile.bWalkable = true;
        TestTile.Elevation = 0.0f;
        return TestTile;
    }
};

// Use in test
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPathfindingTest, ...)
bool FPathfindingTest::RunTest(const FString& Parameters)
{
    UMockGridManager* MockGrid = NewObject<UMockGridManager>();
    UKalkiPathfinding* Pathfinder = NewObject<UKalkiPathfinding>();

    TArray<FKalkiGridCoord> Path = Pathfinder->FindPath(
        FKalkiGridCoord(0, 0),
        FKalkiGridCoord(5, 5),
        MockGrid
    );

    TestTrue(TEXT("Path found"), Path.Num() > 0);
    return true;
}
```

---

## Performance Patterns

### Pattern: Object Pooling

**When to Use:**

- Frequently spawned/destroyed objects
- Projectiles, VFX, UI elements
- Reduce GC pressure

**Implementation:**

```cpp
// Object pool manager
UCLASS()
class UKalkiObjectPool : public UObject
{
    GENERATED_BODY()

public:
    // Initialize pool
    void Initialize(TSubclassOf<AActor> InClass, int32 InitialSize)
    {
        PooledClass = InClass;

        for (int32 i = 0; i < InitialSize; ++i)
        {
            AActor* NewObject = SpawnAndDeactivate();
            AvailableObjects.Add(NewObject);
        }
    }

    // Get object from pool
    AActor* GetObject()
    {
        if (AvailableObjects.Num() > 0)
        {
            AActor* Object = AvailableObjects.Pop();
            Object->SetActorHiddenInGame(false);
            Object->SetActorEnableCollision(true);
            ActiveObjects.Add(Object);
            return Object;
        }

        // Pool exhausted - spawn new
        AActor* NewObject = SpawnAndDeactivate();
        ActiveObjects.Add(NewObject);
        return NewObject;
    }

    // Return object to pool
    void ReturnObject(AActor* Object)
    {
        if (!Object)
        {
            return;
        }

        ActiveObjects.Remove(Object);
        Object->SetActorHiddenInGame(true);
        Object->SetActorEnableCollision(false);
        AvailableObjects.Add(Object);
    }

private:
    TSubclassOf<AActor> PooledClass;
    TArray<AActor*> AvailableObjects;
    TArray<AActor*> ActiveObjects;

    AActor* SpawnAndDeactivate()
    {
        AActor* NewObject = GetWorld()->SpawnActor<AActor>(PooledClass);
        NewObject->SetActorHiddenInGame(true);
        NewObject->SetActorEnableCollision(false);
        return NewObject;
    }
};

// Usage
void AKalkiWeapon::FireProjectile()
{
    // Get from pool instead of spawning
    AActor* Projectile = ProjectilePool->GetObject();
    Projectile->SetActorLocation(MuzzleLocation);
    // ... initialize projectile

    // Return to pool after delay
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, Projectile]()
        {
            ProjectilePool->ReturnObject(Projectile);
        },
        5.0f,
        false
    );
}
```

---

### Pattern: Cached Query Results

**When to Use:**

- Expensive calculations
- Queried frequently
- Data doesn't change often

**Implementation:**

```cpp
UCLASS()
class UKalkiPathfinding : public UObject
{
    GENERATED_BODY()

public:
    // Find path (with caching)
    TArray<FKalkiGridCoord> FindPath(
        const FKalkiGridCoord& Start,
        const FKalkiGridCoord& End
    )
    {
        // Check cache first
        FPathCacheKey Key(Start, End);
        if (TArray<FKalkiGridCoord>* CachedPath = PathCache.Find(Key))
        {
            // Cache hit
            CacheHits++;
            return *CachedPath;
        }

        // Cache miss - calculate
        CacheMisses++;
        TArray<FKalkiGridCoord> Path = CalculatePath(Start, End);

        // Store in cache
        PathCache.Add(Key, Path);

        // Limit cache size
        if (PathCache.Num() > MaxCacheSize)
        {
            // Remove oldest entry (simple FIFO)
            // In production, use LRU or similar
            PathCache.Empty();
        }

        return Path;
    }

    // Invalidate cache when grid changes
    void OnGridChanged()
    {
        PathCache.Empty();
    }

private:
    struct FPathCacheKey
    {
        FKalkiGridCoord Start;
        FKalkiGridCoord End;

        FPathCacheKey(const FKalkiGridCoord& InStart, const FKalkiGridCoord& InEnd)
            : Start(InStart), End(InEnd)
        {}

        bool operator==(const FPathCacheKey& Other) const
        {
            return Start == Other.Start && End == Other.End;
        }

        friend uint32 GetTypeHash(const FPathCacheKey& Key)
        {
            return HashCombine(GetTypeHash(Key.Start), GetTypeHash(Key.End));
        }
    };

    TMap<FPathCacheKey, TArray<FKalkiGridCoord>> PathCache;
    int32 MaxCacheSize = 100;
    int32 CacheHits = 0;
    int32 CacheMisses = 0;
};
```

---

## Summary

### Pattern Checklist

**When creating a new system, consider:**

- ✅ **Subsystem or Actor?** - Use subsystem for managers
- ✅ **Replicated?** - Plan multiplayer from the start
- ✅ **Events needed?** - Use delegates for loose coupling
- ✅ **Validation?** - Early returns with null checks
- ✅ **Logging?** - Use KalkiLog consistently
- ✅ **MVVM for UI?** - Complex UI needs ViewModels
- ✅ **Performance?** - Cache, pool, event-driven updates
- ✅ **Testing?** - Design for testability

---

## Related Documents

### Foundation

- [[02_Conventions/Coding_Standards]] - Code style and structure
- [[02_Conventions/Naming_Conventions]] - Naming patterns

### Systems

- [[03_Systems/MVVM_Pattern]] - UI pattern details
- [[03_Systems/Grid_System]] - Example system using these patterns
- [[03_Systems/Logging_System]] - Logging pattern

### Implementation

- [[04_Implementation/Class_Templates]] - Code templates
- [[04_Implementation/Multiplayer_Guidelines]] - Multiplayer patterns
- [[04_Implementation/Error_Handling]] - Error handling patterns
- [[04_Implementation/Testing_Procedures]] - Testing patterns

### Architecture

- [[01_Architecture/Module_Overview]] - System organization
- [[01_Architecture/Class_Hierarchy]] - Class relationships

---

## Document Info

**File Path:** `Kalki_GDD/04_Implementation/Common_Patterns.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate  
**Note:** Living document - patterns added as project evolves
