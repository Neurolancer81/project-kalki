// Public/Logging/KalkiLogTypes.h

#pragma once

#include "CoreMinimal.h"
#include "KalkiLogTypes.generated.h"

/**
 * Categories of log messages
 * Used to filter and route logs appropriately
 */
UENUM(BlueprintType)
enum class EKalkiLogChannel : uint8
{
    System      UMETA(DisplayName = "System"),
    Combat      UMETA(DisplayName = "Combat"),
    Movement    UMETA(DisplayName = "Movement"),
    Abilities   UMETA(DisplayName = "Abilities"),
    TurnSystem  UMETA(DisplayName = "Turn System"),
    Grid        UMETA(DisplayName = "Grid"),
    Dialogue    UMETA(DisplayName = "Dialogue"),
    Inventory   UMETA(DisplayName = "Inventory"),
    AI          UMETA(DisplayName = "AI"),
    Network     UMETA(DisplayName = "Network"),
    Quest       UMETA(DisplayName = "Quest"),
    Ruleset     UMETA(DisplayName = "Ruleset"),
    Debug       UMETA(DisplayName = "Debug"),
    UI          UMETA(DisplayName = "UI")
};

/**
 * Severity levels for log messages
 * Determines visibility and routing
 */
UENUM(BlueprintType)
enum class EKalkiLogSeverity : uint8
{
    Verbose     UMETA(DisplayName = "Verbose"),  // Detailed debug info
    Log         UMETA(DisplayName = "Log"),      // Normal information
    Warning     UMETA(DisplayName = "Warning"),  // Something unusual
    Error       UMETA(DisplayName = "Error")     // Something went wrong
};

/**
 * Metadata detail level
 * Controls how much additional data is logged
 */
UENUM(BlueprintType)
enum class EKalkiLogMetadataLevel : uint8
{
    None        UMETA(DisplayName = "None"),     // No metadata
    Minimal     UMETA(DisplayName = "Minimal"),  // Basic metadata only
    Full        UMETA(DisplayName = "Full")      // All metadata
};

/**
 * Individual log message
 * Contains all information about a single log event
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiLogMessage
{
    GENERATED_BODY()

    // When this message was logged
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    FDateTime Timestamp;

    // Category of this message
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    EKalkiLogChannel Channel = EKalkiLogChannel::System;

    // Severity level
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    EKalkiLogSeverity Severity = EKalkiLogSeverity::Log;

    // The actual log message
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    FString Message;

    // Optional metadata (key-value pairs)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    TMap<FString, FString> Metadata;


    // Combat session this message belongs to (-1 = not in combat)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    int32 CombatSessionID = -1;

    // Combat session name (empty if not in combat)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Logging")
    FString CombatSessionName;

    // What object triggered this log (if applicable)
    UPROPERTY()
    TWeakObjectPtr<UObject> Source;

    // Constructor
    FKalkiLogMessage()
        : Timestamp(FDateTime::Now())
        , Channel(EKalkiLogChannel::System)
        , Severity(EKalkiLogSeverity::Log)
    {
    }

    FKalkiLogMessage(EKalkiLogChannel InChannel, EKalkiLogSeverity InSeverity, const FString& InMessage)
        : Timestamp(FDateTime::Now())
        , Channel(InChannel)
        , Severity(InSeverity)
        , Message(InMessage)
    {
    }

    // Get formatted string for display
    FString ToString(bool bIncludeTimestamp = true, bool bIncludeChannel = true) const
    {
        FString Result;

        if (bIncludeTimestamp)
        {
            Result += FString::Printf(TEXT("[%s]"), *Timestamp.ToString(TEXT("%H:%M:%S")));
        }

        if (bIncludeChannel)
        {
            Result += FString::Printf(TEXT("[%s]"), *UEnum::GetValueAsString(Channel));
        }

        Result += FString::Printf(TEXT("[%s] %s"), *UEnum::GetValueAsString(Severity), *Message);

        return Result;
    }

    // Get color based on severity
    FLinearColor GetColor() const
    {
        switch (Severity)
        {
            case EKalkiLogSeverity::Verbose:
                return FLinearColor::Gray;
            case EKalkiLogSeverity::Log:
                return FLinearColor::White;
            case EKalkiLogSeverity::Warning:
                return FLinearColor::Yellow;
            case EKalkiLogSeverity::Error:
                return FLinearColor::Red;
            default:
                return FLinearColor::White;
        }
    }
};