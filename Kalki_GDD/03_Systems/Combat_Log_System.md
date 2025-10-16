---
tags:
  - kalki/system/combat-log
  - kalki/ui
  - kalki/mvvm
  - kalki/phase-2
status: complete
phase: 2
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Combat Log System

## Overview
The Combat Log System provides a user-facing log of combat events (damage, healing, abilities, etc.) with MVVM architecture, file logging, and session tracking. Unlike the [[03_Systems/Logging_System]] (developer debugging), this is a gameplay feature visible to players with rich interactive elements and tooltips.

---

## Purpose

### Problems Solved
1. **Combat Feedback** - Players need to see what happened in combat
2. **History Tracking** - Review past combat events
3. **Interactive Information** - Hover over elements for detailed tooltips
4. **Debugging Gameplay** - Players can understand why things happened
5. **Session Logging** - Save combat events to file for analysis

### Design Goals
- ✅ **MVVM Architecture** - Clean separation of data and display
- ✅ **Real-time Updates** - Instant feedback on combat events
- ✅ **Interactive Elements** - Hover tooltips for characters, abilities, dice rolls
- ✅ **Auto-scroll** - Automatically scroll to latest message
- ✅ **Message Styling** - Different colors/formatting per severity
- ✅ **File Logging** - Write to file for post-session analysis
- ✅ **Channel Filtering** - Filter by system (Combat, Grid, etc.)

---

## Architecture

### MVVM Pattern
```
┌─────────────────────────────────────────┐
│  Game Events (Damage, Abilities, etc.)  │
└──────────────┬──────────────────────────┘
               │
               ↓
┌─────────────────────────────────────────┐
│  UKalkiCombatLogViewModel               │
│  - Converts FKalkiLogMessage            │
│  - Creates FKalkiLogDisplayEntry        │
│  - Handles formatting & colors          │
│  - Writes to file                       │
└──────────────┬──────────────────────────┘
               │ (ViewModel binding)
               ↓
┌─────────────────────────────────────────┐
│  UKalkiCombatLogWidget                  │
│  - Displays entries                     │
│  - Scroll view                          │
│  - Interactive element handling         │
│  - Tooltip display                      │
└─────────────────────────────────────────┘
```

**Why MVVM:**
- Separates data (ViewModel) from display (Widget)
- ViewModel can be unit tested
- Multiple widgets can share same ViewModel
- Easy to add new interactive element types
- File logging independent of UI

**See [[03_Systems/MVVM_Pattern]] for detailed architecture.**

---

## Key Data Types

### FKalkiLogDisplayEntry

**File:** `Public/UI/CombatLog/KalkiCombatLogTypes.h`  
**Type:** Struct (USTRUCT)  
**Purpose:** UI-friendly display entry for a single log message
```cpp
USTRUCT(BlueprintType)
struct KALKI_API FKalkiLogDisplayEntry
{
    GENERATED_BODY()

    // Display text (formatted for UI)
    UPROPERTY(BlueprintReadOnly)
    FText DisplayText;

    // Color for this message (based on severity)
    UPROPERTY(BlueprintReadOnly)
    FLinearColor Color = FLinearColor::White;

    // Severity (for icons, styling)
    UPROPERTY(BlueprintReadOnly)
    EKalkiLogSeverity Severity = EKalkiLogSeverity::Log;

    // Channel (for filtering)
    UPROPERTY(BlueprintReadOnly)
    EKalkiLogChannel Channel = EKalkiLogChannel::System;

    // Interactive elements (for tooltips/clicks)
    UPROPERTY(BlueprintReadOnly)
    TArray<FKalkiLogInteractiveElement> InteractiveElements;

    // Original message (for full detail)
    UPROPERTY(BlueprintReadOnly)
    FKalkiLogMessage OriginalMessage;
};
```

**Key Properties:**
- **DisplayText** - What the user sees (formatted FText)
- **Color** - Message color based on severity/channel
- **Severity** - Log/Warning/Error (from KalkiLog system)
- **Channel** - Combat/Grid/UI/etc. (for filtering)
- **InteractiveElements** - Rich tooltip data (future use)
- **OriginalMessage** - Raw log data for reference

