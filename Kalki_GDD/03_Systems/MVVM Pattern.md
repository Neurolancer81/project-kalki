---
tags:
  - kalki/pattern/mvvm
  - kalki/ui
  - kalki/architecture
  - kalki/phase-2
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# MVVM Pattern

## Overview
The MVVM (Model-View-ViewModel) pattern is used for complex UI in Kalki, separating data/logic (ViewModel) from display (View/Widget). This pattern provides clean architecture, testability, and reusability for UI systems.

---

## Purpose

### Problems Solved
1. **Tight Coupling** - Widget code mixed with business logic
2. **Hard to Test** - Can't test UI logic without widgets
3. **Difficult Reuse** - Can't share logic between multiple widgets
4. **State Management** - No clear place for UI state
5. **Multiplayer Sync** - Hard to keep UI in sync with replicated data

### Design Goals
- ✅ **Separation of Concerns** - Data vs Display
- ✅ **Testability** - ViewModel can be unit tested
- ✅ **Reusability** - Multiple widgets can share ViewModel
- ✅ **Maintainability** - Clear responsibility boundaries
- ✅ **Multiplayer-Safe** - ViewModel handles replicated data

---

## What is MVVM?

### Three Layers
```
┌─────────────────────────────────────────┐
│          Model (Game Data)              │
│  - Character stats                      │
│  - Combat state                         │
│  - Grid data                            │
└──────────────┬──────────────────────────┘
               │ (reads from)
               ↓
┌─────────────────────────────────────────┐
│      ViewModel (UI Logic & State)       │
│  - Format data for display              │
│  - Store UI-specific state              │
│  - Handle user input logic              │
│  - Notify View of changes               │
└──────────────┬──────────────────────────┘
               │ (data binding)
               ↓
┌─────────────────────────────────────────┐
│         View (Widget Display)           │
│  - Display formatted data               │
│  - Handle visual styling                │
│  - Send user input to ViewModel         │
└─────────────────────────────────────────┘
```

---

### Responsibilities

**Model (Game Data):**
- Core gameplay data
- Replicated state (multiplayer)
- Business logic
- **Examples:** Character stats, grid tiles, combat state

**ViewModel (UI Logic):**
- Transform Model data for display
- Store UI-specific state (filters, selections)
- Handle UI input logic
- Notify View when data changes
- **Examples:** CombatLogViewModel, GridInfoViewModel

**View (Widget):**
- Display data from ViewModel
- Visual styling (colors, layouts)
- User interaction (buttons, clicks)
- **Examples:** CombatLogWidget, GridInfoWidget

---

## When to Use MVVM

### Use MVVM For:

**✅ Complex UI with State:**
```cpp
// Combat log with filtering, formatting, session tracking
UKalkiCombatLogViewModel + UKalkiCombatLogWidget
```

**✅ Multiple Widgets Sharing Data:**
```cpp
// Character sheet shown in multiple places
UKalkiCharacterSheetViewModel
    ↓ (shared by)
WBP_CharacterSheet_Full
WBP_CharacterSheet_Compact
WBP_CharacterSheet_Tooltip
```

**✅ UI Needing Unit Tests:**
```cpp
// Complex filtering logic
UKalkiInventoryViewModel::FilterItems(...)
// Can test without creating widgets
```

**✅ Replicated Data Display:**
```cpp
// Multiplayer UI showing replicated state
UKalkiPartyListViewModel (reads from replicated AKalkiGameState)
```

---

### Don't Use MVVM For:

**❌ Simple Widgets:**
```cpp
// Health bar - just display a number
UKalkiSimpleHealthBarWidget (inherits UUserWidget directly)
```

**❌ Static Content:**
```cpp
// Main menu buttons - no dynamic data
UKalkiMainMenuWidget (inherits UUserWidget directly)
```

**❌ Pure Visual Effects:**
```cpp
// Damage numbers floating on screen
UKalkiDamageNumberWidget (no ViewModel needed)
```

---

## Architecture Classes

### UKalkiViewModelBase

**Type:** Base class for all ViewModels  
**Path:** `Source/Kalki/Public/UI/Common/KalkiViewModelBase.h`  
**Inherits:** UObject

**Purpose:**
- Base class for all ViewModels
- Common functionality (events, lifecycle)
- Provides notification mechanism

**Key Features:**
```cpp
UCLASS(Abstract, Blueprintable)
class KALKI_API UKalkiViewModelBase : public UObject
{
    GENERATED_BODY()

public:
    // Event for notifying widgets of data changes
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewModelDataChanged);
    
    UPROPERTY(BlueprintAssignable, Category = "Kalki|ViewModel")
    FOnViewModelDataChanged OnDataChanged;

    // Notify all bound widgets
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    void NotifyDataChanged();

    // Lifecycle
    virtual void Initialize() {}
    virtual void Shutdown() {}
};
```

---

### UKalkiViewModelWidget

**Type:** Base class for widgets using ViewModels  
**Path:** `Source/Kalki/Public/UI/Common/KalkiViewModelWidget.h`  
**Inherits:** UUserWidget

**Purpose:**
- Base class for MVVM widgets
- Handles ViewModel binding
- Automatic update on data changes

