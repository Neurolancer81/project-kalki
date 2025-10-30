// Copyright of V.S. Puranam and no one else

#include "Player/Components/KalkiCharacterSelectionComponent.h"
#include "Characters/KalkiCharacter.h"
#include "Grid/KalkiGridManager.h"
#include "Grid/KalkiGridVisualizer.h"
#include "GameFramework/PlayerController.h"
#include "Player/KalkiPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/KalkiLog.h"

UKalkiCharacterSelectionComponent::UKalkiCharacterSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UKalkiCharacterSelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheReferences();

	KalkiLog::System(TEXT("CharacterSelectionComponent initialized"));
}

// ========================================
// CHARACTER CONTROL (Assignment)
// ========================================

bool UKalkiCharacterSelectionComponent::AssignCharacter(AKalkiCharacter* Character)
{
	if (!Character)
	{
		KalkiLog::System(
			TEXT("CharacterSelectionComponent - Cannot assign null character"),
			EKalkiLogSeverity::Warning
		);
		return false;
	}

	// Check if already controlling this character
	if (ControlledCharacters.Contains(Character))
	{
		KalkiLog::System(
			FString::Printf(TEXT("Already controlling character: %s"), *Character->GetName()),
			EKalkiLogSeverity::Verbose
		);
		return false;
	}

	// Assign character
	ControlledCharacters.Add(Character);

	KalkiLog::System(
		FString::Printf(TEXT("Character assigned to Player %d: %s (Total controlled: %d)"), 
			OwningController ? OwningController->GetLocalPlayer()->GetControllerId() : -1,
			*Character->GetName(), 
			ControlledCharacters.Num())
	);

	// Auto-select first assigned character
	if (ControlledCharacters.Num() == 1 && !SelectedCharacter)
	{
		SelectCharacter(Character);
	}

	return true;
}

bool UKalkiCharacterSelectionComponent::UnassignCharacter(AKalkiCharacter* Character)
{
	if (!Character)
	{
		return false;
	}

	if (!ControlledCharacters.Contains(Character))
	{
		KalkiLog::System(
			FString::Printf(TEXT("Not controlling character: %s"), *Character->GetName()),
			EKalkiLogSeverity::Warning
		);
		return false;
	}

	// Deselect if currently selected
	if (SelectedCharacter == Character)
	{
		DeselectCharacter();

		// Auto-select another character if available
		if (ControlledCharacters.Num() > 1)
		{
			// Select next character (or first if this was the last)
			int32 CharIndex = ControlledCharacters.IndexOfByKey(Character);
			int32 NextIndex = (CharIndex + 1) % ControlledCharacters.Num();
			if (NextIndex != CharIndex && ControlledCharacters.IsValidIndex(NextIndex))
			{
				SelectCharacter(ControlledCharacters[NextIndex]);
			}
		}
	}

	// Remove from controlled list
	ControlledCharacters.Remove(Character);

	KalkiLog::System(
		FString::Printf(TEXT("Character unassigned: %s (Remaining: %d)"), 
			*Character->GetName(), 
			ControlledCharacters.Num())
	);

	return true;
}

bool UKalkiCharacterSelectionComponent::ReassignCharacterFrom(AKalkiCharacter* Character, APlayerController* FromController)
{
	if (!Character || !FromController)
	{
		return false;
	}

	// Get other player's selection component
	AKalkiPlayerController* FromKalkiPC = Cast<AKalkiPlayerController>(FromController);
	if (!FromKalkiPC || !FromKalkiPC->CharacterSelectionComponent)
	{
		KalkiLog::System(
			TEXT("ReassignCharacterFrom - FromController doesn't have CharacterSelectionComponent"),
			EKalkiLogSeverity::Warning
		);
		return false;
	}

	// Remove from other player
	if (!FromKalkiPC->CharacterSelectionComponent->UnassignCharacter(Character))
	{
		return false;
	}

	// Assign to this player
	return AssignCharacter(Character);
}

bool UKalkiCharacterSelectionComponent::TransferCharacterTo(AKalkiCharacter* Character, APlayerController* ToController)
{
	if (!Character || !ToController)
	{
		return false;
	}

	// Verify we control this character
	if (!IsControllingCharacter(Character))
	{
		KalkiLog::System(
			FString::Printf(TEXT("Cannot transfer character '%s' - not controlled by this player"), 
				*Character->GetName()),
			EKalkiLogSeverity::Warning
		);
		return false;
	}

	// Get target player's selection component
	AKalkiPlayerController* ToKalkiPC = Cast<AKalkiPlayerController>(ToController);
	if (!ToKalkiPC || !ToKalkiPC->CharacterSelectionComponent)
	{
		KalkiLog::System(
			TEXT("TransferCharacterTo - ToController doesn't have CharacterSelectionComponent"),
			EKalkiLogSeverity::Warning
		);
		return false;
	}

	// Remove from this player
	if (!UnassignCharacter(Character))
	{
		return false;
	}

	// Assign to target player
	return ToKalkiPC->CharacterSelectionComponent->AssignCharacter(Character);
}

