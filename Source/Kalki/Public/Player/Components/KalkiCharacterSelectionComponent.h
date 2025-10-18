// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KalkiCharacterSelectionComponent.generated.h"

// Forward declarations
class AKalkiCharacter;
class AKalkiGridVisualizer;
class UKalkiGridManager;

/**
 * Character Selection Component
 * Manages selection of characters controlled by THIS player
 * Each PlayerController has their own component tracking their assigned characters
 * 
 * MULTIPLAYER USAGE (1-6 players controlling 1-6 characters total):
 * - 1 Player:  Controls all 1-6 characters
 * - 2 Players: Split 1-6 characters any way (e.g., 3-3, 4-2, 5-1)
 * - 3 Players: Split 1-6 characters any way (e.g., 2-2-2, 3-2-1)
 * - 4 Players: Split 1-6 characters any way (e.g., 2-2-1-1, 3-1-1-1)
 * - 5 Players: Split 1-6 characters any way (e.g., 2-1-1-1-1)
 * - 6 Players: Each controls 1 character
 * 
 * Characters can be reassigned dynamically during gameplay.
 */
UCLASS(ClassGroup=(Kalki), meta=(BlueprintSpawnableComponent))
class KALKI_API UKalkiCharacterSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKalkiCharacterSelectionComponent();

protected:
	virtual void BeginPlay() override;

public:
	// ========================================
	// SETTINGS
	// ========================================

	/** Show movement range when character selected? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Character Selection")
	bool bShowMovementRangeOnSelection = true;

	/** Default movement range (for testing - will come from character stats later) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Character Selection")
	int32 DefaultMovementRange = 5;

	// ========================================
	// STATE
	// ========================================

	/** Currently selected character (for movement/actions) */
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Character Selection")
	TObjectPtr<AKalkiCharacter> SelectedCharacter;

	/** Characters controlled by THIS player (can be 1-6 depending on team split) */
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Character Selection")
	TArray<TObjectPtr<AKalkiCharacter>> ControlledCharacters;

	// ========================================
	// CHARACTER CONTROL (Assignment)
	// ========================================

	/** Assign a character to this player's control */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Control")
	bool AssignCharacter(AKalkiCharacter* Character);

	/** Remove a character from this player's control */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Control")
	bool UnassignCharacter(AKalkiCharacter* Character);

	/** Reassign a character from another player to this player */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Control")
	bool ReassignCharacterFrom(AKalkiCharacter* Character, APlayerController* FromController);

	/** Transfer a character to another player */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Control")
	bool TransferCharacterTo(AKalkiCharacter* Character, APlayerController* ToController);

	/** Clear all character assignments */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Control")
	void ClearAllAssignments();

	/** Is this character controlled by this player? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Control")
	bool IsControllingCharacter(AKalkiCharacter* Character) const;

	/** Get number of characters controlled by this player */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Control")
	int32 GetControlledCharacterCount() const { return ControlledCharacters.Num(); }

	/** Get all controlled characters */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Control")
	TArray<AKalkiCharacter*> GetControlledCharacters() const { return ControlledCharacters; }

	// ========================================
	// SELECTION
	// ========================================

	/** Select a character (must be controlled by this player) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Selection")
	void SelectCharacter(AKalkiCharacter* Character);

	/** Deselect current character */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Selection")
	void DeselectCharacter();

	/** Get currently selected character */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Selection")
	AKalkiCharacter* GetSelectedCharacter() const { return SelectedCharacter; }

	/** Is a character selected? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Selection")
	bool HasCharacterSelected() const { return SelectedCharacter != nullptr; }

	// ========================================
	// QUICK SELECTION (Number Keys & Tab)
	// ========================================

	/** Select character by slot (0-5 for keys 1-6) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Selection")
	void SelectCharacterBySlot(int32 SlotIndex);

	/** Cycle to next controlled character */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Selection")
	void SelectNextCharacter();

	/** Cycle to previous controlled character */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Character Selection")
	void SelectPreviousCharacter();

	/** Get character in slot (0-5) - returns null if slot empty */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Selection")
	AKalkiCharacter* GetCharacterInSlot(int32 SlotIndex) const;

	/** Get index of character in controlled list (-1 if not controlled) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Character Selection")
	int32 GetCharacterSlotIndex(AKalkiCharacter* Character) const;

private:
	// ========================================
	// CACHED REFERENCES
	// ========================================

	UPROPERTY()
	TObjectPtr<UKalkiGridManager> GridManager;

	UPROPERTY()
	TObjectPtr<AKalkiGridVisualizer> GridVisualizer;

	UPROPERTY()
	TObjectPtr<APlayerController> OwningController;

	// ========================================
	// INTERNAL FUNCTIONS
	// ========================================

	/** Cache subsystem references */
	void CacheReferences();

	/** Update movement range visualization */
	void UpdateMovementRangeDisplay();

	/** Clear movement range visualization */
	void ClearMovementRangeDisplay();

	/** Validate character is controlled by this player */
	bool ValidateCharacterControl(AKalkiCharacter* Character) const;
};