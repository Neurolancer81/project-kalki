---
tags:
  - kalki/api
  - kalki/ui
  - kalki/mvvm
  - type: api-reference
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# KalkiCombatLogViewModel API Reference

## Overview
Complete API reference for the UKalkiCombatLogViewModel class. This document provides detailed information about all public functions, properties, events, and usage patterns for the combat log system.

---

## Purpose

### What is KalkiCombatLogViewModel?

**KalkiCombatLogViewModel** is a ViewModel that manages combat log data for UI display. It provides:
- Message collection and filtering
- Real-time log updates
- Channel-based filtering
- Severity-based filtering
- Combat session grouping
- Auto-scrolling to latest messages
- File logging (optional)
- MVVM data binding for widgets

**See also:** [[03_Systems/Combat_Log_System]] for architectural details and design patterns.

---

## Quick Start

### Basic Usage
```cpp
#include "UI/ViewModels/KalkiCombatLogViewModel.h"

// Create ViewModel
UKalkiCombatLogViewModel* ViewModel = NewObject<UKalkiCombatLogViewModel>();
ViewModel->Initialize();

// Messages automatically captured from logging system

// Get display entries
TArray<FKalkiCombatLogEntry>& Entries = ViewModel->DisplayEntries;

// Filter by channel
ViewModel->SetChannelFilter(EKalkiLogChannel::Combat);

// Filter by severity
ViewModel->SetMinimumSeverity(EKalkiLogSeverity::Warning);

// Clear filters
ViewModel->ClearFilters();

// Save to file
ViewModel->StartNewSession();

// Clean up
ViewModel->Shutdown();
```

---

## Class Overview

### UKalkiCombatLogViewModel

**Type:** UObject (ViewModel)  
**Path:** `Source/Kalki/Public/UI/ViewModels/KalkiCombatLogViewModel.h`  
**Parent:** `UKalkiViewModelBase`

**Lifetime:** Created when widget created, destroyed when widget destroyed

**Authority:** Works on all machines (server + clients)

**Purpose:**
- Collect log messages from logging system
- Filter and format messages for display
- Provide data binding for UI widgets
- Manage combat session logging
- Save logs to disk (optional)

---

## Lifecycle

### Initialize()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
virtual void Initialize() override;
```

**Description:**
Initializes the ViewModel. Binds to logging system to receive messages.

**Example:**
```cpp
// In widget construction
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create ViewModel
    CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
    CombatLogViewModel->Initialize();

    // Bind to updates
    CombatLogViewModel->OnDataChanged.AddDynamic(this, &UKalkiCombatLogWidget::OnLogUpdated);
}
```

**Actions:**
1. Registers with logging system
2. Sets up message callback
3. Initializes display entry array
4. Resets filters to default
5. Starts listening for log messages

**Notes:**
- Must call before using ViewModel
- Only call once per instance
- Automatically starts capturing messages
- Override to add custom initialization

---

### Shutdown()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
virtual void Shutdown() override;
```

**Description:**
Cleans up the ViewModel. Unregisters from logging system.

**Example:**
```cpp
// In widget destruction
void UKalkiCombatLogWidget::NativeDestruct()
{
    if (CombatLogViewModel)
    {
        CombatLogViewModel->Shutdown();
        CombatLogViewModel = nullptr;
    }

    Super::NativeDestruct();
}
```

**Actions:**
1. Unregisters from logging system
2. Closes any open log files
3. Clears display entries
4. Unbinds all delegates

**Notes:**
- Always call when done with ViewModel
- Safe to call multiple times
- Prevents memory leaks
- Widget destruction should call this

---

## Properties

### DisplayEntries

**Signature:**
```cpp
UPROPERTY(BlueprintReadOnly, Category = "Kalki|UI|ViewModel")
TArray<FKalkiCombatLogEntry> DisplayEntries;
```

**Type:** Array of combat log entries

**Description:**
The filtered and formatted messages currently displayed in the UI.

**Example:**
```cpp
// In widget update
void UKalkiCombatLogWidget::OnLogUpdated()
{
    // Clear list
    LogListView->ClearListItems();

    // Add all display entries
    for (const FKalkiCombatLogEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        UKalkiLogEntryData* Data = NewObject<UKalkiLogEntryData>();
        Data->Entry = Entry;
        LogListView->AddItem(Data);
    }

    // Scroll to bottom
    LogListView->ScrollToBottom();
}
```

**Structure:**
```cpp
struct FKalkiCombatLogEntry
{
    FText DisplayText;           // Formatted message for display
    EKalkiLogSeverity Severity;  // Message severity
    FLinearColor Color;          // Display color
    FDateTime Timestamp;         // When message was logged
    int32 CombatSessionID;       // Session ID (-1 if not in combat)
};
```

