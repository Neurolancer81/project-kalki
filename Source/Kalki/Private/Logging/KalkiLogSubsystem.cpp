// Copyright of V.S. Puranam and no one else
// Private/Logging/KalkiLogSubsystem.cpp

#include "Logging/KalkiLogSubsystem.h"
#include "Logging/KalkiLogFileWriter.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Logging/KalkiLog.h"
#include "Misc/DateTime.h"

void UKalkiLogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    InitializeChannels();
    
    // Initialize file logging
    if (bFileLoggingEnabled)
    {
        InitializeFileLogging();
    }

    UE_LOG(LogTemp, Log, TEXT("KalkiLogSubsystem initialized"));

    // Register crash handler
    FCoreDelegates::OnHandleSystemError.AddUObject(this, &UKalkiLogSubsystem::OnHandleSystemError);

    // Log system startup
    Log(EKalkiLogChannel::System, EKalkiLogSeverity::Log, TEXT("Kalki logging system started"));
}

void UKalkiLogSubsystem::Deinitialize()
{
    // Log system shutdown
    Log(EKalkiLogChannel::System, EKalkiLogSeverity::Log, TEXT("Kalki logging system shutting down"));

    // Shutdown file logging
    if (bFileLoggingEnabled)
    {
        ShutdownFileLogging();
    }

    LogHistory.Empty();
    ChannelEnabled.Empty();

    Super::Deinitialize();
}

void UKalkiLogSubsystem::InitializeChannels()
{
    // Enable all channels by default
    ChannelEnabled.Add(EKalkiLogChannel::System, true);
    ChannelEnabled.Add(EKalkiLogChannel::Combat, true);
    ChannelEnabled.Add(EKalkiLogChannel::Movement, true);
    ChannelEnabled.Add(EKalkiLogChannel::Abilities, true);
    ChannelEnabled.Add(EKalkiLogChannel::TurnSystem, true);
    ChannelEnabled.Add(EKalkiLogChannel::Grid, true);
    ChannelEnabled.Add(EKalkiLogChannel::Dialogue, true);
    ChannelEnabled.Add(EKalkiLogChannel::Inventory, true);
    ChannelEnabled.Add(EKalkiLogChannel::AI, true);
    ChannelEnabled.Add(EKalkiLogChannel::Network, true);
    ChannelEnabled.Add(EKalkiLogChannel::Quest, true);
    ChannelEnabled.Add(EKalkiLogChannel::Ruleset, true);
    ChannelEnabled.Add(EKalkiLogChannel::Debug, true);
}

void UKalkiLogSubsystem::LogMessage(const FKalkiLogMessage& Message)
{
    // Check if this channel is enabled
    if (!IsChannelEnabled(Message.Channel))
    {
        return;
    }

    // Strip metadata if needed based on level
    FKalkiLogMessage ProcessedMessage = Message;
    if (MetadataLevel == EKalkiLogMetadataLevel::None)
    {
        ProcessedMessage.Metadata.Empty();
    }
    else if (MetadataLevel == EKalkiLogMetadataLevel::Minimal)
    {
        // Keep only essential metadata (you can define what "essential" means)
        // For now, keep all - you can refine this later
    }

    // NEW: Tag with current combat session
    ProcessedMessage.CombatSessionID = CurrentCombatSessionID;
    ProcessedMessage.CombatSessionName = CurrentCombatName;

    // Add to history
    LogHistory.Add(ProcessedMessage);

    // Maintain circular buffer (remove old messages if over limit)
    if (LogHistory.Num() > MaxLogHistory)
    {
        LogHistory.RemoveAt(0, LogHistory.Num() - MaxLogHistory);
    }

    // Route to various outputs
    RouteMessage(ProcessedMessage);

    // Broadcast event for UI
    OnLogMessageAdded.Broadcast(ProcessedMessage);
}

void UKalkiLogSubsystem::Log(EKalkiLogChannel Channel, EKalkiLogSeverity Severity, const FString& Message)
{
    FKalkiLogMessage LogMsg(Channel, Severity, Message);
    LogMessage(LogMsg);
}

