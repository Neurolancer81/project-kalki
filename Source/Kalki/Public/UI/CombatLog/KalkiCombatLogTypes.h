// Public/UI/CombatLog/KalkiCombatLogTypes.h

#pragma once

#include "CoreMinimal.h"
#include "Logging/KalkiLogTypes.h"
#include "KalkiCombatLogTypes.generated.h"

/**
 * Types of interactive elements in log messages
 * Used for hover tooltips and click interactions
 */
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

/**
 * Interactive element within a log message
 * Stores data for hover tooltips and click interactions
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiLogInteractiveElement
{
    GENERATED_BODY()

    // Type of this element
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    EKalkiLogElementType ElementType = EKalkiLogElementType::Character;

    // Text range in the display string
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    int32 StartIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    int32 EndIndex = 0;

    // Display text for this element
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    FString DisplayText;

    // Tooltip data (key-value pairs for tooltip display)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    TMap<FString, FString> TooltipData;

    // Reference to source object (if applicable)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    TWeakObjectPtr<UObject> SourceObject;

    FKalkiLogInteractiveElement()
        : ElementType(EKalkiLogElementType::Character)
        , StartIndex(0)
        , EndIndex(0)
    {
    }
};

/**
 * UI-friendly display entry for a single log message
 * Contains formatted text, color, and interactive elements
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiLogDisplayEntry
{
    GENERATED_BODY()

    // Display text (formatted for UI)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    FText DisplayText;

    // Color for this message (based on severity)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    FLinearColor Color = FLinearColor::White;

    // Severity (for icons, styling)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    EKalkiLogSeverity Severity = EKalkiLogSeverity::Log;

    // Channel (for filtering)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    EKalkiLogChannel Channel = EKalkiLogChannel::System;

    // Interactive elements (for tooltips/clicks) - FUTURE USE
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    TArray<FKalkiLogInteractiveElement> InteractiveElements;

    // Original message (for full detail)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
    FKalkiLogMessage OriginalMessage;

    FKalkiLogDisplayEntry()
        : Color(FLinearColor::White)
        , Severity(EKalkiLogSeverity::Log)
        , Channel(EKalkiLogChannel::System)
    {
    }
};