**Notes:**
- Automatically updated when messages added
- Already filtered by current filters
- Read-only (don't modify directly)
- Triggers `OnDataChanged` when updated

---

### MaxDisplayEntries

**Signature:**
```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|UI|ViewModel")
int32 MaxDisplayEntries;
```

**Type:** Integer

**Default:** `100`

**Description:**
Maximum number of messages to keep in DisplayEntries. Older messages removed when limit exceeded.

**Example:**
```cpp
// Increase limit for detailed logging
CombatLogViewModel->MaxDisplayEntries = 500;

// Reduce for performance
CombatLogViewModel->MaxDisplayEntries = 50;

// Unlimited (not recommended)
CombatLogViewModel->MaxDisplayEntries = -1;
```

**Notes:**
- Prevents unbounded memory growth
- Oldest messages removed first (FIFO)
- Setting to -1 = unlimited (use with caution)
- 100-200 is good balance between history and performance

---

## Message Management

### AddLogMessage()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void AddLogMessage(const FKalkiLogMessage& Message);
```

**Parameters:**
- `Message` - Log message to add

**Description:**
Adds a log message to the display. Automatically called by logging system callback.

**Example:**
```cpp
// Usually called automatically, but can call manually
FKalkiLogMessage Message(
    EKalkiLogChannel::Combat,
    EKalkiLogSeverity::Log,
    TEXT("Manual log entry")
);

CombatLogViewModel->AddLogMessage(Message);
```

**Actions:**
1. Checks if message passes filters
2. Converts to display entry
3. Adds to DisplayEntries array
4. Enforces MaxDisplayEntries limit
5. Broadcasts OnDataChanged event
6. Writes to file if logging enabled

**Notes:**
- Usually called automatically by logging system
- Can call manually for custom messages
- Respects current filters
- Thread-safe

---

### ClearMessages()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void ClearMessages();
```

**Description:**
Clears all messages from the display.

**Example:**
```cpp
// Clear log at start of new combat
void UKalkiCombatManager::OnCombatStart()
{
    CombatLogViewModel->ClearMessages();
    CombatLogViewModel->StartNewSession();
}

// Clear log button
void UKalkiCombatLogWidget::OnClearButtonClicked()
{
    CombatLogViewModel->ClearMessages();
}
```

**Actions:**
1. Empties DisplayEntries array
2. Broadcasts OnDataChanged event

**Notes:**
- Does not affect captured messages (only display)
- Does not close log file
- Instant operation
- Triggers UI update

---

## Filtering

### SetChannelFilter()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void SetChannelFilter(EKalkiLogChannel Channel);
```

**Parameters:**
- `Channel` - Channel to filter by

**Description:**
Shows only messages from the specified channel.

**Example:**
```cpp
// Show only combat messages
CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Combat);

// Show only movement messages
CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Movement);

// Show only abilities
CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Abilities);
```

**Actions:**
1. Sets active channel filter
2. Rebuilds DisplayEntries with only matching messages
3. Broadcasts OnDataChanged event

**Notes:**
- Only one channel filter active at a time
- Use `ClearChannelFilter()` to show all channels
- Filters are OR logic (channel OR severity)
- Reapplies filter to all captured messages

---

### ClearChannelFilter()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void ClearChannelFilter();
```

**Description:**
Removes channel filter, showing messages from all channels.

**Example:**
```cpp
// Show all channels
CombatLogViewModel->ClearChannelFilter();

// Toggle filter
if (bFilterActive)
{
    CombatLogViewModel->ClearChannelFilter();
}
else
{
    CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Combat);
}
```

**Actions:**
1. Removes channel filter
2. Rebuilds DisplayEntries with all channels
3. Broadcasts OnDataChanged event

---

### SetMinimumSeverity()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void SetMinimumSeverity(EKalkiLogSeverity MinSeverity);
```

**Parameters:**
- `MinSeverity` - Minimum severity level to display

**Description:**
Shows only messages at or above the specified severity level.

**Example:**
```cpp
// Show all messages
CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Verbose);

// Show log, warning, error (hide verbose)
CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Log);

// Show only warnings and errors
CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Warning);

// Show only errors
CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Error);
```

**Severity Hierarchy:**
```
Verbose < Log < Warning < Error

SetMinimumSeverity(Warning) shows:
✅ Warning
✅ Error
❌ Log
❌ Verbose
```

**Actions:**
1. Sets minimum severity threshold
2. Rebuilds DisplayEntries with only matching messages
3. Broadcasts OnDataChanged event

**Notes:**
- Higher severity includes lower (Error includes Warning)
- Default is Verbose (shows all)
- Common to filter out Verbose in production

---

### ClearFilters()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void ClearFilters();
```

**Description:**
Removes all filters, showing all captured messages.

**Example:**
```cpp
// Reset to show everything
CombatLogViewModel->ClearFilters();

// Reset button
void UKalkiCombatLogWidget::OnResetFiltersClicked()
{
    CombatLogViewModel->ClearFilters();
}
```

**Actions:**
1. Clears channel filter
2. Resets severity to Verbose
3. Rebuilds DisplayEntries with all messages
4. Broadcasts OnDataChanged event

**Notes:**
- Equivalent to calling both clear functions
- Shows all captured messages
- Useful for "show all" button

---

### GetActiveChannelFilter()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
EKalkiLogChannel GetActiveChannelFilter() const;
```

**Returns:** Currently active channel filter, or invalid if none

**Description:**
Gets the current channel filter.

**Example:**
```cpp
EKalkiLogChannel Filter = CombatLogViewModel->GetActiveChannelFilter();