---

### FKalkiLogInteractiveElement

**Type:** Struct (USTRUCT)  
**Purpose:** Interactive element within a log message (hover tooltips, clicks)
```cpp
USTRUCT(BlueprintType)
struct KALKI_API FKalkiLogInteractiveElement
{
    GENERATED_BODY()

    // Type of this element
    UPROPERTY(BlueprintReadOnly)
    EKalkiLogElementType ElementType = EKalkiLogElementType::Character;

    // Text range in the display string
    UPROPERTY(BlueprintReadOnly)
    int32 StartIndex = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 EndIndex = 0;

    // Display text for this element
    UPROPERTY(BlueprintReadOnly)
    FString DisplayText;

    // Tooltip data (key-value pairs)
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> TooltipData;

    // Reference to source object
    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<UObject> SourceObject;
};
```

**Usage:**
- **Character** - Hover to see HP, stats
- **Damage** - Hover to see damage breakdown
- **Ability** - Hover to see ability details
- **DiceRoll** - Hover to see roll breakdown (d20+5 = 18)
- **Position** - Hover to highlight grid tile
- **Item** - Hover to see item properties
- **StatusEffect** - Hover to see effect description

**Example:**
```
"Wizard cast Fireball at (5, 7) dealing 24 damage to Goblin"
   ^          ^            ^              ^            ^
   Character  Ability      Position       Damage       Character
   (tooltip)  (tooltip)    (highlight)    (breakdown)  (tooltip)
```

---

### EKalkiLogElementType

**Type:** Enum  
**Purpose:** Categorize interactive elements
```cpp
UENUM(BlueprintType)
enum class EKalkiLogElementType : uint8
{
    Character       UMETA(DisplayName = "Character"),
    Damage          UMETA(DisplayName = "Damage"),
    Ability         UMETA(DisplayName = "Ability"),
    Item            UMETA(DisplayName = "Item"),
    Position        UMETA(DisplayName = "Position"),
    DiceRoll        UMETA(DisplayName = "Dice Roll"),
    StatusEffect    UMETA(DisplayName = "Status Effect")
};
```

**Future Use:**
- Different tooltip styles per type
- Click actions per type (e.g., click position to focus camera)
- Icons per type
- Color coding

---

## Key Classes

### UKalkiCombatLogViewModel

**Type:** ViewModel (UKalkiViewModelBase)  
**Path:** `Source/Kalki/Public/UI/Combat/KalkiCombatLogViewModel.h`

**Purpose:**
- Convert FKalkiLogMessage to FKalkiLogDisplayEntry
- Store display entries
- Apply color coding based on severity/channel
- Write to file
- Notify widget of changes

**Key Properties:**
```cpp
// Display entries (what widget shows)
UPROPERTY(BlueprintReadOnly)
TArray<FKalkiLogDisplayEntry> DisplayEntries;

// File logging
FString CurrentSessionFile;
bool bFileLoggingEnabled;

// Filtering (future)
EKalkiLogChannel ActiveFilter;
bool bFilterEnabled;
```

**Key Functions:**
```cpp
// Add message from logging system
void AddLogMessage(const FKalkiLogMessage& LogMessage);

// Convert to display entry
FKalkiLogDisplayEntry CreateDisplayEntry(const FKalkiLogMessage& LogMessage);

// File management
void StartNewSession();
void WriteToFile(const FKalkiLogDisplayEntry& Entry);
void ClearLog();

// Filtering (future)
void SetChannelFilter(EKalkiLogChannel Channel);
void ClearFilter();
```

---

### UKalkiCombatLogWidget

**Type:** ViewModelWidget (UKalkiViewModelWidget)  
**Path:** `Source/Kalki/Public/UI/Combat/KalkiCombatLogWidget.h`

**Purpose:**
- Display combat log entries
- Handle scrolling
- Apply visual styling per severity
- Handle interactive elements (future)
- Bind to ViewModel

