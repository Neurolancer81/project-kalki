// Copyright of V.S. Puranam and no one else

#include "Debug/KalkiCheatManager.h"
#include "UI/Common/KalkiHUD.h"
#include "Logging/KalkiLogSubsystem.h"
#include "Logging/KalkiLog.h"
#include "GameFramework/PlayerController.h"

void UKalkiCheatManager::StartCombat(const FString& CombatName)
{
    if (UKalkiLogSubsystem* LogSys = GetWorld()->GetSubsystem<UKalkiLogSubsystem>())
    {
        LogSys->StartCombatSession(CombatName);
        KalkiLog::System(FString::Printf(TEXT("Started combat: %s"), *CombatName), EKalkiLogSeverity::Log, this);
    }
}

void UKalkiCheatManager::EndCombat()
{
    if (UKalkiLogSubsystem* LogSys = GetWorld()->GetSubsystem<UKalkiLogSubsystem>())
    {
        LogSys->EndCombatSession();
        KalkiLog::System(TEXT("Ended combat session"), EKalkiLogSeverity::Log, this);
    }
}

void UKalkiCheatManager::TestCombatLog()
{
    UKalkiLogSubsystem* LogSys = GetWorld()->GetSubsystem<UKalkiLogSubsystem>();
    if (!LogSys)
    {
        return;
    }

    // Start new combat session
    static int32 CombatCounter = 0;
    CombatCounter++;
    
    LogSys->StartCombatSession(FString::Printf(TEXT("Test Combat %d"), CombatCounter));

    // Log test messages
    KalkiLog::Combat(TEXT("Combat started!"));
    
    TMap<FString, FString> MoveData;
    MoveData.Add(TEXT("FromTile"), TEXT("(2,4)"));
    MoveData.Add(TEXT("ToTile"), TEXT("(5,7)"));
    MoveData.Add(TEXT("MovementCost"), TEXT("3"));
    KalkiLog::MovementWithMetadata(TEXT("Arjuna moves from (2,4) to (5,7)"), MoveData);
    
    TMap<FString, FString> AttackData;
    AttackData.Add(TEXT("AttackerPos"), TEXT("(5,7)"));
    AttackData.Add(TEXT("TargetPos"), TEXT("(6,8)"));
    AttackData.Add(TEXT("DiceRoll"), TEXT("d20(15)+3=18"));
    AttackData.Add(TEXT("Damage"), TEXT("8"));
    KalkiLog::CombatWithMetadata(TEXT("Arjuna attacks Goblin - Hit! 8 damage"), AttackData);
    
    KalkiLog::Combat(TEXT("Goblin takes 8 damage"), EKalkiLogSeverity::Warning);
    KalkiLog::Combat(TEXT("Goblin dies!"), EKalkiLogSeverity::Error);
    
    KalkiLog::System(TEXT("Combat complete"));
}

void UKalkiCheatManager::LogMessage(const FString& Message)
{
    KalkiLog::Combat(Message);
}

void UKalkiCheatManager::ShowCombatUI()
{
    if (APlayerController* PC = GetOuterAPlayerController())
    {
        if (AKalkiHUD* KalkiHUD = Cast<AKalkiHUD>(PC->GetHUD()))
        {
            KalkiHUD->SwitchToCombatMode();
        }
    }
}

void UKalkiCheatManager::ShowStrategyUI()
{
    if (APlayerController* PC = GetOuterAPlayerController())
    {
        if (AKalkiHUD* KalkiHUD = Cast<AKalkiHUD>(PC->GetHUD()))
        {
            KalkiHUD->SwitchToStrategyMode();
        }
    }
}

void UKalkiCheatManager::ToggleUIMode()
{
    if (APlayerController* PC = GetOuterAPlayerController())
    {
        if (AKalkiHUD* KalkiHUD = Cast<AKalkiHUD>(PC->GetHUD()))
        {
            if (KalkiHUD->GetCurrentMode() == EKalkiGameMode::Combat)
            {
                KalkiHUD->SwitchToStrategyMode();
            }
            else
            {
                KalkiHUD->SwitchToCombatMode();
            }
        }
    }
}