**Key Features:**
```cpp
UCLASS(Abstract)
class KALKI_API UKalkiViewModelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Set the ViewModel
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    virtual void SetViewModel(UKalkiViewModelBase* InViewModel);

    // Get the ViewModel
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    UKalkiViewModelBase* GetViewModel() const { return ViewModel; }

protected:
    // Override this to update widget when data changes
    UFUNCTION(BlueprintNativeEvent, Category = "Kalki|ViewModel")
    void OnViewModelDataChanged();

    // ViewModel reference
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|ViewModel")
    TObjectPtr<UKalkiViewModelBase> ViewModel;

private:
    // Event handler
    UFUNCTION()
    void HandleDataChanged();
};
```

---

### UKalkiSimpleWidget

**Type:** Base class for simple widgets (no ViewModel)  
**Path:** `Source/Kalki/Public/UI/Common/KalkiSimpleWidget.h`  
**Inherits:** UUserWidget

**Purpose:**
- Base class for simple widgets
- No ViewModel needed
- Direct data binding

**Usage:**
```cpp
UCLASS()
class KALKI_API UKalkiHealthBarWidget : public UKalkiSimpleWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetHealthPercent(float Percent);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthBar;
};
```

---

## Implementation Example

### Example: Combat Log System

**Complete MVVM implementation:**

---

#### 1. ViewModel (Logic & State)

**KalkiCombatLogViewModel.h:**
```cpp
// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelBase.h"
#include "UI/CombatLog/KalkiCombatLogTypes.h"
#include "KalkiCombatLogViewModel.generated.h"

/**
 * Combat Log ViewModel
 * Manages combat log data and formatting
 */
UCLASS(BlueprintType)
class KALKI_API UKalkiCombatLogViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // ========================================
    // DATA
    // ========================================

    // Display entries (what widget shows)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    TArray<FKalkiLogDisplayEntry> DisplayEntries;

    // File logging
    UPROPERTY(BlueprintReadWrite, Category = "Kalki|CombatLog")
    bool bFileLoggingEnabled = true;

    // ========================================
    // PUBLIC FUNCTIONS
    // ========================================

    // Add log message
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void AddLogMessage(const FKalkiLogMessage& LogMessage);

    // Clear log
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void ClearLog();

    // Start new session
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void StartNewSession();

    // Lifecycle
    virtual void Initialize() override;
    virtual void Shutdown() override;

private:
    // Session file
    FString CurrentSessionFile;

    // Convert to display entry
    FKalkiLogDisplayEntry CreateDisplayEntry(const FKalkiLogMessage& LogMessage);

    // File operations
    void WriteToFile(const FKalkiLogDisplayEntry& Entry);
};
```

**KalkiCombatLogViewModel.cpp:**
```cpp
#include "UI/CombatLog/KalkiCombatLogViewModel.h"
#include "Logging/KalkiLog.h"

void UKalkiCombatLogViewModel::Initialize()
{
    Super::Initialize();
    
    StartNewSession();
    
    KalkiLog::UI(TEXT("CombatLogViewModel initialized"));
}

void UKalkiCombatLogViewModel::Shutdown()
{
    if (bFileLoggingEnabled && !CurrentSessionFile.IsEmpty())
    {
        // Write session end
        FString EndMessage = FString::Printf(
            TEXT("\n=== Session End ===\nTotal Messages: %d\n"),
            DisplayEntries.Num()
        );
        FFileHelper::SaveStringToFile(
            EndMessage,
            *CurrentSessionFile,
            FFileHelper::EEncodingOptions::AutoDetect,
            &IFileManager::Get(),
            FILEWRITE_Append
        );
    }

    Super::Shutdown();
}

void UKalkiCombatLogViewModel::AddLogMessage(const FKalkiLogMessage& LogMessage)
{
    // Convert to display entry
    FKalkiLogDisplayEntry DisplayEntry = CreateDisplayEntry(LogMessage);
    
    // Add to list
    DisplayEntries.Add(DisplayEntry);
    
    // Limit to 100 messages
    const int32 MaxMessages = 100;
    if (DisplayEntries.Num() > MaxMessages)
    {
        DisplayEntries.RemoveAt(0, DisplayEntries.Num() - MaxMessages);
    }
    
    // Write to file
    if (bFileLoggingEnabled)
    {
        WriteToFile(DisplayEntry);
    }
    
    // Notify widget
    NotifyDataChanged();
}

FKalkiLogDisplayEntry UKalkiCombatLogViewModel::CreateDisplayEntry(const FKalkiLogMessage& LogMessage)
{
    FKalkiLogDisplayEntry Entry;
    
    // Set display text
    Entry.DisplayText = FText::FromString(LogMessage.Message);
    
    // Set color based on severity
    switch (LogMessage.Severity)
    {
        case EKalkiLogSeverity::Log:
            Entry.Color = FLinearColor::White;
            break;
        case EKalkiLogSeverity::Warning:
            Entry.Color = FLinearColor(1.0f, 1.0f, 0.3f);
            break;
        case EKalkiLogSeverity::Error:
            Entry.Color = FLinearColor(1.0f, 0.3f, 0.3f);
            break;
    }
    
    // Copy data
    Entry.Severity = LogMessage.Severity;
    Entry.Channel = LogMessage.Channel;
    Entry.OriginalMessage = LogMessage;
    
    return Entry;
}

void UKalkiCombatLogViewModel::WriteToFile(const FKalkiLogDisplayEntry& Entry)
{
    if (CurrentSessionFile.IsEmpty())
    {
        return;
    }

    FString FormattedMessage = FString::Printf(
        TEXT("[%s] [%s] [%s] %s\n"),
        *FDateTime::Now().ToString(TEXT("%H:%M:%S")),
        *UEnum::GetValueAsString(Entry.Channel),
        *UEnum::GetValueAsString(Entry.Severity),
        *Entry.DisplayText.ToString()
    );

    FFileHelper::SaveStringToFile(
        FormattedMessage,
        *CurrentSessionFile,
        FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(),
        FILEWRITE_Append
    );
}

void UKalkiCombatLogViewModel::ClearLog()
{
    DisplayEntries.Empty();
    NotifyDataChanged();
    
    KalkiLog::UI(TEXT("CombatLogViewModel - Log cleared"));
}

void UKalkiCombatLogViewModel::StartNewSession()
{
    // Create log directory
    FString LogDir = FPaths::ProjectSavedDir() / TEXT("Logs/CombatLog/");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*LogDir))
    {
        PlatformFile.CreateDirectoryTree(*LogDir);
    }

    // Generate filename
    FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    CurrentSessionFile = LogDir / FString::Printf(TEXT("CombatLog_%s.txt"), *Timestamp);

    // Write header
    FString Header = FString::Printf(
        TEXT("=== Combat Log Session ===\nStarted: %s\n\n"),
        *FDateTime::Now().ToString()
    );
    FFileHelper::SaveStringToFile(Header, *CurrentSessionFile);

    KalkiLog::UI(FString::Printf(TEXT("Combat log session: %s"), *CurrentSessionFile));
}
```