if (Filter == EKalkiLogChannel::Combat)
{
    // Combat filter active
    CombatButton->SetColorAndOpacity(FLinearColor::Green);
}
```

---

### GetMinimumSeverity()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
EKalkiLogSeverity GetMinimumSeverity() const;
```

**Returns:** Current minimum severity level

**Description:**
Gets the current severity filter.

**Example:**
```cpp
EKalkiLogSeverity MinSeverity = CombatLogViewModel->GetMinimumSeverity();

// Update UI to show active filter
if (MinSeverity == EKalkiLogSeverity::Warning)
{
    WarningButton->SetColorAndOpacity(FLinearColor::Yellow);
}
```

---

## Combat Sessions

### StartNewSession()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void StartNewSession();
```

**Description:**
Starts a new combat logging session. Creates a new log file if file logging enabled.

**Example:**
```cpp
// Start session when combat begins
void UKalkiCombatManager::OnCombatStart()
{
    CombatLogViewModel->StartNewSession();
    
    KalkiLog::Combat(TEXT("=== Combat Session Started ==="));
}

// Auto-increment session files
void AGameMode::StartNewBattle(const FString& BattleName)
{
    CombatLogViewModel->StartNewSession();
    
    KalkiLog::Combat(
        FString::Printf(TEXT("Battle: %s"), *BattleName)
    );
}
```

**Actions:**
1. Generates new session ID
2. Creates new log file (if enabled)
3. Writes session header
4. Sets current session active

**File Naming:**
```
SavedDir/Logs/CombatLog/CombatLog_2025-01-09_14-23-45.txt
```

**Notes:**
- Call at start of each combat encounter
- Creates timestamped file
- Previous session file closed automatically
- Can have multiple sessions per play session

---

### EndCurrentSession()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void EndCurrentSession();
```

**Description:**
Ends the current combat logging session. Closes log file if open.

**Example:**
```cpp
// End session when combat ends
void UKalkiCombatManager::OnCombatEnd()
{
    KalkiLog::Combat(TEXT("=== Combat Session Ended ==="));
    
    CombatLogViewModel->EndCurrentSession();
}
```

**Actions:**
1. Writes session footer
2. Closes log file
3. Resets current session ID

**Notes:**
- Safe to call even if no session active
- Flushes any buffered writes
- File immediately available for reading

---

### GetCurrentSessionID()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
int32 GetCurrentSessionID() const;
```

**Returns:** Current session ID, or -1 if no active session

**Description:**
Gets the ID of the current combat session.

**Example:**
```cpp
int32 SessionID = CombatLogViewModel->GetCurrentSessionID();

if (SessionID >= 0)
{
    // Session active
    SessionLabel->SetText(
        FText::Format(INVTEXT("Session {0}"), SessionID)
    );
}
else
{
    // No active session
    SessionLabel->SetText(INVTEXT("No active session"));
}
```

---

## File Logging

### EnableFileLogging()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, Category = "Kalki|UI|ViewModel")
void EnableFileLogging(bool bEnable);
```

**Parameters:**
- `bEnable` - Whether to enable file logging

**Description:**
Enables or disables saving log messages to disk.

**Example:**
```cpp
// Enable file logging
CombatLogViewModel->EnableFileLogging(true);

// Disable file logging
CombatLogViewModel->EnableFileLogging(false);

// Toggle
bool bCurrentlyEnabled = CombatLogViewModel->IsFileLoggingEnabled();
CombatLogViewModel->EnableFileLogging(!bCurrentlyEnabled);
```

**File Location:**
```
{Project}/Saved/Logs/CombatLog/CombatLog_YYYY-MM-DD_HH-MM-SS.txt
```

**Notes:**
- Disabled by default (performance)
- Enable for debugging or player reports
- Files persist between sessions
- Creates directory if doesn't exist

---

### IsFileLoggingEnabled()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
bool IsFileLoggingEnabled() const;
```

**Returns:** `true` if file logging enabled, `false` otherwise

**Description:**
Checks if file logging is currently enabled.

**Example:**
```cpp
if (CombatLogViewModel->IsFileLoggingEnabled())
{
    FileLoggingIndicator->SetVisibility(ESlateVisibility::Visible);
}
else
{
    FileLoggingIndicator->SetVisibility(ESlateVisibility::Collapsed);
}
```

---

### GetCurrentLogFilePath()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
FString GetCurrentLogFilePath() const;
```

**Returns:** Full path to current log file, or empty string if not logging

**Description:**
Gets the file path of the current log file.

**Example:**
```cpp
FString LogPath = CombatLogViewModel->GetCurrentLogFilePath();

if (!LogPath.IsEmpty())
{
    // Show in UI
    FilePathText->SetText(FText::FromString(LogPath));
    
    // Or open in explorer
    FPlatformProcess::ExploreFolder(*FPaths::GetPath(LogPath));
}
```

---

## Events

### OnDataChanged

**Signature:**
```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatLogDataChanged);

UPROPERTY(BlueprintAssignable, Category = "Kalki|UI|ViewModel")
FOnCombatLogDataChanged OnDataChanged;
```

**Fired When:** DisplayEntries array is modified

**Parameters:** None

