// Public/Logging/KalkiLogSubsystem.h
// Add these to the existing class (additions marked with comments)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Logging/KalkiLogTypes.h"
#include "HAL/Runnable.h"
#include "KalkiLogFileWriter.h"
#include "KalkiLogSubsystem.generated.h"

// Forward declarations
// class FKalkiLogFileWriter;

// Delegate for when a new log message is added (UI can bind to this)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogMessageAdded, const FKalkiLogMessage&, Message);

/**
 * Central logging system for the game
 * Manages log history, routing, and filtering
 */
UCLASS()
class KALKI_API UKalkiLogSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Subsystem lifecycle
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Core logging function
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void LogMessage(const FKalkiLogMessage& Message);

    // Convenience function
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void Log(EKalkiLogChannel Channel, EKalkiLogSeverity Severity, const FString& Message);

    // Get log history
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    const TArray<FKalkiLogMessage>& GetLogHistory() const { return LogHistory; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    TArray<FKalkiLogMessage> GetLogHistoryFiltered(EKalkiLogChannel Channel) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void ClearLogHistory();

    // Channel filtering
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void SetChannelEnabled(EKalkiLogChannel Channel, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    bool IsChannelEnabled(EKalkiLogChannel Channel) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void ToggleChannel(EKalkiLogChannel Channel);

    // Metadata level control
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void SetMetadataLevel(EKalkiLogMetadataLevel Level) { MetadataLevel = Level; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    EKalkiLogMetadataLevel GetMetadataLevel() const { return MetadataLevel; }

    // Output to Unreal's log
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void SetOutputToUnrealLog(bool bEnabled) { bOutputToUnrealLog = bEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    bool IsOutputToUnrealLog() const { return bOutputToUnrealLog; }

    // File logging controls
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void SetFileLoggingEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    bool IsFileLoggingEnabled() const { return bFileLoggingEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    FString GetCurrentLogFilePath() const { return CurrentLogFilePath; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging")
    void FlushLogFile();

    // Event for UI to listen to
    UPROPERTY(BlueprintAssignable, Category = "Kalki|Logging")
    FOnLogMessageAdded OnLogMessageAdded;

    // Combat session management
    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging|Combat")
    void StartCombatSession(const FString& CombatName = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging|Combat")
    void EndCombatSession();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging|Combat")
    void SetCombatSessionName(const FString& NewName);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging|Combat")
    int32 GetCurrentCombatSessionID() const { return CurrentCombatSessionID; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging|Combat")
    FString GetCurrentCombatName() const { return CurrentCombatName; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Logging|Combat")
    bool IsInCombat() const { return CurrentCombatSessionID >= 0; }

protected:
    // Storage for log messages (circular buffer concept)
    UPROPERTY()
    TArray<FKalkiLogMessage> LogHistory;

    // Maximum messages to keep in memory
    UPROPERTY(EditDefaultsOnly, Category = "Kalki|Logging")
    int32 MaxLogHistory = 1000;

    // Channel enable/disable state
    UPROPERTY()
    TMap<EKalkiLogChannel, bool> ChannelEnabled;

    // Metadata detail level
    UPROPERTY()
    EKalkiLogMetadataLevel MetadataLevel = EKalkiLogMetadataLevel::Full;

    // Whether to also output to Unreal's log system
    UPROPERTY()
    bool bOutputToUnrealLog = true;

    // File logging settings
    UPROPERTY(EditDefaultsOnly, Category = "Kalki|Logging|File")
    bool bFileLoggingEnabled = true;

    UPROPERTY(EditDefaultsOnly, Category = "Kalki|Logging|File")
    int32 MaxSessionFiles = 10;

    UPROPERTY(EditDefaultsOnly, Category = "Kalki|Logging|File")
    int32 MaxLogAgeInDays = 7;

    UPROPERTY(EditDefaultsOnly, Category = "Kalki|Logging|File")
    int32 CrashLogMessageCount = 500;

    // File logging state
    FString CurrentLogFilePath;
    FString LogDirectory;
    TUniquePtr<FKalkiLogFileWriter> FileWriter;

    // Helper to route message to appropriate outputs
    void RouteMessage(const FKalkiLogMessage& Message);

    // Output to Unreal's log
    void OutputToUnrealLog(const FKalkiLogMessage& Message);

    // File logging functions
    void InitializeFileLogging();
    void ShutdownFileLogging();
    void CreateSessionLogFile();
    void WriteToLogFile(const FKalkiLogMessage& Message);
    void CleanupOldLogs();
    FString GetLogFilePath(const FString& Filename) const;
    
    // Crash handling
    void OnHandleSystemError();
    void CreateCrashLog();

    // Initialize channel states
    void InitializeChannels();

    // Combat session tracking
    UPROPERTY()
    int32 CurrentCombatSessionID = -1;

    UPROPERTY()
    int32 NextCombatSessionID = 0;

    UPROPERTY()
    FString CurrentCombatName;

    
};