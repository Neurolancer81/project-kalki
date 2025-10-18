---
tags:
  - kalki/implementation
  - kalki/templates
  - kalki/code-generation
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Class Templates

## Overview
This document provides copy-paste templates for common class types in Kalki. Use these templates as starting points for new classes to ensure consistency and proper structure.

---

## Purpose

### Why Templates?

**Benefits:**
1. **Consistency** - All classes follow same structure
2. **Speed** - Copy, rename, implement
3. **Completeness** - Don't forget important boilerplate
4. **Best Practices** - Templates include proper patterns
5. **Onboarding** - New developers (or Claude) start correctly

---

## How to Use Templates

### Steps:

1. **Copy template** - Copy entire template below
2. **Find & Replace** - Replace `ClassName` with your class name
3. **Update file paths** - Fix header paths in comments
4. **Remove unused** - Delete sections you don't need
5. **Implement** - Add your specific functionality
6. **Test** - Verify compilation and functionality

---

## Template Index

1. [World Subsystem](#template-world-subsystem)
2. [Game Instance Subsystem](#template-game-instance-subsystem)
3. [Actor (Basic)](#template-actor-basic)
4. [Actor (Replicated)](#template-actor-replicated)
5. [Character (Replicated)](#template-character-replicated)
6. [Actor Component](#template-actor-component)
7. [ViewModel](#template-viewmodel)
8. [Widget (MVVM)](#template-widget-mvvm)
9. [Widget (Simple)](#template-widget-simple)
10. [Struct](#template-struct)
11. [Enum](#template-enum)
12. [Interface](#template-interface)
13. [Blueprint Function Library](#template-blueprint-function-library)

---

## Template: World Subsystem

**When to Use:** Per-world manager/system (GridManager, CombatManager, etc.)

**Files:** `KalkiClassName.h` and `KalkiClassName.cpp`

### Header (KalkiClassName.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassName.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "KalkiClassName.generated.h"

/**
 * [Brief description of what this subsystem manages]
 * 
 * Usage:
 * UKalkiClassName* Manager = GetWorld()->GetSubsystem<UKalkiClassName>();
 */
UCLASS()
class KALKI_API UKalkiClassName : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // ========================================
    // SUBSYSTEM LIFECYCLE
    // ========================================

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Check if we have authority (server/standalone)
    bool HasAuthority() const;

    // ========================================
    // PUBLIC API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void YourPublicFunction();

    // ========================================
    // EVENTS
    // ========================================

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSomethingHappened);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|ClassName")
    FOnSomethingHappened OnSomethingHappened;

private:
    // ========================================
    // INTERNAL STATE
    // ========================================

    // Your private data
    int32 SomeValue;

    // ========================================
    // INTERNAL FUNCTIONS
    // ========================================

    void YourPrivateFunction();
};
```

### Implementation (KalkiClassName.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Category/KalkiClassName.cpp

#include "Category/KalkiClassName.h"
#include "Logging/KalkiLog.h"

void UKalkiClassName::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    KalkiLog::System(TEXT("KalkiClassName initialized"));

    // Initialize your data
    SomeValue = 0;

    // Bind to events if needed
}

void UKalkiClassName::Deinitialize()
{
    KalkiLog::System(TEXT("KalkiClassName deinitialized"));

    // Cleanup

    Super::Deinitialize();
}

bool UKalkiClassName::HasAuthority() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    ENetMode NetMode = World->GetNetMode();
    return (NetMode == NM_Standalone || NetMode == NM_ListenServer || NetMode == NM_DedicatedServer);
}

void UKalkiClassName::YourPublicFunction()
{
    // Check authority if this modifies gameplay state
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("YourPublicFunction - No authority"), EKalkiLogSeverity::Warning);
        return;
    }

    // Implement function
    KalkiLog::System(TEXT("YourPublicFunction called"));

    // Broadcast event if needed
    OnSomethingHappened.Broadcast();
}

void UKalkiClassName::YourPrivateFunction()
{
    // Internal helper
}
```

---

## Template: Game Instance Subsystem

**When to Use:** Persistent state across levels (Settings, SaveGame, etc.)

### Header (KalkiClassName.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassName.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KalkiClassName.generated.h"

/**
 * [Brief description - persists across level transitions]
 * 
 * Usage:
 * UKalkiClassName* Manager = GetGameInstance()->GetSubsystem<UKalkiClassName>();
 */
UCLASS()
class KALKI_API UKalkiClassName : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // ========================================
    // SUBSYSTEM LIFECYCLE
    // ========================================

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // ========================================
    // PUBLIC API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void LoadData();

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void SaveData();

private:
    // Persistent data
    UPROPERTY()
    int32 PersistentValue;
};
```

### Implementation (KalkiClassName.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Category/KalkiClassName.cpp

#include "Category/KalkiClassName.h"
#include "Logging/KalkiLog.h"

void UKalkiClassName::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    KalkiLog::System(TEXT("KalkiClassName (GameInstance) initialized"));

    LoadData();
}

void UKalkiClassName::Deinitialize()
{
    SaveData();

    KalkiLog::System(TEXT("KalkiClassName (GameInstance) deinitialized"));

    Super::Deinitialize();
}

void UKalkiClassName::LoadData()
{
    // Load from file/config
    KalkiLog::System(TEXT("Loading data..."));
}

void UKalkiClassName::SaveData()
{
    // Save to file/config
    KalkiLog::System(TEXT("Saving data..."));
}
```

---

## Template: Actor (Basic)

**When to Use:** Simple actor without replication

### Header (KalkiClassName.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassName.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KalkiClassName.generated.h"

/**
 * [Brief description of actor's purpose]
 */
UCLASS()
class KALKI_API AKalkiClassName : public AActor
{
    GENERATED_BODY()

public:
    AKalkiClassName();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ========================================
    // COMPONENTS
    // ========================================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|ClassName")
    TObjectPtr<USceneComponent> RootComp;

    // ========================================
    // PROPERTIES
    // ========================================

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|ClassName")
    float SomeProperty = 100.0f;

    // ========================================
    // FUNCTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void YourFunction();

private:
    // Internal state
    bool bIsInitialized = false;
};
```

### Implementation (KalkiClassName.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Category/KalkiClassName.cpp

#include "Category/KalkiClassName.h"
#include "Logging/KalkiLog.h"

AKalkiClassName::AKalkiClassName()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    RootComponent = RootComp;
}

void AKalkiClassName::BeginPlay()
{
    Super::BeginPlay();

    KalkiLog::System(TEXT("KalkiClassName spawned"), EKalkiLogSeverity::Log, this);

    bIsInitialized = true;
}

void AKalkiClassName::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsInitialized)
    {
        return;
    }

    // Per-frame logic
}

void AKalkiClassName::YourFunction()
{
    KalkiLog::System(TEXT("YourFunction called"), EKalkiLogSeverity::Log, this);
}
```

---

## Template: Actor (Replicated)

**When to Use:** Actor that needs multiplayer support

### Header (KalkiClassName.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassName.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "KalkiClassName.generated.h"

/**
 * [Brief description - multiplayer-enabled actor]
 */
UCLASS()
class KALKI_API AKalkiClassName : public AActor
{
    GENERATED_BODY()

public:
    AKalkiClassName();

    // ========================================
    // REPLICATION
    // ========================================

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ========================================
    // REPLICATED PROPERTIES
    // ========================================

    UPROPERTY(ReplicatedUsing = OnRep_SomeValue, BlueprintReadOnly, Category = "Kalki|ClassName")
    int32 SomeValue;

    UFUNCTION()
    void OnRep_SomeValue();

    // ========================================
    // RPC FUNCTIONS
    // ========================================

    // Server RPC (Client → Server)
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Kalki|ClassName")
    void ServerDoSomething(int32 Parameter);

    // Client RPC (Server → Specific Client)
    UFUNCTION(Client, Reliable, Category = "Kalki|ClassName")
    void ClientShowFeedback(const FText& Message);

    // Multicast RPC (Server → All Clients)
    UFUNCTION(NetMulticast, Reliable, Category = "Kalki|ClassName")
    void MulticastPlayEffect();

private:
    int32 LastValue;
};
```

### Implementation (KalkiClassName.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Category/KalkiClassName.cpp

#include "Category/KalkiClassName.h"
#include "Logging/KalkiLog.h"

AKalkiClassName::AKalkiClassName()
{
    PrimaryActorTick.bCanEverTick = true;

    // Enable replication
    bReplicates = true;
    SetReplicateMovement(false); // We don't need movement replication

    // Initialize
    SomeValue = 0;
    LastValue = 0;
}

void AKalkiClassName::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKalkiClassName, SomeValue);
}

void AKalkiClassName::BeginPlay()
{
    Super::BeginPlay();

    KalkiLog::System(
        FString::Printf(TEXT("KalkiClassName spawned (Role: %s)"),
            HasAuthority() ? TEXT("Authority") : TEXT("Simulated")),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiClassName::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AKalkiClassName::OnRep_SomeValue()
{
    // Called on clients when SomeValue replicates
    KalkiLog::Network(
        FString::Printf(TEXT("SomeValue replicated: %d → %d"), LastValue, SomeValue),
        EKalkiLogSeverity::Log,
        this
    );

    LastValue = SomeValue;

    // Update visuals based on new value
}

void AKalkiClassName::ServerDoSomething_Implementation(int32 Parameter)
{
    // Runs on server only
    KalkiLog::Network(
        FString::Printf(TEXT("ServerDoSomething: Parameter=%d"), Parameter),
        EKalkiLogSeverity::Log,
        this
    );

    // Validate and execute
    if (Parameter > 0)
    {
        SomeValue = Parameter; // Replicates automatically
    }
}

bool AKalkiClassName::ServerDoSomething_Validate(int32 Parameter)
{
    // Validate input (prevent cheating)
    return Parameter >= 0 && Parameter <= 100;
}

void AKalkiClassName::ClientShowFeedback_Implementation(const FText& Message)
{
    // Runs on specific client only
    KalkiLog::UI(
        FString::Printf(TEXT("Feedback: %s"), *Message.ToString()),
        EKalkiLogSeverity::Log,
        this
    );

    // Show UI message
}

void AKalkiClassName::MulticastPlayEffect_Implementation()
{
    // Runs on all clients + server
    KalkiLog::System(TEXT("Playing effect"), EKalkiLogSeverity::Log, this);

    // Play sound/VFX
}
```

---

## Template: Character (Replicated)

**When to Use:** Playable/NPC character with D&D 5e stats

### Header (KalkiCharacterClassName.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Character/KalkiCharacterClassName.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "KalkiCharacterClassName.generated.h"

/**
 * [Brief description - playable character / NPC]
 */
UCLASS()
class KALKI_API AKalkiCharacterClassName : public ACharacter
{
    GENERATED_BODY()

public:
    AKalkiCharacterClassName();

    // ========================================
    // REPLICATION
    // ========================================

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ========================================
    // STATS (D&D 5e)
    // ========================================

    UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, BlueprintReadOnly, Category = "Kalki|Character|Stats")
    int32 CurrentHP;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Character|Stats")
    int32 MaxHP;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Character|Stats")
    int32 ArmorClass;

    UFUNCTION()
    void OnRep_CurrentHP();

    // ========================================
    // ACTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|Character")
    void TakeDamage(int32 DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Character")
    void Heal(int32 HealAmount);

    // ========================================
    // EVENTS
    // ========================================

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, NewHP, int32, Delta);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Character")
    FOnHealthChanged OnHealthChanged;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Character")
    FOnDeath OnDeath;

private:
    int32 LastHP;
};
```

### Implementation (KalkiCharacterClassName.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Character/KalkiCharacterClassName.cpp

#include "Character/KalkiCharacterClassName.h"
#include "Logging/KalkiLog.h"

AKalkiCharacterClassName::AKalkiCharacterClassName()
{
    PrimaryActorTick.bCanEverTick = true;

    // Enable replication
    bReplicates = true;
    SetReplicateMovement(true);

    // Default stats
    CurrentHP = 100;
    MaxHP = 100;
    ArmorClass = 10;
    LastHP = 100;
}

void AKalkiCharacterClassName::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKalkiCharacterClassName, CurrentHP);
    DOREPLIFETIME(AKalkiCharacterClassName, MaxHP);
    DOREPLIFETIME(AKalkiCharacterClassName, ArmorClass);
}

void AKalkiCharacterClassName::BeginPlay()
{
    Super::BeginPlay();

    KalkiLog::Character(
        FString::Printf(TEXT("Character spawned (Role: %s)"),
            HasAuthority() ? TEXT("Authority") : TEXT("Simulated")),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiCharacterClassName::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AKalkiCharacterClassName::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind inputs
}

void AKalkiCharacterClassName::OnRep_CurrentHP()
{
    int32 Delta = CurrentHP - LastHP;

    KalkiLog::Character(
        FString::Printf(TEXT("HP changed: %d → %d (%+d)"), LastHP, CurrentHP, Delta),
        EKalkiLogSeverity::Log,
        this
    );

    // Broadcast event
    OnHealthChanged.Broadcast(CurrentHP, Delta);

    // Check death
    if (CurrentHP <= 0 && LastHP > 0)
    {
        OnDeath.Broadcast();
    }

    LastHP = CurrentHP;
}

void AKalkiCharacterClassName::TakeDamage(int32 DamageAmount)
{
    // Server authority check
    if (!HasAuthority())
    {
        KalkiLog::Character(TEXT("TakeDamage on client - ignored"), EKalkiLogSeverity::Warning);
        return;
    }

    // Apply damage
    CurrentHP = FMath::Max(0, CurrentHP - DamageAmount);

    // Server also needs to call OnRep manually
    OnRep_CurrentHP();

    KalkiLog::Combat(
        FString::Printf(TEXT("%s took %d damage (%d/%d HP)"),
            *GetName(), DamageAmount, CurrentHP, MaxHP)
    );
}

void AKalkiCharacterClassName::Heal(int32 HealAmount)
{
    // Server authority check
    if (!HasAuthority())
    {
        KalkiLog::Character(TEXT("Heal on client - ignored"), EKalkiLogSeverity::Warning);
        return;
    }

    // Apply healing
    CurrentHP = FMath::Min(CurrentHP + HealAmount, MaxHP);

    // Server also needs to call OnRep manually
    OnRep_CurrentHP();

    KalkiLog::Combat(
        FString::Printf(TEXT("%s healed %d HP (%d/%d HP)"),
            *GetName(), HealAmount, CurrentHP, MaxHP)
    );
}
```

---

## Template: Actor Component

**When to Use:** Reusable functionality for actors

### Header (KalkiClassNameComponent.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/Components/KalkiClassNameComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KalkiClassNameComponent.generated.h"

/**
 * [Brief description of component's functionality]
 * 
 * Usage:
 * Add to actor in constructor:
 * Component = CreateDefaultSubobject<UKalkiClassNameComponent>(TEXT("ComponentName"));
 */
UCLASS(ClassGroup = (Kalki), meta = (BlueprintSpawnableComponent))
class KALKI_API UKalkiClassNameComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UKalkiClassNameComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ========================================
    // PUBLIC API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void YourFunction();

    // ========================================
    // PROPERTIES
    // ========================================

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|ClassName")
    float SomeProperty = 1.0f;

    // ========================================
    // EVENTS
    // ========================================

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSomethingHappened);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|ClassName")
    FOnSomethingHappened OnSomethingHappened;

private:
    // Helper to get owner as specific type
    template<typename T>
    T* GetOwnerAs() const
    {
        return Cast<T>(GetOwner());
    }
};
```

### Implementation (KalkiClassNameComponent.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/Components/KalkiClassNameComponent.cpp

#include "Components/KalkiClassNameComponent.h"
#include "Logging/KalkiLog.h"

UKalkiClassNameComponent::UKalkiClassNameComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UKalkiClassNameComponent::BeginPlay()
{
    Super::BeginPlay();

    KalkiLog::System(
        TEXT("KalkiClassNameComponent initialized"),
        EKalkiLogSeverity::Log,
        GetOwner()
    );

    // Validate owner
    if (!GetOwner())
    {
        KalkiLog::System(
            TEXT("KalkiClassNameComponent has no owner!"),
            EKalkiLogSeverity::Error
        );
        return;
    }
}

void UKalkiClassNameComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Per-frame logic
}

void UKalkiClassNameComponent::YourFunction()
{
    KalkiLog::System(
        TEXT("YourFunction called"),
        EKalkiLogSeverity::Log,
        GetOwner()
    );

    // Broadcast event
    OnSomethingHappened.Broadcast();
}
```

---

## Template: ViewModel

**When to Use:** MVVM UI data layer

### Header (KalkiClassNameViewModel.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/UI/Category/KalkiClassNameViewModel.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelBase.h"
#include "KalkiClassNameViewModel.generated.h"

/**
 * [Brief description of what UI data this manages]
 * 
 * Usage:
 * ViewModel = NewObject<UKalkiClassNameViewModel>(this);
 * ViewModel->Initialize();
 * Widget->SetViewModel(ViewModel);
 */
UCLASS(BlueprintType)
class KALKI_API UKalkiClassNameViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // ========================================
    // LIFECYCLE
    // ========================================

    virtual void Initialize() override;
    virtual void Shutdown() override;

    // ========================================
    // DATA
    // ========================================

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|ClassName")
    int32 SomeValue;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|ClassName")
    FString SomeText;

    // ========================================
    // FUNCTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void UpdateValue(int32 NewValue);

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    void ClearData();

private:
    // Internal state
    bool bIsInitialized = false;
};
```

### Implementation (KalkiClassNameViewModel.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/UI/Category/KalkiClassNameViewModel.cpp

#include "UI/Category/KalkiClassNameViewModel.h"
#include "Logging/KalkiLog.h"

void UKalkiClassNameViewModel::Initialize()
{
    Super::Initialize();

    KalkiLog::UI(TEXT("KalkiClassNameViewModel initialized"));

    // Initialize data
    SomeValue = 0;
    SomeText = TEXT("");

    bIsInitialized = true;
}

void UKalkiClassNameViewModel::Shutdown()
{
    KalkiLog::UI(TEXT("KalkiClassNameViewModel shutdown"));

    bIsInitialized = false;

    Super::Shutdown();
}

void UKalkiClassNameViewModel::UpdateValue(int32 NewValue)
{
    if (!bIsInitialized)
    {
        KalkiLog::UI(
            TEXT("UpdateValue - ViewModel not initialized"),
            EKalkiLogSeverity::Warning
        );
        return;
    }

    SomeValue = NewValue;
    SomeText = FString::Printf(TEXT("Value: %d"), SomeValue);

    // Notify widgets
    NotifyDataChanged();

    KalkiLog::UI(
        FString::Printf(TEXT("Value updated: %d"), SomeValue)
    );
}

void UKalkiClassNameViewModel::ClearData()
{
    SomeValue = 0;
    SomeText = TEXT("");

    NotifyDataChanged();
}
```

---

## Template: Widget (MVVM)

**When to Use:** Complex UI with ViewModel

### Header (KalkiClassNameWidget.h)
```cpp
// Copyright of V.S. Puranam and no one else

// Public/UI/Category/KalkiClassNameWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "KalkiClassNameWidget.generated.h"

// Forward declarations
class UKalkiClassNameViewModel;

/**
 * [Brief description of widget's purpose]
 * 
 * Usage:
 * Widget = CreateWidget<UKalkiClassNameWidget>(...);
 * Widget->SetViewModel(ViewModel);
 * Widget->AddToViewport();
 */
UCLASS()
class KALKI_API UKalkiClassNameWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

public:
    // ========================================
    // UI ELEMENTS (Bound in UMG)
    // ========================================

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TitleText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ValueText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ActionButton;

    // ========================================
    // FUNCTIONS
    // ========================================

    virtual void SetViewModel(UKalkiViewModelBase* InViewModel) override;

protected:
    // Override from base class
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void OnViewModelDataChanged_Implementation() override;

private:
    // ViewModel reference (typed)
    UPROPERTY()
    TObjectPtr<UKalkiClassNameViewModel> ClassNameViewModel;

    // Button handlers
    UFUNCTION()
    void OnActionButtonClicked();

    // Update display
    void UpdateDisplay();
};
```

### Implementation (KalkiClassNameWidget.cpp)
```cpp
// Copyright of V.S. Puranam and no one else

// Private/UI/Category/KalkiClassNameWidget.cpp

#include "UI/Category/KalkiClassNameWidget.h"
#include "UI/Category/KalkiClassNameViewModel.h"
#include "Logging/KalkiLog.h"

void UKalkiClassNameWidget::NativeConstruct()
{
    Super::Native```cpp
Construct();

    KalkiLog::UI(TEXT("KalkiClassNameWidget constructed"));

    // Bind button click
    if (ActionButton)
    {
        ActionButton->OnClicked.AddDynamic(this, &UKalkiClassNameWidget::OnActionButtonClicked);
    }

    // Initial display update
    UpdateDisplay();
}

void UKalkiClassNameWidget::NativeDestruct()
{
    // Unbind events
    if (ActionButton)
    {
        ActionButton->OnClicked.RemoveDynamic(this, &UKalkiClassNameWidget::OnActionButtonClicked);
    }

    KalkiLog::UI(TEXT("KalkiClassNameWidget destructed"));

    Super::NativeDestruct();
}

void UKalkiClassNameWidget::SetViewModel(UKalkiViewModelBase* InViewModel)
{
    // Call base class
    Super::SetViewModel(InViewModel);

    // Cast to specific type
    ClassNameViewModel = Cast<UKalkiClassNameViewModel>(InViewModel);
    if (!ClassNameViewModel)
    {
        KalkiLog::UI(
            TEXT("KalkiClassNameWidget - ViewModel wrong type"),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    UpdateDisplay();
}

void UKalkiClassNameWidget::OnViewModelDataChanged_Implementation()
{
    // Called when ViewModel notifies data changed
    UpdateDisplay();
}

void UKalkiClassNameWidget::OnActionButtonClicked()
{
    if (!ClassNameViewModel)
    {
        return;
    }

    KalkiLog::UI(TEXT("Action button clicked"));

    // Tell ViewModel to do something
    ClassNameViewModel->UpdateValue(ClassNameViewModel->SomeValue + 1);
}

void UKalkiClassNameWidget::UpdateDisplay()
{
    if (!ClassNameViewModel)
    {
        return;
    }

    // Update title
    if (TitleText)
    {
        TitleText->SetText(FText::FromString(TEXT("Title")));
    }

    // Update value display
    if (ValueText)
    {
        ValueText->SetText(FText::FromString(ClassNameViewModel->SomeText));
    }
}
```

---

## Template: Widget (Simple)

**When to Use:** Simple UI without ViewModel

### Header (KalkiClassNameWidget.h)

```cpp
// Copyright of V.S. Puranam and no one else

// Public/UI/Category/KalkiClassNameWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiSimpleWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "KalkiClassNameWidget.generated.h"

/**
 * [Brief description - simple widget]
 * 
 * Usage:
 * Widget = CreateWidget<UKalkiClassNameWidget>(...);
 * Widget->UpdateValue(50);
 * Widget->AddToViewport();
 */
UCLASS()
class KALKI_API UKalkiClassNameWidget : public UKalkiSimpleWidget
{
    GENERATED_BODY()

public:
    // ========================================
    // UI ELEMENTS (Bound in UMG)
    // ========================================

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ValueBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ValueText;

    // ========================================
    // FUNCTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|UI")
    void UpdateValue(float CurrentValue, float MaxValue);

protected:
    virtual void NativeConstruct() override;

private:
    FLinearColor GetColorForPercent(float Percent) const;
};
```

### Implementation (KalkiClassNameWidget.cpp)

```cpp
// Copyright of V.S. Puranam and no one else

// Private/UI/Category/KalkiClassNameWidget.cpp

#include "UI/Category/KalkiClassNameWidget.h"
#include "Logging/KalkiLog.h"

void UKalkiClassNameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    KalkiLog::UI(TEXT("KalkiClassNameWidget (Simple) constructed"));
}

void UKalkiClassNameWidget::UpdateValue(float CurrentValue, float MaxValue)
{
    if (!ValueBar || !ValueText)
    {
        return;
    }

    // Calculate percent
    float Percent = MaxValue > 0.0f ? CurrentValue / MaxValue : 0.0f;

    // Update progress bar
    ValueBar->SetPercent(Percent);
    ValueBar->SetFillColorAndOpacity(GetColorForPercent(Percent));

    // Update text
    FText ValueString = FText::FromString(
        FString::Printf(TEXT("%.0f / %.0f"), CurrentValue, MaxValue)
    );
    ValueText->SetText(ValueString);
}

FLinearColor UKalkiClassNameWidget::GetColorForPercent(float Percent) const
{
    if (Percent < 0.25f)
    {
        return FLinearColor::Red;
    }
    else if (Percent < 0.5f)
    {
        return FLinearColor::Yellow;
    }
    return FLinearColor::Green;
}
```

---

## Template: Struct

**When to Use:** Data container

### Header (KalkiClassNameTypes.h)

```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassNameTypes.h

#pragma once

#include "CoreMinimal.h"
#include "KalkiClassNameTypes.generated.h"

/**
 * [Brief description of struct's purpose]
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiStructName
{
    GENERATED_BODY()

    // ========================================
    // PROPERTIES
    // ========================================

    UPROPERTY(BlueprintReadWrite, Category = "Kalki|StructName")
    int32 SomeValue = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Kalki|StructName")
    FString SomeName;

    UPROPERTY(BlueprintReadWrite, Category = "Kalki|StructName")
    TArray<int32> SomeArray;

    // ========================================
    // CONSTRUCTORS
    // ========================================

    FKalkiStructName()
        : SomeValue(0)
        , SomeName(TEXT(""))
    {
    }

    FKalkiStructName(int32 InValue, const FString& InName)
        : SomeValue(InValue)
        , SomeName(InName)
    {
    }

    // ========================================
    // OPERATORS
    // ========================================

    bool operator==(const FKalkiStructName& Other) const
    {
        return SomeValue == Other.SomeValue && SomeName == Other.SomeName;
    }

    bool operator!=(const FKalkiStructName& Other) const
    {
        return !(*this == Other);
    }

    // ========================================
    // HELPERS
    // ========================================

    FString ToString() const
    {
        return FString::Printf(TEXT("StructName(Value=%d, Name=%s)"), SomeValue, *SomeName);
    }

    bool IsValid() const
    {
        return SomeValue > 0 && !SomeName.IsEmpty();
    }
};

// Hash function (for use in TMap/TSet)
FORCEINLINE uint32 GetTypeHash(const FKalkiStructName& Struct)
{
    return HashCombine(GetTypeHash(Struct.SomeValue), GetTypeHash(Struct.SomeName));
}
```

---

## Template: Enum

**When to Use:** Enumerated type

### Header (KalkiClassNameTypes.h)

```cpp
// Copyright of V.S. Puranam and no one else

// Public/Category/KalkiClassNameTypes.h

#pragma once

#include "CoreMinimal.h"
#include "KalkiClassNameTypes.generated.h"

/**
 * [Brief description of enum]
 */
UENUM(BlueprintType)
enum class EKalkiEnumName : uint8
{
    None        UMETA(DisplayName = "None"),
    Option1     UMETA(DisplayName = "Option 1"),
    Option2     UMETA(DisplayName = "Option 2"),
    Option3     UMETA(DisplayName = "Option 3"),
    
    MAX         UMETA(Hidden)
};

/**
 * Enum with explicit values (flags)
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EKalkiFlagEnum : uint8
{
    None        = 0         UMETA(DisplayName = "None"),
    Flag1       = 1 << 0    UMETA(DisplayName = "Flag 1"),
    Flag2       = 1 << 1    UMETA(DisplayName = "Flag 2"),
    Flag3       = 1 << 2    UMETA(DisplayName = "Flag 3"),
    All         = 0xFF      UMETA(DisplayName = "All")
};
ENUM_CLASS_FLAGS(EKalkiFlagEnum)
```

---

## Template: Interface

**When to Use:** Define contract/interface for multiple classes

### Header (KalkiInterfaceName.h)

```cpp
// Copyright of V.S. Puranam and no one else

// Public/Interfaces/KalkiInterfaceName.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KalkiInterfaceName.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UKalkiInterfaceName : public UInterface
{
    GENERATED_BODY()
};

/**
 * [Brief description of interface]
 * 
 * Usage:
 * class AMyClass : public AActor, public IKalkiInterfaceName
 * {
 *     // Implement interface functions
 * };
 */
class KALKI_API IKalkiInterfaceName
{
    GENERATED_BODY()

public:
    // ========================================
    // INTERFACE FUNCTIONS
    // ========================================

    // Pure virtual (must implement)
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kalki|InterfaceName")
    void RequiredFunction();

    // Virtual with default (optional to override)
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kalki|InterfaceName")
    bool OptionalFunction();
    virtual bool OptionalFunction_Implementation() { return true; }

    // Pure C++ function (no Blueprint)
    virtual void CppOnlyFunction() = 0;
};
```

### Implementation Example

```cpp
// In some actor that implements the interface

// Header
class KALKI_API AMyActor : public AActor, public IKalkiInterfaceName
{
    GENERATED_BODY()

public:
    // Implement required function
    virtual void RequiredFunction_Implementation() override;

    // Optionally override optional function
    virtual bool OptionalFunction_Implementation() override;

    // Implement C++ only function
    virtual void CppOnlyFunction() override;
};

// CPP
void AMyActor::RequiredFunction_Implementation()
{
    KalkiLog::System(TEXT("RequiredFunction called"));
}

bool AMyActor::OptionalFunction_Implementation()
{
    return false; // Override default
}

void AMyActor::CppOnlyFunction()
{
    // C++ only logic
}

// Usage - calling interface
void SomeFunction(AActor* Actor)
{
    if (Actor->Implements<UKalkiInterfaceName>())
    {
        IKalkiInterfaceName::Execute_RequiredFunction(Actor);
        
        bool Result = IKalkiInterfaceName::Execute_OptionalFunction(Actor);
        
        // Or cast
        IKalkiInterfaceName* Interface = Cast<IKalkiInterfaceName>(Actor);
        if (Interface)
        {
            Interface->CppOnlyFunction();
        }
    }
}
```

---

## Template: Blueprint Function Library

**When to Use:** Static utility functions for Blueprint

### Header (KalkiClassNameLibrary.h)

```cpp
// Copyright of V.S. Puranam and no one else

// Public/Utilities/KalkiClassNameLibrary.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KalkiClassNameLibrary.generated.h"

/**
 * [Brief description - static utility functions]
 * 
 * Usage (Blueprint):
 * Call functions directly as pure functions
 * 
 * Usage (C++):
 * UKalkiClassNameLibrary::FunctionName(...)
 */
UCLASS()
class KALKI_API UKalkiClassNameLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ========================================
    // PURE FUNCTIONS (no side effects)
    // ========================================

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|ClassName")
    static int32 CalculateSomething(int32 ValueA, int32 ValueB);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|ClassName")
    static bool IsValid(const FString& Value);

    // ========================================
    // UTILITY FUNCTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Kalki|ClassName")
    static void DoSomething(UObject* WorldContextObject);

    // ========================================
    // CONVERSION FUNCTIONS
    // ========================================

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|ClassName")
    static FString ConvertToString(int32 Value);
};
```

### Implementation (KalkiClassNameLibrary.cpp)

```cpp
// Copyright of V.S. Puranam and no one else

// Private/Utilities/KalkiClassNameLibrary.cpp

#include "Utilities/KalkiClassNameLibrary.h"
#include "Logging/KalkiLog.h"

int32 UKalkiClassNameLibrary::CalculateSomething(int32 ValueA, int32 ValueB)
{
    // Pure function - no side effects
    return ValueA + ValueB;
}

bool UKalkiClassNameLibrary::IsValid(const FString& Value)
{
    return !Value.IsEmpty() && Value.Len() > 0;
}

void UKalkiClassNameLibrary::DoSomething(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        KalkiLog::System(TEXT("DoSomething - Invalid WorldContextObject"), EKalkiLogSeverity::Error);
        return;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return;
    }

    // Do something with World
    KalkiLog::System(TEXT("DoSomething called"));
}

FString UKalkiClassNameLibrary::ConvertToString(int32 Value)
{
    return FString::Printf(TEXT("%d"), Value);
}
```

---

## Quick Reference Table

|Class Type|Parent Class|Use Case|Replication|
|---|---|---|---|
|World Subsystem|UWorldSubsystem|Per-world manager|No|
|Game Instance Subsystem|UGameInstanceSubsystem|Persistent manager|No|
|Actor (Basic)|AActor|Simple actor|No|
|Actor (Replicated)|AActor|Multiplayer actor|Yes|
|Character|ACharacter|Player/NPC|Yes|
|Actor Component|UActorComponent|Reusable functionality|No|
|ViewModel|UKalkiViewModelBase|UI data layer|No|
|Widget (MVVM)|UKalkiViewModelWidget|Complex UI|No|
|Widget (Simple)|UKalkiSimpleWidget|Simple UI|No|
|Struct|N/A|Data container|Optional|
|Enum|N/A|Enumerated type|N/A|
|Interface|UInterface|Contract|N/A|
|Function Library|UBlueprintFunctionLibrary|Static utilities|N/A|

---

## Related Documents

### Foundation

- [[02_Conventions/Naming_Conventions]] - How to name your classes
- [[02_Conventions/Coding_Standards]] - How to format your code
- [[04_Implementation/Common_Patterns]] - Patterns used in templates

### Implementation

- [[04_Implementation/Multiplayer_Guidelines]] - Replication templates
- [[03_Systems/MVVM_Pattern]] - ViewModel/Widget templates
- [[04_Implementation/Error_Handling]] - Error handling in templates

### Examples

- [[03_Systems/Grid_System]] - Real subsystem example
- [[03_Systems/Combat_Log_System]] - Real MVVM example

---

## Document Info

**File Path:** `Kalki_GDD/04_Implementation/Class_Templates.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Beginner  
**Note:** Copy-paste templates for all common class types