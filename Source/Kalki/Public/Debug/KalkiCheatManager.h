// Copyright of V.S. Puranam and no one else

// Public/Cheats/KalkiCheatManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "KalkiCheatManager.generated.h"

// Forward Declarations
class AKalkiCameraPawn;
/**
 * Cheat Manager for development/testing
 * Provides console commands for testing game systems
 */
UCLASS()
class KALKI_API UKalkiCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
   
    // === Debug visualization ===
    UFUNCTION(Exec, Category = "Kalki|Stats")
    void DrawDebugStats() const;

    // // Or simpler - apply damage/heal directly through GAS
    // UFUNCTION(Exec, Category = "Kalki|Stats")
    // void ApplyHealthChange();
    
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

    // ========================================
    // CAMERA COMMANDS
    // ========================================

    /** Set camera zoom distance */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void SetCameraDistance(float Distance);

    /** Set camera pitch angle (negative = looking down) */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void SetCameraPitch(float Pitch);

    /** Set camera yaw rotation */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void SetCameraYaw(float Yaw);

    /** Teleport camera to location */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void TeleportCamera(float X, float Y, float Z);

    /** Toggle camera bounds */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void ToggleCameraBounds();

    /** Toggle smooth camera rotation */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void ToggleSmoothRotation();

    /** Toggle smooth camera zoom */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void ToggleSmoothZoom();

    /** Print camera info */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void PrintCameraInfo();

    /** Snap camera to rotation increment */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void SnapCameraRotation();

    /** Set camera rotation increment */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void SetRotationIncrement(float Degrees);

    /** Enable/disable edge scrolling */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void ToggleEdgeScrolling();

    /** Set camera pan speed */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void SetCameraPanSpeed(float Speed);

    /** Reset camera to default settings */
    UFUNCTION(Exec, Category = "Kalki|Camera")
    void ResetCamera();
    
    /** Get camera pawn from player controller */
    AKalkiCameraPawn* GetCameraPawn() const;

    // ========================================
    // CHARACTER CONTROL COMMANDS
    // ========================================

    /** Test: Assign a test character to current player */
    UFUNCTION(Exec, Category = "Kalki|Character")
    void TestAssignCharacter(int32 CharacterIndex);

    /** Test: Select character by slot */
    UFUNCTION(Exec, Category = "Kalki|Character")
    void TestSelectCharacter(int32 SlotIndex);

    /** Test: Cycle to next character */
    UFUNCTION(Exec, Category = "Kalki|Character")
    void TestCycleCharacter();

    /** Print controlled characters */
    UFUNCTION(Exec, Category = "Kalki|Character")
    void PrintControlledCharacters();

};