**Key Properties:**
```cpp
// UI Elements (bound in UMG)
UPROPERTY(meta = (BindWidget))
TObjectPtr<UScrollBox> MessageScrollBox;

// ViewModel reference
UPROPERTY()
TObjectPtr<UKalkiCombatLogViewModel> CombatLogViewModel;
```

**Key Functions:**
```cpp
// Override from base class
virtual void SetViewModel(UKalkiViewModelBase* InViewModel) override;

// Update display
void UpdateMessageDisplay();
void ScrollToBottom();

// Color coding
FLinearColor GetColorForSeverity(EKalkiLogSeverity Severity) const;
FLinearColor GetColorForChannel(EKalkiLogChannel Channel) const;
```

---

## Color Coding

### By Severity
```cpp
FLinearColor GetColorForSeverity(EKalkiLogSeverity Severity) const
{
    switch (Severity)
    {
        case EKalkiLogSeverity::Log:
            return FLinearColor::White;
        case EKalkiLogSeverity::Warning:
            return FLinearColor(1.0f, 1.0f, 0.3f); // Yellow
        case EKalkiLogSeverity::Error:
            return FLinearColor(1.0f, 0.3f, 0.3f); // Red
        default:
            return FLinearColor::White;
    }
}
```

**Visual Result:**
- **Log** - White (normal information)
- **Warning** - Yellow (caution, unexpected but not critical)
- **Error** - Red (critical problem)

---

### By Channel (Optional)
```cpp
FLinearColor GetColorForChannel(EKalkiLogChannel Channel) const
{
    switch (Channel)
    {
        case EKalkiLogChannel::Combat:
            return FLinearColor(1.0f, 0.3f, 0.3f); // Red
        case EKalkiLogChannel::Grid:
            return FLinearColor(0.3f, 1.0f, 0.3f); // Green
        case EKalkiLogChannel::UI:
            return FLinearColor(0.3f, 0.6f, 1.0f); // Blue
        case EKalkiLogChannel::System:
            return FLinearColor::White;
        default:
            return FLinearColor::White;
    }
}
```

**Note:** Can mix severity + channel for nuanced coloring

---

## Usage Examples

### Basic Usage - Adding Messages

**From Combat System:**
```cpp
// Combat events automatically flow through KalkiLog
KalkiLog::Combat(
    FString::Printf(TEXT("%s dealt %d damage to %s"), 
        *Attacker->GetName(), DamageAmount, *Target->GetName()),
    EKalkiLogSeverity::Log,
    this
);

// This is captured by CombatLogViewModel and displayed
```

**From Grid System:**
```cpp
// Grid events
KalkiLog::Grid(
    FString::Printf(TEXT("Character moved to (%d, %d)"), X, Y),
    EKalkiLogSeverity::Log,
    Character
);
```

**From Ability System (future):**
```cpp
// Ability cast
KalkiLog::Ability(
    FString::Printf(TEXT("%s cast %s"), *Caster->GetName(), *AbilityName),
    EKalkiLogSeverity::Log,
    Caster
);
```

---

### Advanced Usage - Interactive Elements (Future)