TArray<FKalkiLogMessage> UKalkiLogSubsystem::GetLogHistoryFiltered(EKalkiLogChannel Channel) const
{
    TArray<FKalkiLogMessage> Filtered;

    for (const FKalkiLogMessage& Message : LogHistory)
    {
        if (Message.Channel == Channel)
        {
            Filtered.Add(Message);
        }
    }

    return Filtered;
}

void UKalkiLogSubsystem::ClearLogHistory()
{
    LogHistory.Empty();
    UE_LOG(LogTemp, Log, TEXT("KalkiLogSubsystem: Log history cleared"));
}

void UKalkiLogSubsystem::SetChannelEnabled(EKalkiLogChannel Channel, bool bEnabled)
{
    ChannelEnabled.Add(Channel, bEnabled);

    FString StatusText = bEnabled ? TEXT("enabled") : TEXT("disabled");
    UE_LOG(LogTemp, Log, TEXT("KalkiLogSubsystem: Channel %s %s"), 
        *UEnum::GetValueAsString(Channel), *StatusText);
}

bool UKalkiLogSubsystem::IsChannelEnabled(EKalkiLogChannel Channel) const
{
    const bool* EnabledPtr = ChannelEnabled.Find(Channel);
    return EnabledPtr ? *EnabledPtr : true; // Default to enabled if not found
}

void UKalkiLogSubsystem::ToggleChannel(EKalkiLogChannel Channel)
{
    bool bCurrentState = IsChannelEnabled(Channel);
    SetChannelEnabled(Channel, !bCurrentState);
}

void UKalkiLogSubsystem::SetFileLoggingEnabled(bool bEnabled)
{
    if (bEnabled && !bFileLoggingEnabled)
    {
        bFileLoggingEnabled = true;
        InitializeFileLogging();
    }
    else if (!bEnabled && bFileLoggingEnabled)
    {
        bFileLoggingEnabled = false;
        ShutdownFileLogging();
    }
}

void UKalkiLogSubsystem::FlushLogFile()
{
    if (FileWriter.IsValid())
    {
        FileWriter->Flush();
    }
}

void UKalkiLogSubsystem::RouteMessage(const FKalkiLogMessage& Message)
{
    // Output to Unreal's log system
    if (bOutputToUnrealLog)
    {
        OutputToUnrealLog(Message);
    }

    // Output to file
    if (bFileLoggingEnabled)
    {
        WriteToLogFile(Message);
    }
}

void UKalkiLogSubsystem::OutputToUnrealLog(const FKalkiLogMessage& Message)
{
    // Map our severity to Unreal's log verbosity
    switch (Message.Severity)
    {
        case EKalkiLogSeverity::Verbose:
            UE_LOG(LogTemp, Verbose, TEXT("[%s] %s"), 
                *UEnum::GetValueAsString(Message.Channel), *Message.Message);
            break;

        case EKalkiLogSeverity::Log:
            UE_LOG(LogTemp, Log, TEXT("[%s] %s"), 
                *UEnum::GetValueAsString(Message.Channel), *Message.Message);
            break;

        case EKalkiLogSeverity::Warning:
            UE_LOG(LogTemp, Warning, TEXT("[%s] %s"), 
                *UEnum::GetValueAsString(Message.Channel), *Message.Message);
            break;

        case EKalkiLogSeverity::Error:
            UE_LOG(LogTemp, Error, TEXT("[%s] %s"), 
                *UEnum::GetValueAsString(Message.Channel), *Message.Message);
            break;
    }
}

void UKalkiLogSubsystem::InitializeFileLogging()
{
    // Set log directory
    LogDirectory = FPaths::ProjectSavedDir() / TEXT("Logs");

    // Clean up old logs
    CleanupOldLogs();

    // Create new session log file
    CreateSessionLogFile();
}

void UKalkiLogSubsystem::ShutdownFileLogging()
{
    if (FileWriter.IsValid())
    {
        FileWriter->Flush();
        FileWriter.Reset();
    }
}

