// Copyright of V.S. Puranam and no one else

// Public/Cheats/KalkiCheatManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "KalkiCheatManager.generated.h"

/**
 * Cheat Manager for development/testing
 * Provides console commands for testing game systems
 */
UCLASS()
class KALKI_API UKalkiCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    // === Combat Log Testing ===
    
    UFUNCTION(Exec, Category = "Kalki|CombatLog")
    void StartTestCombatLog();

    UFUNCTION(Exec, Category = "Kalki|CombatLog")
    void EndTestCombatLog();

    UFUNCTION(Exec, Category = "Kalki|CombatLog")
    void LogTestMessage(const FString& Message);

    // === UI Testing ===
    
    UFUNCTION(Exec, Category = "Kalki|UI")
    void ShowCombatUI();

    UFUNCTION(Exec, Category = "Kalki|UI")
    void ShowStrategyUI();

    UFUNCTION(Exec, Category = "Kalki|UI")
    void ToggleUIMode();

    // === Grid Testing ===
    
    UFUNCTION(Exec, Category = "Kalki|Grid")
    void CreateTestGrid(int32 SizeX = 20, int32 SizeY = 20);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void PrintGridInfo();

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void PrintTileInfo(int32 X, int32 Y);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void SetTileElevation(int32 X, int32 Y, float Elevation);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void SetTileWalkable(int32 X, int32 Y, bool bWalkable);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void CreateTestPlatform(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float Elevation);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void CreateTestRamp(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float StartElevation, float EndElevation);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void PrintNeighbors(int32 X, int32 Y, bool bDiagonalAllowed = true);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void PrintTilesInRange(int32 X, int32 Y, int32 Range);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void ShowGridVisualizer();

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void HideGridVisualizer();

    // Grid selection commands
    UFUNCTION(Exec, Category = "Kalki|Grid")
    void SelectGridTile(int32 X, int32 Y);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void DeselectGridTile();

    // Movement range preview commands
    UFUNCTION(Exec, Category = "Kalki|Grid")
    void ShowGridMovementRange(int32 X, int32 Y, int32 Range);

    UFUNCTION(Exec, Category = "Kalki|Grid")
    void HideGridMovementRange();

    // Scale grid
    UFUNCTION(Exec, Category = "Kalki|Grid")
    void SetGridTileScale(float Scale);
    
};