**Description:**
Broadcast when the display data changes (new message, filter change, etc.).

**Example:**
```cpp
// C++ Binding
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    CombatLogViewModel->OnDataChanged.AddDynamic(
        this,
        &UKalkiCombatLogWidget::OnLogDataChanged
    );
}

void UKalkiCombatLogWidget::OnLogDataChanged()
{
    // Rebuild list view
    UpdateLogDisplay();
    
    // Auto-scroll to bottom
    if (bAutoScroll)
    {
        LogListView->ScrollToBottom();
    }
}

void UKalkiCombatLogWidget::NativeDestruct()
{
    if (CombatLogViewModel)
    {
        CombatLogViewModel->OnDataChanged.RemoveDynamic(
            this,
            &UKalkiCombatLogWidget::OnLogDataChanged
        );
    }
    
    Super::NativeDestruct();
}
```

**Blueprint Usage:**
```
Event OnDataChanged
  → Clear List
  → For Each Entry in DisplayEntries
    → Add to List View
  → Scroll to Bottom
```

**Triggers:**
- New message added
- Message cleared
- Filter changed
- Session started/ended

---

## Utility Functions

### GetMessageCount()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
int32 GetMessageCount() const;
```

**Returns:** Number of messages currently displayed

**Description:**
Gets the count of messages in DisplayEntries.

**Example:**
```cpp
int32 Count = CombatLogViewModel->GetMessageCount();

MessageCountText->SetText(
    FText::Format(INVTEXT("Messages: {0}"), Count)
);

// Check if empty
if (Count == 0)
{
    EmptyMessageWidget->SetVisibility(ESlateVisibility::Visible);
}
```

---

### GetTotalCapturedMessages()

**Signature:**
```cpp
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|UI|ViewModel")
int32 GetTotalCapturedMessages() const;
```

**Returns:** Total number of messages captured (including filtered)

**Description:**
Gets the total count of all messages captured, including those filtered out.

**Example:**
```cpp
int32 Total = CombatLogViewModel->GetTotalCapturedMessages();
int32 Displayed = CombatLogViewModel->GetMessageCount();
int32 Filtered = Total - Displayed;

FilterStatusText->SetText(
    FText::Format(
        INVTEXT("Showing {0} of {1} messages ({2} filtered)"),
        Displayed,
        Total,
        Filtered
    )
);
```

---

## Usage Patterns

### Pattern: Basic Combat Log Widget
```cpp
// Header
UCLASS()
class UKalkiCombatLogWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UListView* LogListView;

    UPROPERTY(meta = (BindWidget))
    UButton* ClearButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MessageCountText;

    UPROPERTY()
    UKalkiCombatLogViewModel* CombatLogViewModel;

    UFUNCTION()
    void OnLogDataChanged();

    UFUNCTION()
    void OnClearButtonClicked();
};

// Implementation
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create ViewModel
    CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
    CombatLogViewModel->Initialize();

    // Bind to ViewModel events
    CombatLogViewModel->OnDataChanged.AddDynamic(
        this,
        &UKalkiCombatLogWidget::OnLogDataChanged
    );

    // Bind button
    if (ClearButton)
    {
        ClearButton->OnClicked.AddDynamic(
            this,
            &UKalkiCombatLogWidget::OnClearButtonClicked
        );
    }

    // Initial display
    OnLogDataChanged();
}

void UKalkiCombatLogWidget::NativeDestruct()
{
    // Clean up ViewModel
    if (CombatLogViewModel)
    {
        CombatLogViewModel->OnDataChanged.RemoveDynamic(
            this,
            &UKalkiCombatLogWidget::OnLogDataChanged
        );
        
        CombatLogViewModel->Shutdown();
        CombatLogViewModel = nullptr;
    }

    // Unbind button
    if (ClearButton)
    {
        ClearButton->OnClicked.RemoveDynamic(
            this,
            &UKalkiCombatLogWidget::OnClearButtonClicked
        );
    }

    Super::NativeDestruct();
}

void UKalkiCombatLogWidget::OnLogDataChanged()
{
    if (!LogListView || !CombatLogViewModel)
    {
        return;
    }

    // Clear list
    LogListView->ClearListItems();

    // Add all entries
    for (const FKalkiCombatLogEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        // Create data object
        UKalkiLogEntryData* Data = NewObject<UKalkiLogEntryData>(this);
        Data->Entry = Entry;
        
        // Add to list
        LogListView->AddItem(Data);
    }

    // Update count
    if (MessageCountText)
    {
        MessageCountText->SetText(
            FText::Format(
                INVTEXT("{0} messages"),
                CombatLogViewModel->GetMessageCount()
            )
        );
    }

    // Auto-scroll to bottom
    LogListView->ScrollToBottom();
}

void UKalkiCombatLogWidget::OnClearButtonClicked()
{
    if (CombatLogViewModel)
    {
        CombatLogViewModel->ClearMessages();
    }
}
```

---

### Pattern: Channel Filter Buttons
```cpp
// Header
UCLASS()
class UKalkiCombatLogWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* AllChannelsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CombatButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MovementButton;

    UPROPERTY(meta = (BindWidget))
    UButton* AbilitiesButton;

    UPROPERTY()
    UKalkiCombatLogViewModel* CombatLogViewModel;

    UFUNCTION()
    void OnAllChannelsClicked();

    UFUNCTION()
    void OnCombatClicked();

    UFUNCTION()
    void OnMovementClicked();

    UFUNCTION()
    void OnAbilitiesClicked();

    void UpdateFilterButtons();
};