void UKalkiLogSubsystem::CreateSessionLogFile()
{
    // Generate filename with timestamp
    FDateTime Now = FDateTime::Now();
    FString Filename = FString::Printf(TEXT("Kalki_%s.log"), 
        *Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S")));
    
    CurrentLogFilePath = GetLogFilePath(Filename);

    // Create file writer
    FileWriter = MakeUnique<FKalkiLogFileWriter>(CurrentLogFilePath);

    // Write header
    FString Header = FString::Printf(
        TEXT("=================================================\n")
        TEXT("Kalki Game Log\n")
        TEXT("Session started: %s\n")
        TEXT("=================================================\n\n"),
        *Now.ToString()
    );
    
    if (FileWriter.IsValid())
    {
        FileWriter->QueueMessage(Header);
    }

    UE_LOG(LogTemp, Log, TEXT("KalkiLogSubsystem: Created log file: %s"), *CurrentLogFilePath);
}

void UKalkiLogSubsystem::WriteToLogFile(const FKalkiLogMessage& Message)
{
    if (!FileWriter.IsValid())
    {
        return;
    }

    // Format: [HH:MM:SS][Combat:Name][Channel][Severity] Message
    FString CombatInfo;
    if (Message.CombatSessionID >= 0)
    {
        CombatInfo = FString::Printf(TEXT("[Combat:%s]"), *Message.CombatSessionName);
    }

    FString FormattedMessage = FString::Printf(
        TEXT("[%s]%s[%s][%s] %s\n"),
        *Message.Timestamp.ToString(TEXT("%H:%M:%S.%s")),
        *CombatInfo, // NEW: Combat session info
        *UEnum::GetValueAsString(Message.Channel),
        *UEnum::GetValueAsString(Message.Severity),
        *Message.Message
    );

    // Add metadata if present and not None level
    if (MetadataLevel != EKalkiLogMetadataLevel::None && Message.Metadata.Num() > 0)
    {
        FormattedMessage += TEXT("  Metadata: ");
        for (const auto& Pair : Message.Metadata)
        {
            FormattedMessage += FString::Printf(TEXT("%s=%s, "), *Pair.Key, *Pair.Value);
        }
        FormattedMessage += TEXT("\n");
    }

    FileWriter->QueueMessage(FormattedMessage);

    // Flush immediately on warnings and errors
    if (Message.Severity == EKalkiLogSeverity::Warning || 
        Message.Severity == EKalkiLogSeverity::Error)
    {
        FileWriter->Flush();
    }
}

void UKalkiLogSubsystem::CleanupOldLogs()
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*LogDirectory))
    {
        return;
    }

    // Get all log files
    TArray<FString> LogFiles;
    PlatformFile.FindFiles(LogFiles, *LogDirectory, TEXT(".log"));

    // Sort by modification time (newest first)
    LogFiles.Sort([&PlatformFile](const FString& A, const FString& B) {
        return PlatformFile.GetTimeStamp(*A) > PlatformFile.GetTimeStamp(*B);
    });

    FDateTime Now = FDateTime::Now();
    int32 FilesKept = 0;

    for (const FString& LogFile : LogFiles)
    {
        // Skip crash logs (keep them always)
        if (LogFile.Contains(TEXT("Crash")))
        {
            continue;
        }

        FDateTime FileTime = PlatformFile.GetTimeStamp(*LogFile);
        FTimespan Age = Now - FileTime;

        // Keep if: within age limit OR within session count limit
        bool bKeep = (Age.GetDays() < MaxLogAgeInDays) || (FilesKept < MaxSessionFiles);

        if (bKeep)
        {
            FilesKept++;
        }
        else
        {
            // Delete old log
            PlatformFile.DeleteFile(*LogFile);
            UE_LOG(LogTemp, Log, TEXT("KalkiLogSubsystem: Deleted old log: %s"), *LogFile);
        }
    }
}

FString UKalkiLogSubsystem::GetLogFilePath(const FString& Filename) const
{
    return LogDirectory / Filename;
}

void UKalkiLogSubsystem::OnHandleSystemError()
{
    CreateCrashLog();
}