void UKalkiCharacterSelectionComponent::ClearAllAssignments()
{
	if (SelectedCharacter)
	{
		DeselectCharacter();
	}

	int32 Count = ControlledCharacters.Num();
	ControlledCharacters.Empty();

	KalkiLog::System(
		FString::Printf(TEXT("All character assignments cleared (%d characters released)"), Count)
	);
}

bool UKalkiCharacterSelectionComponent::IsControllingCharacter(AKalkiCharacter* Character) const
{
	return Character && ControlledCharacters.Contains(Character);
}

// ========================================
// CHARACTER SELECTION
// ========================================

void UKalkiCharacterSelectionComponent::SelectCharacter(AKalkiCharacter* Character)
{
	if (!Character)
	{
		KalkiLog::System(
			TEXT("CharacterSelectionComponent - Cannot select null character"),
			EKalkiLogSeverity::Warning
		);
		return;
	}

	// Validate control
	if (!ValidateCharacterControl(Character))
	{
		return;
	}

	// Deselect previous character
	if (SelectedCharacter)
	{
		// TODO: Clear visual feedback on old character
		KalkiLog::System(
			FString::Printf(TEXT("Deselecting character: %s"), *SelectedCharacter->GetName()),
			EKalkiLogSeverity::Verbose
		);
	}

	// Select new character
	SelectedCharacter = Character;

	// TODO: Show visual feedback on new character
	KalkiLog::System(
		FString::Printf(TEXT("Character selected: %s"), *Character->GetName())
	);

	// Show movement range
	if (bShowMovementRangeOnSelection)
	{
		UpdateMovementRangeDisplay();
	}
}

void UKalkiCharacterSelectionComponent::DeselectCharacter()
{
	if (!SelectedCharacter)
	{
		return;
	}

	KalkiLog::System(
		FString::Printf(TEXT("Character deselected: %s"), *SelectedCharacter->GetName()),
		EKalkiLogSeverity::Verbose
	);

	// TODO: Clear visual feedback
	SelectedCharacter = nullptr;

	// Clear movement range
	ClearMovementRangeDisplay();
}

// ========================================
// QUICK SELECTION (Number Keys)
// ========================================

void UKalkiCharacterSelectionComponent::SelectCharacterBySlot(int32 SlotIndex)
{
	if (!ControlledCharacters.IsValidIndex(SlotIndex))
	{
		// Verbose logging - pressing empty slots is expected behavior
		KalkiLog::System(
			FString::Printf(TEXT("Slot %d empty (Controlled: %d)"), 
				SlotIndex + 1, // Display as 1-6 instead of 0-5
				ControlledCharacters.Num()),
			EKalkiLogSeverity::Verbose
		);
		return;
	}

	SelectCharacter(ControlledCharacters[SlotIndex]);
}

void UKalkiCharacterSelectionComponent::SelectNextCharacter()
{
	if (ControlledCharacters.Num() == 0)
	{
		return;
	}

	if (ControlledCharacters.Num() == 1)
	{
		// Only one character, reselect it
		SelectCharacter(ControlledCharacters[0]);
		return;
	}

	int32 CurrentIndex = ControlledCharacters.IndexOfByKey(SelectedCharacter);
	int32 NextIndex = (CurrentIndex + 1) % ControlledCharacters.Num();

	SelectCharacter(ControlledCharacters[NextIndex]);
}

void UKalkiCharacterSelectionComponent::SelectPreviousCharacter()
{
	if (ControlledCharacters.Num() == 0)
	{
		return;
	}

	if (ControlledCharacters.Num() == 1)
	{
		// Only one character, reselect it
		SelectCharacter(ControlledCharacters[0]);
		return;
	}

	int32 CurrentIndex = ControlledCharacters.IndexOfByKey(SelectedCharacter);
	int32 PrevIndex = (CurrentIndex - 1 + ControlledCharacters.Num()) % ControlledCharacters.Num();

	SelectCharacter(ControlledCharacters[PrevIndex]);
}

AKalkiCharacter* UKalkiCharacterSelectionComponent::GetCharacterInSlot(int32 SlotIndex) const
{
	if (ControlledCharacters.IsValidIndex(SlotIndex))
	{
		return ControlledCharacters[SlotIndex];
	}
	return nullptr;
}