---

#### 2. Widget (Display)

**KalkiCombatLogWidget.h:**
```cpp
// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "KalkiCombatLogWidget.generated.h"

// Forward declarations
class UKalkiCombatLogViewModel;

/**
 * Combat Log Widget
 * Displays combat log messages
 */
UCLASS()
class KALKI_API UKalkiCombatLogWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

public:
    // ========================================
    // UI ELEMENTS (Bound in UMG)
    // ========================================

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> MessageScrollBox;

    // ========================================
    // FUNCTIONS
    // ========================================

    // Override SetViewModel to cast to specific type
    virtual void SetViewModel(UKalkiViewModelBase* InViewModel) override;

protected:
    // Override from base class
    virtual void OnViewModelDataChanged_Implementation() override;

    // Widget lifecycle
    virtual void NativeConstruct() override;

private:
    // ViewModel reference (typed)
    UPROPERTY()
    TObjectPtr<UKalkiCombatLogViewModel> CombatLogViewModel;

    // Update display
    void UpdateMessageDisplay();
    void ScrollToBottom();
};
```

**KalkiCombatLogWidget.cpp:**
```cpp
#include "UI/CombatLog/KalkiCombatLogWidget.h"
#include "UI/CombatLog/KalkiCombatLogViewModel.h"
#include "Components/TextBlock.h"
#include "Logging/KalkiLog.h"

void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create ViewModel if not set
    if (!CombatLogViewModel)
    {
        CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
        CombatLogViewModel->Initialize();
        
        // Set as base ViewModel
        SetViewModel(CombatLogViewModel);
        
        KalkiLog::UI(TEXT("CombatLogWidget - ViewModel created"));
    }

    UpdateMessageDisplay();
}

void UKalkiCombatLogWidget::SetViewModel(UKalkiViewModelBase* InViewModel)
{
    // Call base class
    Super::SetViewModel(InViewModel);

    // Cast to specific type
    CombatLogViewModel = Cast<UKalkiCombatLogViewModel>(InViewModel);
    if (!CombatLogViewModel)
    {
        KalkiLog::UI(
            TEXT("CombatLogWidget - ViewModel wrong type"),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    UpdateMessageDisplay();
}

void UKalkiCombatLogWidget::OnViewModelDataChanged_Implementation()
{
    // Called when ViewModel notifies data changed
    UpdateMessageDisplay();
}

void UKalkiCombatLogWidget::UpdateMessageDisplay()
{
    if (!CombatLogViewModel || !MessageScrollBox)
    {
        return;
    }

    // Clear existing messages
    MessageScrollBox->ClearChildren();

    // Add all display entries
    for (const FKalkiLogDisplayEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        // Create text block
        UTextBlock* MessageText = NewObject<UTextBlock>(this);
        MessageText->SetText(Entry.DisplayText);
        MessageText->SetColorAndOpacity(FSlateColor(Entry.Color));

        // Font settings
        FSlateFontInfo FontInfo = MessageText->GetFont();
        FontInfo.Size = 12;
        MessageText->SetFont(FontInfo);

        // Add to scroll box
        MessageScrollBox->AddChild(MessageText);
    }

    // Scroll to bottom
    ScrollToBottom();
}

void UKalkiCombatLogWidget::ScrollToBottom()
{
    if (MessageScrollBox)
    {
        MessageScrollBox->ScrollToEnd();
    }
}
```