**Creating rich messages with tooltips:**
```cpp
FKalkiLogDisplayEntry Entry;
Entry.DisplayText = FText::FromString(TEXT("Wizard cast Fireball dealing 24 damage to Goblin"));
Entry.Channel = EKalkiLogChannel::Combat;
Entry.Severity = EKalkiLogSeverity::Log;

// Add character element (Wizard)
FKalkiLogInteractiveElement WizardElement;
WizardElement.ElementType = EKalkiLogElementType::Character;
WizardElement.StartIndex = 0;
WizardElement.EndIndex = 6; // "Wizard"
WizardElement.DisplayText = TEXT("Wizard");
WizardElement.TooltipData.Add(TEXT("HP"), TEXT("45/45"));
WizardElement.TooltipData.Add(TEXT("Class"), TEXT("Wizard"));
WizardElement.SourceObject = WizardCharacter;
Entry.InteractiveElements.Add(WizardElement);

// Add ability element (Fireball)
FKalkiLogInteractiveElement AbilityElement;
AbilityElement.ElementType = EKalkiLogElementType::Ability;
AbilityElement.StartIndex = 12;
AbilityElement.EndIndex = 20; // "Fireball"
AbilityElement.DisplayText = TEXT("Fireball");
AbilityElement.TooltipData.Add(TEXT("Damage"), TEXT("8d6 fire"));
AbilityElement.TooltipData.Add(TEXT("Range"), TEXT("150 ft"));
AbilityElement.TooltipData.Add(TEXT("Save"), TEXT("DEX DC 15"));
Entry.InteractiveElements.Add(AbilityElement);

// Add damage element
FKalkiLogInteractiveElement DamageElement;
DamageElement.ElementType = EKalkiLogElementType::Damage;
DamageElement.StartIndex = 29;
DamageElement.EndIndex = 31; // "24"
DamageElement.DisplayText = TEXT("24");
DamageElement.TooltipData.Add(TEXT("Roll"), TEXT("8d6 = [3,5,4,2,1,6,3,4]"));
DamageElement.TooltipData.Add(TEXT("Type"), TEXT("Fire"));
DamageElement.TooltipData.Add(TEXT("Resist"), TEXT("None"));
Entry.InteractiveElements.Add(DamageElement);

// Add target character element (Goblin)
FKalkiLogInteractiveElement GoblinElement;
GoblinElement.ElementType = EKalkiLogElementType::Character;
GoblinElement.StartIndex = 42;
GoblinElement.EndIndex = 48; // "Goblin"
GoblinElement.DisplayText = TEXT("Goblin");
GoblinElement.TooltipData.Add(TEXT("HP"), TEXT("7/31 - Bloodied"));
GoblinElement.TooltipData.Add(TEXT("AC"), TEXT("15"));
GoblinElement.SourceObject = GoblinCharacter;
Entry.InteractiveElements.Add(GoblinElement);

CombatLogViewModel->AddDisplayEntry(Entry);
```

---

## File Logging

### Session Files

**Location:** `Saved/Logs/CombatLog/`

**Filename Format:** `CombatLog_YYYY-MM-DD_HH-MM-SS.txt`

**Example:** `CombatLog_2025-01-09_14-23-45.txt`

---

### File Content Format
```
=== Combat Log Session ===
Started: 2025-01-09 14:23:45

[14:23:47] [COMBAT] [LOG] Combat started
[14:23:50] [ABILITY] [LOG] Wizard cast Fireball
[14:23:51] [COMBAT] [LOG] Wizard dealt 24 damage to Goblin
[14:23:51] [COMBAT] [WARNING] Goblin is bloodied (7/31 HP)
[14:23:55] [ABILITY] [LOG] Fighter used Second Wind
[14:23:55] [COMBAT] [LOG] Fighter healed 10 HP (45/45)
[14:24:02] [COMBAT] [LOG] Critical hit! Fighter dealt 32 damage
[14:24:10] [SYSTEM] [LOG] Combat ended

=== Session End ===
Duration: 23 seconds
Total Messages: 8
```

---