int32 UKalkiCharacterSelectionComponent::GetCharacterSlotIndex(AKalkiCharacter* Character) const
{
	return ControlledCharacters.IndexOfByKey(Character);
}

// ========================================
// MOVEMENT RANGE
// ========================================

void UKalkiCharacterSelectionComponent::UpdateMovementRangeDisplay()
{
	if (!SelectedCharacter || !GridVisualizer || !GridManager)
	{
		return;
	}

	// ✅ UPDATED - Use interface to get grid position
	IKalkiGridOccupant* GridOccupant = Cast<IKalkiGridOccupant>(SelectedCharacter);
	if (!GridOccupant)
	{
		KalkiLog::System(
			TEXT("Selected character doesn't implement IKalkiGridOccupant"),
			EKalkiLogSeverity::Warning
		);
		return;
	}

	// Get character's grid position via interface
	FKalkiGridCoord CharacterPos = IKalkiGridOccupant::Execute_GetGridPosition(SelectedCharacter);
	
	if (!CharacterPos.IsValid())
	{
		KalkiLog::System(
			FString::Printf(TEXT("Selected character '%s' is not on grid"), 
				*SelectedCharacter->GetName()),
			EKalkiLogSeverity::Verbose
		);
		return;
	}

	// ✅ NEW - Get movement range from character
	int32 BaseMovementRange = DefaultMovementRange;
	
	// Try to get actual movement range from character
	AKalkiCharacter* KalkiChar = Cast<AKalkiCharacter>(SelectedCharacter);
	if (KalkiChar)
	{
		BaseMovementRange = KalkiChar->GetMovementRange();
	}

	// ✅ NEW - Create movement tiers (D&D 5e style: Normal + Dash)
	TArray<FKalkiMovementTier> MovementTiers;

	// Normal Move (1x base movement)
	FKalkiMovementTier NormalTier;
	NormalTier.Range = BaseMovementRange;
	NormalTier.BorderColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.8f); // Cyan
	NormalTier.TierName = TEXT("Normal");
	NormalTier.ZOffset = 5.0f;
	MovementTiers.Add(NormalTier);

	// Dash Move (2x base movement)
	FKalkiMovementTier DashTier;
	DashTier.Range = BaseMovementRange * 2;
	DashTier.BorderColor = FLinearColor(1.0f, 0.9f, 0.2f, 0.8f); // Yellow
	DashTier.TierName = TEXT("Dash");
	DashTier.ZOffset = 6.0f;
	MovementTiers.Add(DashTier);

	// Show movement range with tiers
	GridVisualizer->ShowMovementRange(CharacterPos, MovementTiers);

	KalkiLog::System(
		FString::Printf(TEXT("Showing movement range for %s: Normal=%d, Dash=%d tiles"), 
			*SelectedCharacter->GetName(),
			BaseMovementRange,
			BaseMovementRange * 2),
		EKalkiLogSeverity::Verbose
	);
}

void UKalkiCharacterSelectionComponent::ClearMovementRangeDisplay()
{
	if (GridVisualizer)
	{
		GridVisualizer->HideMovementRange();
	}
}

// ========================================
// INTERNAL FUNCTIONS
// ========================================

void UKalkiCharacterSelectionComponent::CacheReferences()
{
	// Get owning controller
	OwningController = Cast<APlayerController>(GetOwner());
	if (!OwningController)
	{
		KalkiLog::System(
			TEXT("CharacterSelectionComponent - Owner is not a PlayerController!"),
			EKalkiLogSeverity::Error,
			this
		);
		return;
	}

	// Get grid manager
	GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
	if (!GridManager)
	{
		KalkiLog::System(
			TEXT("CharacterSelectionComponent - GridManager not found!"),
			EKalkiLogSeverity::Warning,
			this
		);
	}

	// Find grid visualizer
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiGridVisualizer::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		GridVisualizer = Cast<AKalkiGridVisualizer>(FoundActors[0]);
		
		if (GridVisualizer)
		{
			KalkiLog::System(TEXT("CharacterSelectionComponent - Found GridVisualizer"));
		}
	}
}

bool UKalkiCharacterSelectionComponent::ValidateCharacterControl(AKalkiCharacter* Character) const
{
	if (!IsControllingCharacter(Character))
	{
		KalkiLog::System(
			FString::Printf(TEXT("Cannot select character '%s' - not controlled by this player"), 
				*Character->GetName()),
			EKalkiLogSeverity::Warning
		);
		return false;
	}

	return true;
}