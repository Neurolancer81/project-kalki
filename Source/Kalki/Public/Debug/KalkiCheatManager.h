// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "KalkiCheatManager.generated.h"

// Forward declarations
class AKalkiCharacter;
class UKalkiGridManager;

/**
 * Kalki Cheat Manager
 * Development and debugging commands
 */
UCLASS()
class KALKI_API UKalkiCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	// ========================================
	// GRID COMMANDS
	// ========================================

	/** Spawn a test character and place on grid */
	UFUNCTION(Exec, Category = "Kalki|Grid")
	void SpawnTestCharacter(int32 X, int32 Y);

	/** Place selected character on grid */
	UFUNCTION(Exec, Category = "Kalki|Grid")
	void PlaceCharacterOnGrid(int32 X, int32 Y);

	/** Remove selected character from grid */
	UFUNCTION(Exec, Category = "Kalki|Grid")
	void RemoveCharacterFromGrid();

	/** Print grid position of selected character */
	UFUNCTION(Exec, Category = "Kalki|Grid")
	void PrintCharacterGridPosition();

	/** Snap all characters to nearest grid tiles */
	UFUNCTION(Exec, Category = "Kalki|Grid")
	void SnapAllCharactersToGrid();

	/** Force snap selected character to nearest tile */
	UFUNCTION(Exec, Category = "Kalki|Grid")
	void SnapSelectedCharacter();

	// ========================================
	// CHARACTER CONTROL COMMANDS
	// ========================================

	/** Assign all characters in level to current player */
	UFUNCTION(Exec, Category = "Kalki|Character")
	void AssignAllCharacters();

	/** Assign character by name */
	UFUNCTION(Exec, Category = "Kalki|Character")
	void AssignCharacterByName(const FString& CharacterName);

	/** Select character by slot */
	UFUNCTION(Exec, Category = "Kalki|Character")
	void TestSelectCharacter(int32 SlotIndex);

	/** Cycle to next character */
	UFUNCTION(Exec, Category = "Kalki|Character")
	void TestCycleCharacter();

	/** Print controlled characters */
	UFUNCTION(Exec, Category = "Kalki|Character")
	void PrintControlledCharacters();

	// ========================================
	// CHARACTER DEBUG COMMANDS
	// ========================================

	/** Toggle debug stats display for all characters */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void ToggleCharacterDebug();

	/** Toggle debug stats for selected character only */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void ToggleSelectedCharacterDebug();

	/** Draw debug info for all characters (one frame) */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void ShowAllCharacterStats();

	/** Draw detailed grid alignment debug for all characters */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void ShowCharacterGridAlignment();

	/** Print detailed character info to console */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void PrintCharacterInfo(const FString& CharacterName);

	/** Print all characters in level with their grid positions */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void PrintAllCharacters();

	// ========================================
	// HEALTH DEBUG COMMANDS
	// ========================================

	/** Damage selected character */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void DamageCharacter(float Amount);

	/** Heal selected character */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void HealCharacter(float Amount);

	/** Set selected character health */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void SetCharacterHealth(float Health);

	/** Kill selected character */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void KillCharacter();

	/** Full heal selected character */
	UFUNCTION(Exec, Category = "Kalki|Debug")
	void FullHealCharacter();

private:
	// ========================================
	// HELPER FUNCTIONS
	// ========================================

	/** Get selected character from player controller */
	AKalkiCharacter* GetSelectedCharacter();

	/** Get grid manager */
	UKalkiGridManager* GetGridManager();

	/** Draw character stats in world */
	void DrawCharacterStats(AKalkiCharacter* Character);

	/** Draw character grid alignment debug */
	void DrawCharacterGridAlignment(AKalkiCharacter* Character);

	/** Print character details to console */
	void PrintCharacterDetails(AKalkiCharacter* Character);

};