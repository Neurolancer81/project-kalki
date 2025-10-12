// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "KalkiCheatManager.generated.h"

/**
 * Cheat Manager for Kalki
 * Contains all debug/test commands
 * Automatically excluded from shipping builds
 */
UCLASS()
class KALKI_API UKalkiCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    // ============================================
    // Combat Testing
    // ============================================

    /** Start a combat session with optional name */
    UFUNCTION(Exec, Category = "Kalki|Combat")
    void StartCombat(const FString& CombatName = TEXT("Test Combat"));

    /** End current combat session */
    UFUNCTION(Exec, Category = "Kalki|Combat")
    void EndCombat();

    /** Test combat log with sample messages */
    UFUNCTION(Exec, Category = "Kalki|Combat")
    void TestCombatLog();

    /** Log a custom test message */
    UFUNCTION(Exec, Category = "Kalki|Combat")
    void LogMessage(const FString& Message);

    // ============================================
    // UI Testing
    // ============================================

    /** Switch to combat mode */
    UFUNCTION(Exec, Category = "Kalki|UI")
    void ShowCombatUI();

    /** Switch to strategy mode */
    UFUNCTION(Exec, Category = "Kalki|UI")
    void ShowStrategyUI();

    /** Toggle between modes */
    UFUNCTION(Exec, Category = "Kalki|UI")
    void ToggleUIMode();
};