---

## Usage Patterns

### Pattern 1: Creating ViewModel + Widget

**In C++ (Manual Creation):**
```cpp
void AKalkiHUD::CreateCombatLog()
{
    // Create ViewModel
    CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
    CombatLogViewModel->Initialize();

    // Create Widget
    if (CombatLogWidgetClass)
    {
        CombatLogWidget = CreateWidget<UKalkiCombatLogWidget>(
            GetOwningPlayerController(),
            CombatLogWidgetClass
        );

        // Bind ViewModel to Widget
        CombatLogWidget->SetViewModel(CombatLogViewModel);

        // Add to viewport
        CombatLogWidget->AddToViewport();
    }
}
```

**In Blueprint:**
```
1. Create ViewModel (Construct Object from Class)
2. Initialize ViewModel
3. Create Widget (Create Widget)
4. Set ViewModel on Widget
5. Add to Viewport
```

---

### Pattern 2: Sharing ViewModel Between Widgets
```cpp
// Multiple widgets sharing same ViewModel
void AKalkiHUD::CreateCharacterUI()
{
    // Create ONE ViewModel
    CharacterViewModel = NewObject<UKalkiCharacterViewModel>(this);

    // Create multiple widgets
    CharacterSheet = CreateWidget<UKalkiCharacterSheetWidget>(...);
    CharacterSheet->SetViewModel(CharacterViewModel);

    CharacterTooltip = CreateWidget<UKalkiCharacterTooltipWidget>(...);
    CharacterTooltip->SetViewModel(CharacterViewModel);

    CharacterMiniInfo = CreateWidget<UKalkiCharacterMiniWidget>(...);
    CharacterMiniInfo->SetViewModel(CharacterViewModel);

    // All three widgets show same data, stay in sync
}
```

---

### Pattern 3: ViewModel Updates Model
```cpp
// User clicks button in widget
void UKalkiInventoryWidget::OnEquipButtonClicked()
{
    // Tell ViewModel
    InventoryViewModel->EquipItem(SelectedItemIndex);
}

// ViewModel handles logic
void UKalkiInventoryViewModel::EquipItem(int32 ItemIndex)
{
    // Validate
    if (!Items.IsValidIndex(ItemIndex))
    {
        return;
    }

    // Update Model (game state)
    Character->EquipItem(Items[ItemIndex]);

    // Update ViewModel state
    SelectedItemIndex = -1;

    // Notify widgets
    NotifyDataChanged();
}
```

---

## Data Binding Patterns

### One-Way Binding (Model → ViewModel → View)
```
Model Changes
    ↓
ViewModel Observes
    ↓
ViewModel Updates Internal State
    ↓
ViewModel Notifies (OnDataChanged)
    ↓
View Updates Display
```

**Example:**
```cpp
// Model changes
Character->TakeDamage(10);

// ViewModel observes (via event or polling)
void UKalkiCharacterViewModel::OnCharacterDamaged(float Damage)
{
    // Update ViewModel state
    CurrentHP = Character->GetHP();
    MaxHP = Character->GetMaxHP();

    // Notify widgets
    NotifyDataChanged();
}

// View updates (automatic)
void UKalkiCharacterSheetWidget::OnViewModelDataChanged_Implementation()
{
    HealthBar->SetPercent(CharacterViewModel->CurrentHP / CharacterViewModel->MaxHP);
    HealthText->SetText(FText::FromString(
        FString::Printf(TEXT("%d / %d"), CurrentHP, MaxHP)
    ));
}
```

---

### Two-Way Binding (View ↔ ViewModel ↔ Model)
```
User Input in View
    ↓
View Calls ViewModel Method
    ↓
ViewModel Updates Model
    ↓
Model Notifies ViewModel
    ↓
ViewModel Notifies View
    ↓
View Updates Display
```

**Example:**
```cpp
// User edits text field
void UKalkiCharacterCreatorWidget::OnNameTextChanged(const FText& NewText)
{
    // Tell ViewModel
    CreatorViewModel->SetCharacterName(NewText.ToString());
}

// ViewModel updates Model
void UKalkiCharacterCreatorViewModel::SetCharacterName(const FString& NewName)
{
    // Validate
    if (NewName.Len() > 20)
    {
        return; // Name too long
    }

    // Update internal state
    CharacterName = NewName;

    // Notify (other widgets might care)
    NotifyDataChanged();
}
```

---

## Advanced Patterns

### Pattern: Filtered Display