### Implementation
```cpp
void UKalkiCombatLogViewModel::StartNewSession()
{
    // Create log directory if needed
    FString LogDir = FPaths::ProjectSavedDir() / TEXT("Logs/CombatLog/");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*LogDir))
    {
        PlatformFile.CreateDirectoryTree(*LogDir);
    }

    // Generate filename with timestamp
    FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    CurrentSessionFile = LogDir / FString::Printf(TEXT("CombatLog_%s.txt"), *Timestamp);

    // Write session header
    FString Header = FString::Printf(
        TEXT("=== Combat Log Session ===\nStarted: %s\n\n"),
        *FDateTime::Now().ToString()
    );
    FFileHelper::SaveStringToFile(Header, *CurrentSessionFile);

    bFileLoggingEnabled = true;

    KalkiLog::UI(
        FString::Printf(TEXT("Combat log session started: %s"), *CurrentSessionFile)
    );
}

void UKalkiCombatLogViewModel::WriteToFile(const FKalkiLogDisplayEntry& Entry)
{
    if (!bFileLoggingEnabled || CurrentSessionFile.IsEmpty())
    {
        return;
    }

    // Format: [HH:MM:SS] [CHANNEL] [SEVERITY] Message
    FString FormattedMessage = FString::Printf(
        TEXT("[%s] [%s] [%s] %s\n"),
        *FDateTime::Now().ToString(TEXT("%H:%M:%S")),
        *GetChannelName(Entry.Channel),
        *GetSeverityName(Entry.Severity),
        *Entry.DisplayText.ToString()
    );

    // Append to file
    FFileHelper::SaveStringToFile(
        FormattedMessage,
        *CurrentSessionFile,
        FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(),
        FILEWRITE_Append
    );
}

void UKalkiCombatLogViewModel::AddLogMessage(const FKalkiLogMessage& LogMessage)
{
    // Convert to display entry
    FKalkiLogDisplayEntry DisplayEntry = CreateDisplayEntry(LogMessage);
    
    // Add to display list
    DisplayEntries.Add(DisplayEntry);
    
    // Limit to last 100 messages
    const int32 MaxMessages = 100;
    if (DisplayEntries.Num() > MaxMessages)
    {
        DisplayEntries.RemoveAt(0, DisplayEntries.Num() - MaxMessages);
    }
    
    // Write to file
    WriteToFile(DisplayEntry);
    
    // Notify widget
    OnEntriesChanged.Broadcast();
}

FKalkiLogDisplayEntry UKalkiCombatLogViewModel::CreateDisplayEntry(const FKalkiLogMessage& LogMessage)
{
    FKalkiLogDisplayEntry Entry;
    
    // Set display text
    Entry.DisplayText = FText::FromString(LogMessage.Message);
    
    // Set color based on severity
    Entry.Color = GetColorForSeverity(LogMessage.Severity);
    
    // Copy severity and channel
    Entry.Severity = LogMessage.Severity;
    Entry.Channel = LogMessage.Channel;
    
    // Store original message
    Entry.OriginalMessage = LogMessage;
    
    // Note: Interactive elements would be parsed here in future
    // For now, they're empty
    
    return Entry;
}
```

---

## Widget Implementation

### UMG Widget Structure

**WBP_CombatLog hierarchy:**
```
Canvas Panel (Root)
└─ Border (Background)
   └─ Vertical Box
      ├─ Text Block (Title: "Combat Log")
      ├─ Horizontal Box (Filter buttons - future)
      └─ Scroll Box (MessageScrollBox)
         └─ [Text Blocks created dynamically]
```

---

### C++ Widget Code
```cpp
void UKalkiCombatLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create ViewModel if not set
    if (!CombatLogViewModel)
    {
        CombatLogViewModel = NewObject<UKalkiCombatLogViewModel>(this);
        KalkiLog::UI(TEXT("CombatLogWidget - ViewModel created"));
    }

    // Initial display update
    UpdateMessageDisplay();
}

void UKalkiCombatLogWidget::SetViewModel(UKalkiViewModelBase* InViewModel)
{
    Super::SetViewModel(InViewModel);

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
        // Create text block for message
        UTextBlock* MessageText = NewObject<UTextBlock>(this);
        MessageText->SetText(Entry.DisplayText);
        
        // Apply color from entry
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

FLinearColor UKalkiCombatLogWidget::GetColorForSeverity(EKalkiLogSeverity Severity) const
{
    switch (Severity)
    {
        case EKalkiLogSeverity::Log:
            return FLinearColor::White;
        case EKalkiLogSeverity::Warning:
            return FLinearColor(1.0f, 1.0f, 0.3f); // Yellow
        case EKalkiLogSeverity::Error:
            return FLinearColor(1.0f, 0.3f, 0.3f); // Red
        default:
            return FLinearColor::White;
    }
}
```

---

## Integration with KalkiLog

