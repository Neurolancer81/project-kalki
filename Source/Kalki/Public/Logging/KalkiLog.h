// Public/Logging/KalkiLog.h

#pragma once

#include "CoreMinimal.h"
#include "Logging/KalkiLogTypes.h"

class UKalkiLogSubsystem;

/**
 * Static helper functions for convenient logging throughout the codebase
 * Usage: KalkiLog::Combat(TEXT("Arjuna attacks Goblin"));
 */
namespace KalkiLog
{
    // Get the log subsystem (helper function)
    KALKI_API UKalkiLogSubsystem* GetLogSubsystem(const UObject* WorldContextObject);

    // Channel-specific logging functions
    
    KALKI_API void System(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Combat(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Movement(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Abilities(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void TurnSystem(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Grid(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Dialogue(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Inventory(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void AI(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Network(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Quest(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Ruleset(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Log, const UObject* WorldContextObject = nullptr);
    
    KALKI_API void Debug(const FString& Message, EKalkiLogSeverity Severity = EKalkiLogSeverity::Verbose, const UObject* WorldContextObject = nullptr);

    // Advanced: Log with full message struct (for metadata)
    KALKI_API void LogMessage(const FKalkiLogMessage& Message, const UObject* WorldContextObject = nullptr);

    // Utility: Log with metadata helpers
    KALKI_API void CombatWithMetadata(
        const FString& Message, 
        const TMap<FString, FString>& Metadata,
        EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
        const UObject* WorldContextObject = nullptr);

    KALKI_API void MovementWithMetadata(
        const FString& Message, 
        const TMap<FString, FString>& Metadata,
        EKalkiLogSeverity Severity = EKalkiLogSeverity::Log,
        const UObject* WorldContextObject = nullptr);
}