void UKalkiLogSubsystem::CreateCrashLog()
{
    // Generate crash log filename
    FDateTime Now = FDateTime::Now();
    FString CrashFilename = FString::Printf(TEXT("Kalki_Crash_%s.log"), 
        *Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S")));
    
    FString CrashLogPath = GetLogFilePath(CrashFilename);

    // Write crash log synchronously (can't use async when crashing)
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // Ensure directory exists
    FString Directory = FPaths::GetPath(CrashLogPath);
    if (!PlatformFile.DirectoryExists(*Directory))
    {
        PlatformFile.CreateDirectoryTree(*Directory);
    }
    
    IFileHandle* CrashFile = PlatformFile.OpenWrite(*CrashLogPath);

    if (!CrashFile)
    {
        return;
    }

    // Write header
    FString Header = FString::Printf(
        TEXT("=================================================\n")
        TEXT("KALKI CRASH LOG\n")
        TEXT("Crash occurred: %s\n")
        TEXT("=================================================\n\n"),
        *Now.ToString()
    );

    FTCHARToUTF8 HeaderUTF8(*Header);
    CrashFile->Write(reinterpret_cast<const uint8*>(HeaderUTF8.Get()), HeaderUTF8.Length());

    // Write last N messages from history
    int32 StartIndex = FMath::Max(0, LogHistory.Num() - CrashLogMessageCount);
    
    for (int32 i = StartIndex; i < LogHistory.Num(); ++i)
    {
        const FKalkiLogMessage& Msg = LogHistory[i];
        
        FString Line = FString::Printf(
            TEXT("[%s][%s][%s] %s\n"),
            *Msg.Timestamp.ToString(TEXT("%H:%M:%S")),
            *UEnum::GetValueAsString(Msg.Channel),
            *UEnum::GetValueAsString(Msg.Severity),
            *Msg.Message
        );

        FTCHARToUTF8 LineUTF8(*Line);
        CrashFile->Write(reinterpret_cast<const uint8*>(LineUTF8.Get()), LineUTF8.Length());
    }

    // Close file
    CrashFile->Flush();
    delete CrashFile;

    UE_LOG(LogTemp, Error, TEXT("KalkiLogSubsystem: Created crash log: %s"), *CrashLogPath);
}

void UKalkiLogSubsystem::StartCombatSession(const FString& CombatName)
{
    CurrentCombatSessionID = NextCombatSessionID++;
    
    // Generate name if not provided
    if (CombatName.IsEmpty())
    {
        CurrentCombatName = FString::Printf(TEXT("Combat %d"), CurrentCombatSessionID + 1);
    }
    else
    {
        CurrentCombatName = CombatName;
    }

    KalkiLog::System(
        FString::Printf(TEXT("Combat session started: %s (ID: %d)"), 
            *CurrentCombatName, CurrentCombatSessionID),
        EKalkiLogSeverity::Log
    );
}

void UKalkiLogSubsystem::EndCombatSession()
{
    if (CurrentCombatSessionID < 0)
    {
        KalkiLog::System(TEXT("EndCombatSession called but no combat active"), EKalkiLogSeverity::Warning);
        return;
    }

    KalkiLog::System(
        FString::Printf(TEXT("Combat session ended: %s (ID: %d)"), 
            *CurrentCombatName, CurrentCombatSessionID),
        EKalkiLogSeverity::Log
    );

    CurrentCombatSessionID = -1;
    CurrentCombatName.Empty();
}

void UKalkiLogSubsystem::SetCombatSessionName(const FString& NewName)
{
    if (CurrentCombatSessionID < 0)
    {
        KalkiLog::System(TEXT("SetCombatSessionName called but no combat active"), EKalkiLogSeverity::Warning);
        return;
    }

    FString OldName = CurrentCombatName;
    CurrentCombatName = NewName;

    KalkiLog::System(
        FString::Printf(TEXT("Combat session renamed: '%s' -> '%s'"), *OldName, *NewName),
        EKalkiLogSeverity::Log
    );
}