### How Messages Flow
```
1. Game code calls KalkiLog
   KalkiLog::Combat(TEXT("Damage dealt"));
        ↓
2. KalkiLog creates FKalkiLogMessage
   (with channel, severity, context)
        ↓
3. CombatLogViewModel listens to log events
   (or receives messages explicitly)
        ↓
4. ViewModel converts to FKalkiLogDisplayEntry
   (adds color, formatting)
        ↓
5. Widget displays entry
   (with proper styling)
```

---

## Performance Considerations

### Message Limit
```cpp
// Prevent unbounded growth
const int32 MaxMessages = 100;
if (DisplayEntries.Num() > MaxMessages)
{
    DisplayEntries.RemoveAt(0, DisplayEntries.Num() - MaxMessages);
}
```

---

### Widget Pooling (Future Optimization)

**For many messages:**
```cpp
// Reuse text blocks instead of creating new ones
TArray<UTextBlock*> TextBlockPool;

void UpdateMessageAtIndex(int32 Index, const FKalkiLogDisplayEntry& Entry)
{
    if (TextBlockPool.IsValidIndex(Index))
    {
        // Reuse existing
        TextBlockPool[Index]->SetText(Entry.DisplayText);
        TextBlockPool[Index]->SetColorAndOpacity(Entry.Color);
    }
    else
    {
        // Create new and add to pool
        UTextBlock* NewText = NewObject<UTextBlock>(this);
        TextBlockPool.Add(NewText);
        MessageScrollBox->AddChild(NewText);
    }
}
```

---

## Future Enhancements

### Planned Features

**1. Interactive Tooltips**
```cpp
// Hover over interactive elements
void OnHoverElement(const FKalkiLogInteractiveElement& Element)
{
    // Show tooltip with TooltipData
    ShowTooltip(Element.TooltipData);
}
```

**2. Click Actions**
```cpp
// Click on character → focus camera
// Click on position → highlight grid tile
// Click on dice roll → show breakdown
void OnClickElement(const FKalkiLogInteractiveElement& Element)
{
    switch (Element.ElementType)
    {
        case EKalkiLogElementType::Character:
            FocusCharacter(Element.SourceObject);
            break;
        case EKalkiLogElementType::Position:
            HighlightGridTile(Element.DisplayText);
            break;
        // ... etc
    }
}
```

**3. Channel Filtering**
```cpp
// Show only Combat messages
CombatLogViewModel->SetChannelFilter(EKalkiLogChannel::Combat);

// Show all
CombatLogViewModel->ClearFilter();
```

**4. Rich Text Formatting**
```cpp
// Support rich text markup
<b>Fighter</b> dealt <color=#ff0000>32</color> damage
```

**5. Message Search**
```cpp
// Find messages containing text
TArray<FKalkiLogDisplayEntry> SearchEntries(const FString& Query);
```

---

## Related Documents

### Core Systems
- [[03_Systems/MVVM_Pattern]] - Architecture pattern used
- [[03_Systems/Logging_System]] - Developer logging (KalkiLog)

### Types
- `KalkiCombatLogTypes.h` - FKalkiLogDisplayEntry, FKalkiLogInteractiveElement
- `KalkiLogTypes.h` - FKalkiLogMessage, EKalkiLogChannel, EKalkiLogSeverity

### Architecture
- [[01_Architecture/Module_Overview]] - UI subsystem overview

### Implementation
- [[04_Implementation/Common_Patterns]] - MVVM implementation patterns
- [[04_Implementation/Testing_Procedures]] - Testing UI systems

---

## Quick Reference

### Adding Combat Log Messages

**Method 1: Via KalkiLog (recommended)**
```cpp
KalkiLog::Combat(TEXT("Fighter dealt 10 damage"), EKalkiLogSeverity::Log, Fighter);
// Automatically appears in combat log
```

**Method 2: Direct to ViewModel**
```cpp
CombatLogViewModel->AddLogMessage(LogMessage);
```

---

## Document Info
**File Path:** `Kalki_GDD/03_Systems/Combat_Log_System.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** 2  
**Complexity:** Intermediate  
**Note:** Uses KalkiCombatLogTypes.h for data structures