**ViewModel with filtering:**
```cpp
UCLASS()
class UKalkiInventoryViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // All items
    UPROPERTY(BlueprintReadOnly)
    TArray<FKalkiItemData> AllItems;

    // Filtered items (what widget shows)
    UPROPERTY(BlueprintReadOnly)
    TArray<FKalkiItemData> DisplayedItems;

    // Filter state
    UPROPERTY(BlueprintReadWrite)
    EKalkiItemType FilterType = EKalkiItemType::All;

    UPROPERTY(BlueprintReadWrite)
    FString SearchText;

    // Apply filter
    UFUNCTION(BlueprintCallable)
    void ApplyFilter()
    {
        DisplayedItems.Empty();

        for (const FKalkiItemData& Item : AllItems)
        {
            // Check type filter
            if (FilterType != EKalkiItemType::All && Item.Type != FilterType)
            {
                continue;
            }

            // Check search text
            if (!SearchText.IsEmpty() && !Item.Name.Contains(SearchText))
            {
                continue;
            }

            // Passed filters
            DisplayedItems.Add(Item);
        }

        // Notify widgets
        NotifyDataChanged();
    }
};
```

---

### Pattern: Computed Properties

**ViewModel with derived data:**
```cpp
UCLASS()
class UKalkiCharacterViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // Raw data
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentHP = 100;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxHP = 100;

    // Computed property
    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const
    {
        if (MaxHP == 0) return 0.0f;
        return (float)CurrentHP / (float)MaxHP;
    }

    UFUNCTION(BlueprintCallable)
    bool IsBloodied() const
    {
        return GetHealthPercent() < 0.5f;
    }

    UFUNCTION(BlueprintCallable)
    FLinearColor GetHealthColor() const
    {
        float Percent = GetHealthPercent();
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
};
```

---

### Pattern: Async Operations

**ViewModel handling async loading:**
```cpp
UCLASS()
class UKalkiSaveGameViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    bool bIsLoading = false;

    UPROPERTY(BlueprintReadOnly)
    TArray<FKalkiSaveSlotInfo> SaveSlots;

    UFUNCTION(BlueprintCallable)
    void LoadSaveSlots()
    {
        bIsLoading = true;
        NotifyDataChanged(); // Widget shows "Loading..."

        // Async load
        AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]()
        {
            // Load from disk (slow operation)
            TArray<FKalkiSaveSlotInfo> LoadedSlots = LoadSaveSlotsFromDisk();

            // Return to game thread
            AsyncTask(ENamedThreads::GameThread, [this, LoadedSlots]()
            {
                SaveSlots = LoadedSlots;
                bIsLoading = false;
                NotifyDataChanged(); // Widget shows results
            });
        });
    }
};
```

---

## Testing ViewModels

### Unit Test Example

**Test ViewModel without widgets:**
```cpp
// In test file:
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FKalkiCombatLogViewModelTest,
    "Kalki.UI.CombatLogViewModel.AddMessage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FKalkiCombatLogViewModelTest::RunTest(const FString& Parameters)
{
    // Create ViewModel
    UKalkiCombatLogViewModel* ViewModel = NewObject<UKalkiCombatLogViewModel>();
    ViewModel->Initialize();

    // Test initial state
    TestEqual(TEXT("Starts empty"), ViewModel->DisplayEntries.Num(), 0);

    // Add message
    FKalkiLogMessage TestMessage;
    TestMessage.Message = TEXT("Test message");
    TestMessage.Severity = EKalkiLogSeverity::Log;
    TestMessage.Channel = EKalkiLogChannel::Combat;

    ViewModel->AddLogMessage(TestMessage);

    // Test result
    TestEqual(TEXT("Has one message"), ViewModel->DisplayEntries.Num(), 1);
    TestEqual(TEXT("Message text correct"),
        ViewModel->DisplayEntries[0].DisplayText.ToString(),
        TEXT("Test message")
    );

    return true;
}
```

---

## Blueprint Integration

### Accessing ViewModel in Blueprint

**Widget Blueprint (WBP_CombatLog):**
```
Event Graph:

[Event Construct]
    ↓
[Get ViewModel] (from parent class)
    ↓
[Cast to KalkiCombatLogViewModel]
    ↓
[Set Variable] (store as typed reference)
    ↓
[Update Display]
```

**Bind to data changed event:**
```
[Event Construct]
    ↓
[Get ViewModel]
    ↓
[Bind Event to OnDataChanged]
    ↓
[Event: OnDataChanged]
    ↓
[Update Display]
```

---

## Performance Considerations

### ViewModel Performance

**✅ Good Practices:**
```cpp
// Cache computed values
void UKalkiCharacterViewModel::UpdateStats()
{
    // Compute once
    CachedHealthPercent = (float)CurrentHP / (float)MaxHP;
    CachedDamageBonus = CalculateDamageBonus(); // Expensive

    // Notify once
    NotifyDataChanged();
}

// Widget just reads cached values (fast)
HealthBar->SetPercent(ViewModel->CachedHealthPercent);
```

**❌ Bad Practices:**
```cpp
// Don't compute in Tick
void UKalkiCharacterWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    // BAD! Recomputes every frame
    float Percent = (float)ViewModel->CurrentHP / (float)ViewModel->MaxHP;
    HealthBar->SetPercent(Percent);
}
```

---

### Notification Batching

**Batch multiple changes:**
```cpp
// Bad: Multiple notifications
void UKalkiCharacterViewModel::TakeDamage(int32 Damage)
{
    CurrentHP -= Damage;
    NotifyDataChanged(); // Notification 1

    if (CurrentHP <= 0)
    {
        bIsAlive = false;
        NotifyDataChanged(); // Notification 2
    }
}

// Good: Single notification
void UKalkiCharacterViewModel::TakeDamage(int32 Damage)
{
    CurrentHP -= Damage;

    if (CurrentHP <= 0)
    {
        bIsAlive = false;
    }

    NotifyDataChanged(); // Single notification
}
```

