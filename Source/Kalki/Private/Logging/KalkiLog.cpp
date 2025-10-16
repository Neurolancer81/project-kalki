// Private/Logging/KalkiLog.cpp

#include "Logging/KalkiLog.h"
#include "Logging/KalkiLogSubsystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

namespace KalkiLog
{
    // Private helper function
    static void LogToChannel(
        EKalkiLogChannel Channel, 
        const FString& Message, 
        EKalkiLogSeverity Severity, 
        const UObject* WorldContextObject)
    {
        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);
        if (LogSubsystem)
        {
            LogSubsystem->Log(Channel, Severity, Message);
        }
    }

    UKalkiLogSubsystem* GetLogSubsystem(const UObject* WorldContextObject)
    {
        // Try to get world from context object
        UWorld* World = nullptr;
        
        if (WorldContextObject)
        {
            World = WorldContextObject->GetWorld();
        }
        
        // Fallback: try to get from game instance
        if (!World && GEngine)
        {
            // Try to get first valid world
            for (const FWorldContext& Context : GEngine->GetWorldContexts())
            {
                if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
                {
                    World = Context.World();
                    break;
                }
            }
        }

        if (!World)
        {
            return nullptr;
        }

        return World->GetSubsystem<UKalkiLogSubsystem>();
    }

    // Now all channel functions are one-liners

    void System(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::System, Message, Severity, WorldContextObject);
    }

    void Combat(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Combat, Message, Severity, WorldContextObject);
    }

    void Movement(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Movement, Message, Severity, WorldContextObject);
    }

    void Abilities(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Abilities, Message, Severity, WorldContextObject);
    }

    void TurnSystem(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::TurnSystem, Message, Severity, WorldContextObject);
    }

    void Grid(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Grid, Message, Severity, WorldContextObject);
    }

    void Dialogue(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Dialogue, Message, Severity, WorldContextObject);
    }

    void Inventory(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Inventory, Message, Severity, WorldContextObject);
    }

    void AI(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::AI, Message, Severity, WorldContextObject);
    }

    void Network(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Network, Message, Severity, WorldContextObject);
    }

    void Quest(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Quest, Message, Severity, WorldContextObject);
    }

    void Ruleset(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Ruleset, Message, Severity, WorldContextObject);
    }

    void Debug(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::Debug, Message, Severity, WorldContextObject);
    }

    void UI(const FString& Message, EKalkiLogSeverity Severity, const UObject* WorldContextObject)
    {
        LogToChannel(EKalkiLogChannel::UI, Message, Severity, WorldContextObject);
    }

    void LogMessage(const FKalkiLogMessage& Message, const UObject* WorldContextObject)
    {
        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);
        if (LogSubsystem)
        {
            LogSubsystem->LogMessage(Message);
        }
    }

    void CombatWithMetadata(
        const FString& Message, 
        const TMap<FString, FString>& Metadata,
        EKalkiLogSeverity Severity,
        const UObject* WorldContextObject)
    {
        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);
        if (LogSubsystem)
        {
            FKalkiLogMessage LogMsg(EKalkiLogChannel::Combat, Severity, Message);
            LogMsg.Metadata = Metadata;
            LogSubsystem->LogMessage(LogMsg);
        }
    }

    void MovementWithMetadata(
        const FString& Message, 
        const TMap<FString, FString>& Metadata,
        EKalkiLogSeverity Severity,
        const UObject* WorldContextObject)
    {
        UKalkiLogSubsystem* LogSubsystem = GetLogSubsystem(WorldContextObject);
        if (LogSubsystem)
        {
            FKalkiLogMessage LogMsg(EKalkiLogChannel::Movement, Severity, Message);
            LogMsg.Metadata = Metadata;
            LogSubsystem->LogMessage(LogMsg);
        }
    }
}