// Implementation
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind buttons
    if (AllChannelsButton)
    {
        AllChannelsButton->OnClicked.AddDynamic(this, &UKalkiCombatLogWidget::OnAllChannelsClicked);
    }

    if (CombatButton)
    {
        CombatButton->OnClicked.AddDynamic(this, &UKalkiCombatLogWidget::OnCombatClicked);
    }

    if (MovementButton)
    {
        MovementButton->OnClicked.AddDynamic(this, &UKalkiCombatLogWidget::OnMovementClicked);
    }

    if (AbilitiesButton)
    {
        AbilitiesButton->OnClicked.AddDynamic(this, &UKalkiCombatLogWidget::OnAbilitiesClicked);
    }

    UpdateFilterButtons();
}

void UKalkiCombatLogWidget::OnAllChannelsClicked()
{
    CombatLogViewModel->ClearChannelFilter();
    UpdateFilterButtons();
}

void UKalkiCombatLogWidget::OnCombatClicked()
{
    CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Combat);
    UpdateFilterButtons();
}

void UKalkiCombatLogWidget::OnMovementClicked()
{
    CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Movement);
    UpdateFilterButtons();
}

void UKalkiCombatLogWidget::OnAbilitiesClicked()
{
    CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Abilities);
    UpdateFilterButtons();
}

void UKalkiCombatLogWidget::UpdateFilterButtons()
{
    EKalkiLogChannel ActiveFilter = CombatLogViewModel->GetActiveChannelFilter();

    // Highlight active button
    FLinearColor ActiveColor = FLinearColor(0.2f, 0.6f, 1.0f);  // Blue
    FLinearColor InactiveColor = FLinearColor(0.5f, 0.5f, 0.5f);  // Gray

    if (AllChannelsButton)
    {
        bool bActive = (ActiveFilter == EKalkiLogChannel::MAX);  // No filter
        AllChannelsButton->SetColorAndOpacity(bActive ? ActiveColor : InactiveColor);
    }

    if (CombatButton)
    {
        bool bActive = (ActiveFilter == EKalkiLogChannel::Combat);
        CombatButton->SetColorAndOpacity(bActive ? ActiveColor : InactiveColor);
    }

    if (MovementButton)
    {
        bool bActive = (ActiveFilter == EKalkiLogChannel::Movement);
        MovementButton->SetColorAndOpacity(bActive ? ActiveColor : InactiveColor);
    }

    if (AbilitiesButton)
    {
        bool bActive = (ActiveFilter == EKalkiLogChannel::Abilities);
        AbilitiesButton->SetColorAndOpacity(bActive ? ActiveColor : InactiveColor);
    }
}
```

---

### Pattern: Severity Filter Toggle
```cpp
void UKalkiCombatLogWidget::OnSeverityFilterChanged(int32 NewIndex)
{
    switch (NewIndex)
    {
        case 0:  // All
            CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Verbose);
            break;

        case 1:  // Log+
            CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Log);
            break;

        case 2:  // Warnings+
            CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Warning);
            break;

        case 3:  // Errors Only
            CombatLogViewModel->SetMinimumSeverity(EKalkiLogSeverity::Error);
            break;
    }
}
```

---

### Pattern: Combat Session Management
```cpp
// In Combat Manager
void UKalkiCombatManager::StartCombat()
{
    // Start new log session
    CombatLogViewModel->StartNewSession();
    CombatLogViewModel->EnableFileLogging(true);

    KalkiLog::Combat(TEXT("=== Combat Started ==="));
    KalkiLog::Combat(
        FString::Printf(TEXT("Encounter: %s"), *CurrentEncounter->GetName())
    );

    // Combat logic...
}