---

## Common Pitfalls

### Pitfall 1: Circular References

**❌ Don't:**
```cpp
// ViewModel stores strong reference to Widget
UPROPERTY()
TObjectPtr<UKalkiCombatLogWidget> Widget; // BAD!

// Widget stores strong reference to ViewModel
UPROPERTY()
TObjectPtr<UKalkiCombatLogViewModel> ViewModel; // Also has this

// Result: Circular reference, memory leak
```

**✅ Do:**
```cpp
// Widget → ViewModel (strong reference)
UPROPERTY()
TObjectPtr<UKalkiCombatLogViewModel> ViewModel; // OK

// ViewModel → Widget (weak reference or delegate)
FOnViewModelDataChanged OnDataChanged; // Delegate (OK)
// or
TWeakObjectPtr<UKalkiCombatLogWidget> Widget; // Weak (OK)
### Pitfall 2: Forgetting to Notify

**❌ Don't:**

cpp

```cpp
void UKalkiInventoryViewModel::AddItem(const FKalkiItemData& Item)
{
    Items.Add(Item);
    // FORGOT: NotifyDataChanged();
    // Widget never updates!
}
```

**✅ Do:**

cpp

```cpp
void UKalkiInventoryViewModel::AddItem(const FKalkiItemData& Item)
{
    Items.Add(Item);
    NotifyDataChanged(); // Widget updates
}
```

---

### Pitfall 3: ViewModel Depends on Widget

**❌ Don't:**

cpp

```cpp
// ViewModel shouldn't know about widget specifics
void UKalkiCombatLogViewModel::UpdateScrollPosition()
{
    // BAD! ViewModel knows about widget internals
    Widget->ScrollToBottom();
}
```

**✅ Do:**

cpp

```cpp
// ViewModel just manages data
void UKalkiCombatLogViewModel::AddMessage(...)
{
    DisplayEntries.Add(...);
    NotifyDataChanged(); // Widget decides how to handle
}

// Widget handles its own display
void UKalkiCombatLogWidget::OnViewModelDataChanged_Implementation()
{
    UpdateMessageDisplay();
    ScrollToBottom(); // Widget's responsibility
}
```

---

### Pitfall 4: Expensive Operations in Getter

**❌ Don't:**

cpp

```cpp
// Called many times per frame
UFUNCTION(BlueprintCallable)
TArray<FKalkiItemData> GetFilteredItems() const
{
    TArray<FKalkiItemData> Result;
    // Expensive filtering every call!
    for (const FKalkiItemData& Item : AllItems)
    {
        if (PassesFilter(Item))
        {
            Result.Add(Item);
        }
    }
    return Result;
}
```

**✅ Do:**

cpp

```cpp
// Cache filtered results
UPROPERTY(BlueprintReadOnly)
TArray<FKalkiItemData> FilteredItems;

// Update cache when filter changes
void ApplyFilter()
{
    FilteredItems.Empty();
    for (const FKalkiItemData& Item : AllItems)
    {
        if (PassesFilter(Item))
        {
            FilteredItems.Add(Item);
        }
    }
    NotifyDataChanged();
}

// Getter is cheap
const TArray<FKalkiItemData>& GetFilteredItems() const
{
    return FilteredItems;
}
```

---

### Pitfall 5: Not Cleaning Up Events

**❌ Don't:**

cpp

```cpp
void UKalkiCharacterWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Bind to character events
    Character->OnHealthChanged.AddDynamic(this, &UKalkiCharacterWidget::OnHealthChanged);
    
    // FORGOT: Unbind in NativeDestruct
    // Memory leak if character outlives widget
}
```

**✅ Do:**

cpp

```cpp
void UKalkiCharacterWidget::NativeConstruct()
{
    Super::NativeConstruct();
    Character->OnHealthChanged.AddDynamic(this, &UKalkiCharacterWidget::OnHealthChanged);
}

void UKalkiCharacterWidget::NativeDestruct()
{
    if (Character)
    {
        Character->OnHealthChanged.RemoveDynamic(this, &UKalkiCharacterWidget::OnHealthChanged);
    }
    
    Super::NativeDestruct();
}
```

---

## Real-World Examples

### Example 1: Grid Info Tooltip

**ViewModel:**

cpp

```cpp
UCLASS()
class UKalkiGridInfoViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // Current tile being inspected
    UPROPERTY(BlueprintReadOnly)
    FKalkiGridCoord CurrentCoord;

    // Tile properties
    UPROPERTY(BlueprintReadOnly)
    float Elevation = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bWalkable = true;

    UPROPERTY(BlueprintReadOnly)
    int32 MovementCost = 1;

    // Occupant info (future)
    UPROPERTY(BlueprintReadOnly)
    FString OccupantName;

    // Update from grid
    void UpdateFromGrid(const FKalkiGridCoord& Coord)
    {
        CurrentCoord = Coord;
        
        UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
        if (!GridManager || !GridManager->IsValidCoord(Coord))
        {
            return;
        }

        FKalkiGridTile Tile = GridManager->GetTile(Coord);
        Elevation = Tile.Elevation;
        bWalkable = Tile.bWalkable;
        
        if (Tile.IsOccupied())
        {
            OccupantName = Tile.Occupant->GetName();
        }
        else
        {
            OccupantName = TEXT("");
        }

        NotifyDataChanged();
    }
};
```

**Widget:**

cpp

```cpp
UCLASS()
class UKalkiGridInfoWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CoordinateText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ElevationText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WalkableText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> OccupantText;

