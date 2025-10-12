// Copyright of V.S. Puranam and no one else


// Private/UI/CombatLog/KalkiCombatLogViewModel.cpp

#include "UI/CombatLog/KalkiCombatLogViewModel.h"
#include "Logging/KalkiLogSubsystem.h"
#include "Logging/KalkiLog.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"

void UKalkiCombatLogViewModel::OnInitialize()
{
    Super::OnInitialize();

    // Get log subsystem
    LogSubsystem = GetWorld()->GetSubsystem<UKalkiLogSubsystem>();
    
    if (!LogSubsystem)
    {
        KalkiLog::System(TEXT("CombatLogViewModel - Failed to get LogSubsystem"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Bind to log events
    LogSubsystem->OnLogMessageAdded.AddDynamic(this, &UKalkiCombatLogViewModel::OnLogMessageAdded);

    // Initialize filters (show all channels by default)
    ClearAllFilters();

    // Get current combat session
    if (LogSubsystem->IsInCombat())
    {
        TrackedCombatSessionID = LogSubsystem->GetCurrentCombatSessionID();
        RefreshFromSubsystem();
    }

    KalkiLog::System(TEXT("CombatLogViewModel initialized"), EKalkiLogSeverity::Log, this);
}

void UKalkiCombatLogViewModel::OnShutdown()
{
    // Unbind from log events
    if (LogSubsystem)
    {
        LogSubsystem->OnLogMessageAdded.RemoveDynamic(this, &UKalkiCombatLogViewModel::OnLogMessageAdded);
    }

    CombatMessages.Empty();
    LogSubsystem = nullptr;

    Super::OnShutdown();
}

void UKalkiCombatLogViewModel::OnLogMessageAdded(const FKalkiLogMessage& Message)
{
    // Check if this is a combat session start
    if (Message.Message.Contains(TEXT("Combat session started")))
    {
        // New combat started - clear old messages and track new session
        if (LogSubsystem)
        {
            TrackedCombatSessionID = LogSubsystem->GetCurrentCombatSessionID();
        }
        CombatMessages.Empty();
    }

    // Check if this is a combat session end
    if (Message.Message.Contains(TEXT("Combat session ended")))
    {
        // Combat ended - stop tracking
        TrackedCombatSessionID = -1;
        // Keep messages displayed until explicitly cleared
    }

    // Only add messages from current combat session (or system messages)
    if (Message.CombatSessionID == TrackedCombatSessionID || 
        Message.Channel == EKalkiLogChannel::System)
    {
        // Check filters
        if (PassesFilters(Message))
        {
            // Transform and add
            FKalkiLogDisplayEntry Entry = TransformMessage(Message);
            CombatMessages.Add(Entry);

            // Notify UI
            OnCombatLogUpdated.Broadcast();
        }
    }
}

FKalkiLogDisplayEntry UKalkiCombatLogViewModel::TransformMessage(const FKalkiLogMessage& Message)
{
    FKalkiLogDisplayEntry Entry;

    // Format display text
    // Format: [Channel] Message
    FString ChannelStr = UEnum::GetDisplayValueAsText(Message.Channel).ToString();
    Entry.DisplayText = FText::FromString(FString::Printf(TEXT("[%s] %s"), *ChannelStr, *Message.Message));

    // Set color based on severity
    Entry.Color = Message.GetColor();

    // Copy severity and channel
    Entry.Severity = Message.Severity;
    Entry.Channel = Message.Channel;

    // Store original message
    Entry.OriginalMessage = Message;

    // Extract interactive elements (placeholder for future)
    Entry.InteractiveElements = ExtractInteractiveElements(Message);

    return Entry;
}

TArray<FKalkiLogInteractiveElement> UKalkiCombatLogViewModel::ExtractInteractiveElements(const FKalkiLogMessage& Message)
{
    TArray<FKalkiLogInteractiveElement> Elements;

    // TODO: Future implementation
    // Parse message text and metadata to find:
    // - Character names (from metadata or text parsing)
    // - Damage numbers (from metadata)
    // - Positions (from metadata: FromTile, ToTile)
    // - Dice rolls (from metadata)
    
    // For now, return empty array
    return Elements;
}

bool UKalkiCombatLogViewModel::PassesFilters(const FKalkiLogMessage& Message) const
{
    // Check channel filter
    const bool* FilterValue = ChannelFilters.Find(Message.Channel);
    if (FilterValue && !(*FilterValue))
    {
        return false; // Channel is filtered out
    }

    return true;
}

void UKalkiCombatLogViewModel::SetChannelFilter(EKalkiLogChannel Channel, bool bShow)
{
    ChannelFilters.Add(Channel, bShow);

    // Refresh display with new filters
    RefreshFromSubsystem();

    KalkiLog::System(
        FString::Printf(TEXT("CombatLogViewModel - Channel filter %s: %s"), 
            *UEnum::GetValueAsString(Channel),
            bShow ? TEXT("Show") : TEXT("Hide")),
        EKalkiLogSeverity::Log,
        this
    );
}

bool UKalkiCombatLogViewModel::IsChannelFiltered(EKalkiLogChannel Channel) const
{
    const bool* FilterValue = ChannelFilters.Find(Channel);
    return FilterValue ? *FilterValue : true; // Default to showing
}

void UKalkiCombatLogViewModel::ClearAllFilters()
{
    ChannelFilters.Empty();

    // Set all channels to show by default
    ChannelFilters.Add(EKalkiLogChannel::System, true);
    ChannelFilters.Add(EKalkiLogChannel::Combat, true);
    ChannelFilters.Add(EKalkiLogChannel::Movement, true);
    ChannelFilters.Add(EKalkiLogChannel::Abilities, true);
    ChannelFilters.Add(EKalkiLogChannel::TurnSystem, true);
    ChannelFilters.Add(EKalkiLogChannel::Grid, true);
    ChannelFilters.Add(EKalkiLogChannel::Dialogue, false); // Hide dialogue by default
    ChannelFilters.Add(EKalkiLogChannel::Inventory, false); // Hide inventory by default
    ChannelFilters.Add(EKalkiLogChannel::AI, true);
    ChannelFilters.Add(EKalkiLogChannel::Network, true);
    ChannelFilters.Add(EKalkiLogChannel::Quest, false); // Hide quest by default
    ChannelFilters.Add(EKalkiLogChannel::Ruleset, true);
    ChannelFilters.Add(EKalkiLogChannel::Debug, false); // Hide debug by default

    RefreshFromSubsystem();
}

void UKalkiCombatLogViewModel::ClearCombatLog()
{
    CombatMessages.Empty();
    TrackedCombatSessionID = -1;
    OnCombatLogUpdated.Broadcast();

    KalkiLog::System(TEXT("CombatLogViewModel - Combat log cleared"), EKalkiLogSeverity::Log, this);
}

void UKalkiCombatLogViewModel::RefreshFromSubsystem()
{
    if (!LogSubsystem)
    {
        return;
    }

    CombatMessages.Empty();

    // Get all messages from subsystem
    const TArray<FKalkiLogMessage>& AllMessages = LogSubsystem->GetLogHistory();

    // Filter and transform
    for (const FKalkiLogMessage& Message : AllMessages)
    {
        // Only show messages from current combat session
        if (Message.CombatSessionID == TrackedCombatSessionID || 
            Message.Channel == EKalkiLogChannel::System)
        {
            if (PassesFilters(Message))
            {
                CombatMessages.Add(TransformMessage(Message));
            }
        }
    }

    OnCombatLogUpdated.Broadcast();
}

void UKalkiCombatLogViewModel::OpenCurrentLogFile()
{
    if (!LogSubsystem)
    {
        KalkiLog::System(TEXT("CombatLogViewModel - Cannot open log file: No LogSubsystem"), EKalkiLogSeverity::Warning, this);
        return;
    }

    FString LogFilePath = LogSubsystem->GetCurrentLogFilePath();
    
    if (LogFilePath.IsEmpty())
    {
        KalkiLog::System(TEXT("CombatLogViewModel - Cannot open log file: No active log file"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Flush log to ensure everything is written
    LogSubsystem->FlushLogFile();

    // Copy log file to temp location (to avoid locking issues)
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    FString TempPath = FPaths::ProjectSavedDir() / TEXT("Logs") / TEXT("Temp_CurrentLog.log");
    
    if (PlatformFile.CopyFile(*TempPath, *LogFilePath))
    {
        // Open the temp copy
        FPlatformProcess::LaunchFileInDefaultExternalApplication(*TempPath, nullptr, ELaunchVerb::Open);
        
        KalkiLog::System(TEXT("CombatLogViewModel - Opened current log file"), EKalkiLogSeverity::Log, this);
    }
    else
    {
        KalkiLog::System(TEXT("CombatLogViewModel - Failed to copy log file"), EKalkiLogSeverity::Error, this);
    }
}

void UKalkiCombatLogViewModel::OpenLogFolder()
{
    FString LogDirectory = FPaths::ProjectSavedDir() / TEXT("Logs");
    FPlatformProcess::ExploreFolder(*LogDirectory);

    KalkiLog::System(TEXT("CombatLogViewModel - Opened log folder"), EKalkiLogSeverity::Log, this);
}