void UKalkiCombatManager::EndCombat(bool bVictory)
{
    KalkiLog::Combat(
        FString::Printf(TEXT("=== Combat Ended: %s ==="),
            bVictory ? TEXT("Victory") : TEXT("Defeat")
        )
    );

    // End log session
    CombatLogViewModel->EndCurrentSession();
}
```

---

### Pattern: Custom Log Entry Widget
```cpp
// Custom widget for each log entry
UCLASS()
class UKalkiLogEntryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetEntry(const FKalkiCombatLogEntry& Entry)
    {
        if (MessageText```cpp
        if (MessageText)
        {
            MessageText->SetText(Entry.DisplayText);
            MessageText->SetColorAndOpacity(Entry.Color);
        }

        if (TimestampText)
        {
            FString TimeStr = Entry.Timestamp.ToString(TEXT("%H:%M:%S"));
            TimestampText->SetText(FText::FromString(TimeStr));
        }

        if (SeverityIcon)
        {
            UTexture2D* Icon = GetSeverityIcon(Entry.Severity);
            SeverityIcon->SetBrushFromTexture(Icon);
        }

        // Show session indicator if in combat
        if (SessionIndicator)
        {
            bool bInCombat = Entry.CombatSessionID >= 0;
            SessionIndicator->SetVisibility(
                bInCombat ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
            );
        }
    }

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MessageText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimestampText;

    UPROPERTY(meta = (BindWidget))
    UImage* SeverityIcon;

    UPROPERTY(meta = (BindWidget))
    UImage* SessionIndicator;

    UTexture2D* GetSeverityIcon(EKalkiLogSeverity Severity)
    {
        switch (Severity)
        {
            case EKalkiLogSeverity::Verbose:
                return VerboseIcon;
            case EKalkiLogSeverity::Log:
                return LogIcon;
            case EKalkiLogSeverity::Warning:
                return WarningIcon;
            case EKalkiLogSeverity::Error:
                return ErrorIcon;
            default:
                return nullptr;
        }
    }

    UPROPERTY(EditAnywhere)
    UTexture2D* VerboseIcon;

    UPROPERTY(EditAnywhere)
    UTexture2D* LogIcon;

    UPROPERTY(EditAnywhere)
    UTexture2D* WarningIcon;

    UPROPERTY(EditAnywhere)
    UTexture2D* ErrorIcon;
};

// Using in list view
void UKalkiCombatLogWidget::OnLogDataChanged()
{
    LogListView->ClearListItems();

    for (const FKalkiCombatLogEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        UKalkiLogEntryData* Data = NewObject<UKalkiLogEntryData>(this);
        Data->Entry = Entry;
        LogListView->AddItem(Data);
    }
}

// List entry data object
UCLASS()
class UKalkiLogEntryData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FKalkiCombatLogEntry Entry;
};
```

---

### Pattern: Auto-Scroll Control

```cpp
UCLASS()
class UKalkiCombatLogWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UListView* LogListView;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* AutoScrollCheckbox;

    UPROPERTY(meta = (BindWidget))
    UButton* ScrollToBottomButton;

    bool bAutoScroll = true;

    UFUNCTION()
    void OnAutoScrollChanged(bool bIsChecked);

    UFUNCTION()
    void OnScrollToBottomClicked();

    UFUNCTION()
    void OnLogDataChanged();
};

void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind auto-scroll checkbox
    if (AutoScrollCheckbox)
    {
        AutoScrollCheckbox->SetIsChecked(bAutoScroll);
        AutoScrollCheckbox->OnCheckStateChanged.AddDynamic(
            this,
            &UKalkiCombatLogWidget::OnAutoScrollChanged
        );
    }

    // Bind scroll button
    if (ScrollToBottomButton)
    {
        ScrollToBottomButton->OnClicked.AddDynamic(
            this,
            &UKalkiCombatLogWidget::OnScrollToBottomClicked
        );
    }

    // Bind to ViewModel
    CombatLogViewModel->OnDataChanged.AddDynamic(
        this,
        &UKalkiCombatLogWidget::OnLogDataChanged
    );
}

void UKalkiCombatLogWidget::OnAutoScrollChanged(bool bIsChecked)
{
    bAutoScroll = bIsChecked;

    if (bAutoScroll && LogListView)
    {
        LogListView->ScrollToBottom();
    }
}

void UKalkiCombatLogWidget::OnScrollToBottomClicked()
{
    if (LogListView)
    {
        LogListView->ScrollToBottom();
    }
}

void UKalkiCombatLogWidget::OnLogDataChanged()
{
    // Update list
    UpdateLogDisplay();

    // Auto-scroll if enabled
    if (bAutoScroll && LogListView)
    {
        LogListView->ScrollToBottom();
    }
}
```

---

### Pattern: Search/Filter UI

```cpp
UCLASS()
class UKalkiCombatLogWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* SearchBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ResultsText;

    FString CurrentSearchTerm;

    UFUNCTION()
    void OnSearchTextChanged(const FText& Text);

    UFUNCTION()
    void OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    void ApplySearchFilter();
};

void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SearchBox)
    {
        SearchBox->OnTextChanged.AddDynamic(
            this,
            &UKalkiCombatLogWidget::OnSearchTextChanged
        );

        SearchBox->OnTextCommitted.AddDynamic(
            this,
            &UKalkiCombatLogWidget::OnSearchTextCommitted
        );
    }
}

void UKalkiCombatLogWidget::OnSearchTextChanged(const FText& Text)
{
    CurrentSearchTerm = Text.ToString();
    
    // Live search (can be expensive)
    // Consider debouncing for large logs
    ApplySearchFilter();
}

void UKalkiCombatLogWidget::OnSearchTextCommitted(
    const FText& Text,
    ETextCommit::Type CommitMethod
)
{
    if (CommitMethod == ETextCommit::OnEnter)
    {
        CurrentSearchTerm = Text.ToString();
        ApplySearchFilter();
    }
}

void UKalkiCombatLogWidget::ApplySearchFilter()
{
    if (!LogListView || !CombatLogViewModel)
    {
        return;
    }

    LogListView->ClearListItems();

    int32 MatchCount = 0;

    // Filter entries by search term
    for (const FKalkiCombatLogEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        FString Message = Entry.DisplayText.ToString();

        // Case-insensitive search
        if (CurrentSearchTerm.IsEmpty() || 
            Message.Contains(CurrentSearchTerm, ESearchCase::IgnoreCase))
        {
            UKalkiLogEntryData* Data = NewObject<UKalkiLogEntryData>(this);
            Data->Entry = Entry;
            LogListView->AddItem(Data);
            MatchCount++;
        }
    }

    // Update results text
    if (ResultsText)
    {
        if (CurrentSearchTerm.IsEmpty())
        {
            ResultsText->SetText(
                FText::Format(INVTEXT("{0} messages"), MatchCount)
            );
        }
        else
        {
            ResultsText->SetText(
                FText::Format(
                    INVTEXT("{0} matches for '{1}'"),
                    MatchCount,
                    FText::FromString(CurrentSearchTerm)
                )
            );
        }
    }
}
```

---

### Pattern: Export Log to Clipboard

```cpp
void UKalkiCombatLogWidget::OnExportButtonClicked()
{
    if (!CombatLogViewModel)
    {
        return;
    }

    // Build export string
    FString ExportText;

    for (const FKalkiCombatLogEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        FString TimeStr = Entry.Timestamp.ToString(TEXT("%H:%M:%S"));
        FString SeverityStr = UEnum::GetValueAsString(Entry.Severity);
        FString MessageStr = Entry.DisplayText.ToString();

        ExportText += FString::Printf(
            TEXT("[%s] [%s] %s\n"),
            *TimeStr,
            *SeverityStr,
            *MessageStr
        );
    }

    // Copy to clipboard
    FPlatformApplicationMisc::ClipboardCopy(*ExportText);

    // Show confirmation
    ShowNotification(TEXT("Log copied to clipboard!"));
}
```

---

## Performance Considerations

### Memory Usage

**Per Message:**

```
FKalkiCombatLogEntry: ~200 bytes
- FText DisplayText: ~100 bytes
- Other fields: ~100 bytes

100 messages: ~20 KB
500 messages: ~100 KB
1000 messages: ~200 KB
```

**Recommendations:**

- Keep `MaxDisplayEntries` between 100-500
- Clear messages when not needed
- Use file logging for long-term storage

---

### Update Frequency

**OnDataChanged Event:**

- Fired every time DisplayEntries changes
- Can fire frequently during active combat
- Widget should batch updates if needed

**Optimization:**

```cpp
// Debounce rapid updates
void UKalkiCombatLogWidget::OnLogDataChanged()
{
    // Clear pending timer
    GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);

    // Schedule update after brief delay
    GetWorld()->GetTimerManager().SetTimer(
        UpdateTimerHandle,
        this,
        &UKalkiCombatLogWidget::UpdateLogDisplay,
        0.1f,  // 100ms delay
        false
    );
}

void UKalkiCombatLogWidget::UpdateLogDisplay()
{
    // Do actual update
    RebuildListView();
}
```

---

### Filter Performance

**Filter Rebuilding:**

- Filters rebuild entire DisplayEntries array
- O(n) operation where n = captured messages
- Acceptable for < 1000 messages

**Large Log Optimization:**

```cpp
// For very large logs (1000+ messages)
// Consider incremental filtering

void UKalkiCombatLogViewModel::SetChannelFilter(EKalkiLogChannel Channel)
{
    CurrentChannelFilter = Channel;

    // Instead of rebuilding entire array,
    // mark entries as visible/hidden
    for (FKalkiCombatLogEntry& Entry : DisplayEntries)
    {
        Entry.bVisible = (Entry.Channel == Channel);
    }

    OnDataChanged.Broadcast();
}
```

---

## Common Mistakes

### Mistake 1: Forgetting to Initialize

```cpp
// ❌ BAD - ViewModel not initialized
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
    // Missing: CombatLogViewModel->Initialize();
    
    // ViewModel won't receive messages!
}

// ✅ GOOD - Properly initialized
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
    CombatLogViewModel->Initialize();  // Required!
}
```

---

### Mistake 2: Not Unbinding Events

```cpp
// ❌ BAD - Memory leak, event still bound
void UKalkiCombatLogWidget::NativeDestruct()
{
    if (CombatLogViewModel)
    {
        CombatLogViewModel->Shutdown();
    }
    // Missing: Unbind OnDataChanged!
    
    Super::NativeDestruct();
}

// ✅ GOOD - Properly cleaned up
void UKalkiCombatLogWidget::NativeDestruct()
{
    if (CombatLogViewModel)
    {
        CombatLogViewModel->OnDataChanged.RemoveDynamic(
            this,
            &UKalkiCombatLogWidget::OnLogDataChanged
        );
        
        CombatLogViewModel->Shutdown();
        CombatLogViewModel = nullptr;
    }
    
    Super::NativeDestruct();
}
```

---

### Mistake 3: Modifying DisplayEntries Directly

```cpp
// ❌ BAD - Modifying read-only array
void UKalkiCombatLogWidget::RemoveOldMessages()
{
    CombatLogViewModel->DisplayEntries.RemoveAt(0, 10);  // Don't do this!
}

// ✅ GOOD - Use provided functions
void UKalkiCombatLogWidget::RemoveOldMessages()
{
    CombatLogViewModel->ClearMessages();  // Or adjust MaxDisplayEntries
}
```

---

### Mistake 4: Not Handling Empty State

```cpp
// ❌ BAD - Assumes messages exist
void UKalkiCombatLogWidget::OnLogDataChanged()
{
    LogListView->ClearListItems();
    
    // Crashes if no messages!
    UKalkiLogEntryData* FirstEntry = 
        CombatLogViewModel->DisplayEntries[0];
}

// ✅ GOOD - Check for empty
void UKalkiCombatLogWidget::OnLogDataChanged()
{
    LogListView->ClearListItems();
    
    if (CombatLogViewModel->GetMessageCount() == 0)
    {
        ShowEmptyMessage();
        return;
    }
    
    // Process messages...
}
```

---

### Mistake 5: Inefficient List Rebuilding

```cpp
// ❌ BAD - Rebuilds entire list every message
void UKalkiCombatLogWidget::OnLogDataChanged()
{
    LogListView->ClearListItems();
    
    for (const FKalkiCombatLogEntry& Entry : CombatLogViewModel->DisplayEntries)
    {
        // Creates new widget every time!
        UKalkiLogEntryData* Data = NewObject<UKalkiLogEntryData>(this);
        Data->Entry = Entry;
        LogListView->AddItem(Data);
    }
}

// ✅ GOOD - Only add new items
void UKalkiCombatLogWidget::OnLogDataChanged()
{
    int32 CurrentCount = LogListView->GetNumItems();
    int32 NewCount = CombatLogViewModel->GetMessageCount();
    
    if (NewCount < CurrentCount)
    {
        // List was cleared - rebuild
        LogListView->ClearListItems();
        CurrentCount = 0;
    }
    
    // Only add new items
    for (int32 i = CurrentCount; i < NewCount; ++i)
    {
        const FKalkiCombatLogEntry& Entry = CombatLogViewModel->DisplayEntries[i];
        UKalkiLogEntryData* Data = NewObject<UKalkiLogEntryData>(this);
        Data->Entry = Entry;
        LogListView->AddItem(Data);
    }
}
```

---

## Troubleshooting

### Messages Not Appearing

**Problem:** ViewModel created but no messages show

**Solutions:**

1. Verify `Initialize()` called
2. Check if filters too restrictive
3. Verify logging system working (check Output Log)
4. Ensure OnDataChanged bound correctly
5. Check MaxDisplayEntries not 0

---

### Widget Not Updating

**Problem:** Messages added but widget doesn't refresh

**Solutions:**

1. Verify OnDataChanged bound with `AddDynamic`
2. Check if widget update function implemented
3. Verify ViewModel not null
4. Check if widget still alive (not destroyed)
5. Try calling UpdateLogDisplay() manually

---

### File Logging Not Working

**Problem:** EnableFileLogging(true) but no file created

**Solutions:**

1. Check log output for file creation errors
2. Verify write permissions to Saved folder
3. Call `StartNewSession()` after enabling
4. Check disk space available
5. Verify path: `{Project}/Saved/Logs/CombatLog/`

---

### Performance Issues

**Problem:** UI stutters when many messages

**Solutions:**

1. Reduce MaxDisplayEntries (100-200 recommended)
2. Implement update debouncing
3. Use virtualized list view
4. Disable file logging in performance mode
5. Clear old messages periodically

---

## Related Documents

### Architecture

- [[03_Systems/Combat_Log_System]] - System design
- [[03_Systems/MVVM_Pattern]] - MVVM architecture
- [[03_Systems/Logging_System]] - Logging foundation

### Related APIs

- [[05_API_Reference/KalkiLog_API]] - Logging API
- [[05_API_Reference/KalkiViewModelBase_API]] - ViewModel base
- [[05_API_Reference/KalkiViewModelWidget_API]] - Widget base

### Implementation

- [[04_Implementation/Common_Patterns]] - MVVM patterns
- [[04_Implementation/Class_Templates]] - ViewModel template

---

## Quick Reference

### Essential Functions

```cpp
// Create and initialize
UKalkiCombatLogViewModel* VM = NewObject<UKalkiCombatLogViewModel>();
VM->Initialize();

// Get messages
TArray<FKalkiCombatLogEntry>& Entries = VM->DisplayEntries;
int32 Count = VM->GetMessageCount();

// Filtering
VM->SetChannelFilter(EKalkiLogChannel::Combat);
VM->SetMinimumSeverity(EKalkiLogSeverity::Warning);
VM->ClearFilters();

// Management
VM->ClearMessages();

// Sessions
VM->StartNewSession();
VM->EndCurrentSession();

// File logging
VM->EnableFileLogging(true);
FString Path = VM->GetCurrentLogFilePath();

// Events
VM->OnDataChanged.AddDynamic(this, &UWidget::OnLogUpdated);

// Cleanup
VM->OnDataChanged.RemoveDynamic(this, &UWidget::OnLogUpdated);
VM->Shutdown();
```

---

## Document Info

**File Path:** `Kalki_GDD/05_API_Reference/KalkiCombatLogViewModel_API.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate  
**Note:** Complete API reference for KalkiCombatLogViewModel