protected:
    virtual void OnViewModelDataChanged_Implementation() override
    {
        UKalkiGridInfoViewModel* GridViewModel = Cast<UKalkiGridInfoViewModel>(ViewModel);
        if (!GridViewModel)
        {
            return;
        }

        // Update coordinate
        CoordinateText->SetText(FText::FromString(
            FString::Printf(TEXT("(%d, %d)"), 
                GridViewModel->CurrentCoord.X, 
                GridViewModel->CurrentCoord.Y)
        ));

        // Update elevation
        ElevationText->SetText(FText::FromString(
            FString::Printf(TEXT("Elevation: %.0f"), GridViewModel->Elevation)
        ));

        // Update walkable
        WalkableText->SetText(FText::FromString(
            GridViewModel->bWalkable ? TEXT("Walkable") : TEXT("Blocked")
        ));
        WalkableText->SetColorAndOpacity(
            GridViewModel->bWalkable ? FLinearColor::Green : FLinearColor::Red
        );

        // Update occupant
        if (GridViewModel->OccupantName.IsEmpty())
        {
            OccupantText->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            OccupantText->SetVisibility(ESlateVisibility::Visible);
            OccupantText->SetText(FText::FromString(
                FString::Printf(TEXT("Occupied by: %s"), *GridViewModel->OccupantName)
            ));
        }
    }
};
```

**Usage:**

cpp

```cpp
// In GridVisualizer or PlayerController
void OnTileHovered(const FKalkiGridCoord& Coord)
{
    GridInfoViewModel->UpdateFromGrid(Coord);
    // Widget automatically updates via OnViewModelDataChanged
}
```

---

### Example 2: Character Sheet

**ViewModel:**

cpp

```cpp
UCLASS()
class UKalkiCharacterSheetViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // Character reference
    UPROPERTY()
    TWeakObjectPtr<AKalkiCharacter> Character;

    // Cached stats (updated from character)
    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly)
    int32 Level = 1;

    UPROPERTY(BlueprintReadOnly)
    FString ClassName;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentHP = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxHP = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 ArmorClass = 10;

    // D&D 5e ability scores
    UPROPERTY(BlueprintReadOnly)
    int32 Strength = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Dexterity = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Constitution = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Intelligence = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Wisdom = 10;

    UPROPERTY(BlueprintReadOnly)
    int32 Charisma = 10;

    // Bind to character
    void BindToCharacter(AKalkiCharacter* InCharacter)
    {
        if (!InCharacter)
        {
            return;
        }

        Character = InCharacter;

        // Bind to character events
        InCharacter->OnHealthChanged.AddDynamic(this, &UKalkiCharacterSheetViewModel::OnHealthChanged);
        InCharacter->OnStatsChanged.AddDynamic(this, &UKalkiCharacterSheetViewModel::OnStatsChanged);

        // Initial update
        UpdateFromCharacter();
    }

    void UpdateFromCharacter()
    {
        if (!Character.IsValid())
        {
            return;
        }

        CharacterName = Character->GetCharacterName();
        Level = Character->GetLevel();
        ClassName = Character->GetClassName();
        CurrentHP = Character->GetCurrentHP();
        MaxHP = Character->GetMaxHP();
        ArmorClass = Character->GetArmorClass();

        Strength = Character->GetAbilityScore(EKalkiAbility::Strength);
        Dexterity = Character->GetAbilityScore(EKalkiAbility::Dexterity);
        Constitution = Character->GetAbilityScore(EKalkiAbility::Constitution);
        Intelligence = Character->GetAbilityScore(EKalkiAbility::Intelligence);
        Wisdom = Character->GetAbilityScore(EKalkiAbility::Wisdom);
        Charisma = Character->GetAbilityScore(EKalkiAbility::Charisma);

        NotifyDataChanged();
    }

    // Computed properties
    UFUNCTION(BlueprintCallable)
    int32 GetAbilityModifier(int32 AbilityScore) const
    {
        return (AbilityScore - 10) / 2;
    }

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const
    {
        if (MaxHP == 0) return 0.0f;
        return (float)CurrentHP / (float)MaxHP;
    }

private:
    UFUNCTION()
    void OnHealthChanged(float NewHP, float Delta)
    {
        CurrentHP = FMath::RoundToInt(NewHP);
        NotifyDataChanged();
    }

    UFUNCTION()
    void OnStatsChanged()
    {
        UpdateFromCharacter();
    }
};
```

---

### Example 3: Ability Bar

**ViewModel:**

cpp

````cpp
UCLASS()
class UKalkiAbilityBarViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // Ability slots
    UPROPERTY(BlueprintReadOnly)
    TArray<FKalkiAbilitySlot> AbilitySlots;

    // Selected slot
    UPROPERTY(BlueprintReadOnly)
    int32 SelectedSlotIndex = -1;

    // Update from character
    void UpdateFromCharacter(AKalkiCharacter* Character)
    {
        if (!Character)
        {
            return;
        }

        AbilitySlots.Empty();

        // Get abilities from character
        TArray<UKalkiAbility*> Abilities = Character->GetAbilities();
        for (UKalkiAbility* Ability : Abilities)
        {
            FKalkiAbilitySlot Slot;
            Slot.Ability = Ability;
            Slot.Icon = Ability->GetIcon();
            Slot.Name = Ability->GetName();
            Slot.IsOnCooldown = Ability->IsOnCooldown();
            Slot.CooldownRemaining = Ability->GetCooldownRemaining();
            Slot.CanUse = Ability->CanUse();

            AbilitySlots.Add(Slot);
        }

        NotifyDataChanged();
    }

    // Select slot
    UFUNCTION(BlueprintCallable)
    void SelectSlot(int32 SlotIndex)
    {
        if (!AbilitySlots.IsValidIndex(SlotIndex))
        {
            return;
        }

        SelectedSlotIndex = SlotIndex;
        NotifyDataChanged();
    }

    // Get selected ability
    UFUNCTION(BlueprintCallable)
    UKalkiAbility* GetSelectedAbility() const
    {
        if (!AbilitySlots.IsValidIndex(SelectedSlotIndex))
        {
            return nullptr;
        }

        return AbilitySlots[SelectedSlotIndex].Ability;
    }
};
```

---

## Best Practices Summary

### ✅ Do This:

1. **Separate Concerns** - ViewModel = logic, Widget = display
2. **Cache Expensive Computations** - Don't recalculate every frame
3. **Batch Notifications** - Single NotifyDataChanged() for multiple changes
4. **Use Weak References** - ViewModel → Model (avoid circular refs)
5. **Test ViewModels** - Unit test without creating widgets
6. **Share ViewModels** - Multiple widgets can bind to same ViewModel
7. **Clean Up Events** - Unbind in NativeDestruct/Shutdown

---

### ❌ Don't Do This:

1. **Mix Logic in Widget** - Keep widgets dumb (display only)
2. **ViewModel Depends on Widget** - ViewModel shouldn't know about widget internals
3. **Forget NotifyDataChanged** - Always notify after data changes
4. **Store Strong Widget References in ViewModel** - Use delegates/weak refs
5. **Expensive Getters** - Cache computed values
6. **Update Every Tick** - Use event-driven updates
7. **Create Circular References** - ViewModel ↔ Widget strong refs

---

## Decision Tree

### Should I Use MVVM?
```
Is the UI complex? (filtering, state, multiple views)
    ├─ YES → Use MVVM
    └─ NO → Is there shared data between widgets?
            ├─ YES → Use MVVM
            └─ NO → Will I need to unit test this?
                    ├─ YES → Use MVVM
                    └─ NO → Use simple widget (UKalkiSimpleWidget)
````

---

## Related Documents

### Core Systems

- [[03_Systems/Combat_Log_System]] - Full MVVM example
- [[03_Systems/Logging_System]] - Model layer (data source)

### Architecture

- [[01_Architecture/Module_Overview]] - UI subsystem structure
- [[01_Architecture/Class_Hierarchy]] - Widget class hierarchy

### Implementation

- [[04_Implementation/Common_Patterns]] - UI patterns
- [[04_Implementation/Class_Templates]] - ViewModel/Widget templates
- [[04_Implementation/Testing_Procedures]] - Testing ViewModels

### Conventions

- [[02_Conventions/Naming_Conventions]] - ViewModel/Widget naming
- [[02_Conventions/Coding_Standards]] - UI code style

---

## Quick Reference

### Creating New MVVM Widget

**1. Create ViewModel:**

cpp

```cpp
// YourViewModel.h
UCLASS(BlueprintType)
class UKalkiYourViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    // Your data properties

    UFUNCTION(BlueprintCallable)
    void YourMethod()
    {
        // Update data
        NotifyDataChanged();
    }
};
```

**2. Create Widget:**

cpp

```cpp
// YourWidget.h
UCLASS()
class UKalkiYourWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

protected:
    virtual void OnViewModelDataChanged_Implementation() override
    {
        // Update display from ViewModel
    }
};
```

**3. Create Blueprint:**

- Inherit from `KalkiYourWidget`
- Design visual layout
- Bind widget references (BindWidget)

**4. Use It:**

cpp

```cpp
// Create and bind
ViewModel = NewObject<UKalkiYourViewModel>(this);
Widget = CreateWidget<UKalkiYourWidget>(...);
Widget->SetViewModel(ViewModel);
Widget->AddToViewport();
```

---

## Document Info

**File Path:** `Kalki_GDD/03_Systems/MVVM_Pattern.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate  
**Note:** Foundation pattern for all complex UI in Kalki