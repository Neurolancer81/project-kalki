// Copyright of V.S. Puranam and no one else

#include "Debug/KalkiCheatManager.h"
#include "Player/KalkiPlayerController.h"
#include "Player/Components/KalkiCharacterSelectionComponent.h"
#include "Characters/KalkiCharacter.h"
#include "Grid/KalkiGridManager.h"
#include "Grid/Components/KalkiGridOccupancyComponent.h"
#include "Grid/Components/KalkiGridOccupant.h"
#include "AbilitySystem/KalkiAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Logging/KalkiLog.h"


// ========================================
// GRID COMMANDS
// ========================================

void UKalkiCheatManager::SpawnTestCharacter(int32 X, int32 Y)
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC)
	{
		KalkiLog::System(TEXT("SpawnTestCharacter - No PlayerController"), EKalkiLogSeverity::Warning);
		return;
	}

	UWorld* World = PC->GetWorld();
	if (!World)
	{
		return;
	}

	// Get grid manager
	UKalkiGridManager* GridManager = GetGridManager();
	if (!GridManager)
	{
		KalkiLog::System(TEXT("SpawnTestCharacter - No GridManager"), EKalkiLogSeverity::Warning);
		return;
	}

	// Get spawn location from grid coordinate
	FKalkiGridCoord SpawnCoord(X, Y);
	if (!GridManager->IsValidCoord(SpawnCoord))
	{
		KalkiLog::System(
			FString::Printf(TEXT("SpawnTestCharacter - Invalid coord: (%d, %d)"), X, Y),
			EKalkiLogSeverity::Warning
		);
		return;
	}

	FVector SpawnLocation = GridManager->CoordToWorldPosition(SpawnCoord);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// Spawn character
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AKalkiCharacter* NewCharacter = World->SpawnActor<AKalkiCharacter>(
		AKalkiCharacter::StaticClass(),
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (NewCharacter)
	{
		// Character will auto-place via BeginPlay
		
		// Assign to player
		if (PC->CharacterSelectionComponent)
		{
			PC->CharacterSelectionComponent->AssignCharacter(NewCharacter);
		}

		KalkiLog::System(
			FString::Printf(TEXT("Spawned test character at grid: (%d, %d)"), X, Y)
		);
	}
	else
	{
		KalkiLog::System(TEXT("Failed to spawn test character"), EKalkiLogSeverity::Error);
	}
}

void UKalkiCheatManager::PlaceCharacterOnGrid(int32 X, int32 Y)
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("PlaceCharacterOnGrid - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// Get grid component
	UKalkiGridOccupancyComponent* GridComp = Character->GetGridOccupancyComponent_Implementation();
	if (!GridComp)
	{
		KalkiLog::System(TEXT("PlaceCharacterOnGrid - Character has no GridOccupancyComponent"), EKalkiLogSeverity::Error);
		return;
	}

	// Place on grid
	FKalkiGridCoord Coord(X, Y);
	if (GridComp->PlaceOnGrid(Coord))
	{
		KalkiLog::System(
			FString::Printf(TEXT("Placed %s on grid at: (%d, %d)"), 
				*Character->GetName(), X, Y)
		);
	}
	else
	{
		KalkiLog::System(
			FString::Printf(TEXT("Failed to place %s at: (%d, %d)"), 
				*Character->GetName(), X, Y),
			EKalkiLogSeverity::Warning
		);
	}
}

void UKalkiCheatManager::RemoveCharacterFromGrid()
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("RemoveCharacterFromGrid - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// Get grid component
	UKalkiGridOccupancyComponent* GridComp = Character->GetGridOccupancyComponent_Implementation();
	if (!GridComp)
	{
		KalkiLog::System(TEXT("RemoveCharacterFromGrid - Character has no GridOccupancyComponent"), EKalkiLogSeverity::Error);
		return;
	}

	GridComp->RemoveFromGrid();

	KalkiLog::System(
		FString::Printf(TEXT("Removed %s from grid"), *Character->GetName())
	);
}

void UKalkiCheatManager::PrintCharacterGridPosition()
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("PrintCharacterGridPosition - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	PrintCharacterDetails(Character);
}

void UKalkiCheatManager::SnapAllCharactersToGrid()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UKalkiGridManager* GridManager = GetGridManager();
	if (!GridManager)
	{
		KalkiLog::System(TEXT("SnapAllCharactersToGrid - Grid not created"), EKalkiLogSeverity::Warning);
		return;
	}

	// Find all characters in level
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(World, AKalkiCharacter::StaticClass(), FoundCharacters);

	if (FoundCharacters.Num() == 0) return;
	
	int32 SnappedCount = 0;
	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (!Character)
		{
			continue;
		}

		// Force snap
		if (Character->SnapToNearestTile())
		{
			SnappedCount++;
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("Snapped %d characters to grid tiles"), SnappedCount)
	);
}

void UKalkiCheatManager::SnapSelectedCharacter()
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("SnapSelectedCharacter - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	if (Character->SnapToNearestTile())
	{
		KalkiLog::System(
			FString::Printf(TEXT("Snapped %s to nearest tile"), *Character->GetName())
		);
	}
	else
	{
		KalkiLog::System(
			FString::Printf(TEXT("Failed to snap %s"), *Character->GetName()),
			EKalkiLogSeverity::Warning
		);
	}
}

// ========================================
// CHARACTER CONTROL COMMANDS
// ========================================

void UKalkiCheatManager::AssignAllCharacters()
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC || !PC->CharacterSelectionComponent)
	{
		KalkiLog::System(TEXT("AssignAllCharacters - No CharacterSelectionComponent"), EKalkiLogSeverity::Warning);
		return;
	}

	// Find all characters in level
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(PC->GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	int32 AssignedCount = 0;
	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (Character)
		{
			if (PC->CharacterSelectionComponent->AssignCharacter(Character))
			{
				AssignedCount++;
			}
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("Assigned %d characters to player"), AssignedCount)
	);
}

void UKalkiCheatManager::AssignCharacterByName(const FString& CharacterName)
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC || !PC->CharacterSelectionComponent)
	{
		KalkiLog::System(TEXT("AssignCharacterByName - No CharacterSelectionComponent"), EKalkiLogSeverity::Warning);
		return;
	}

	// Find all characters in level
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(PC->GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (Character && Character->GetName().Contains(CharacterName))
		{
			if (PC->CharacterSelectionComponent->AssignCharacter(Character))
			{
				KalkiLog::System(
					FString::Printf(TEXT("Assigned character: %s"), *Character->GetName())
				);
				return;
			}
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("Character not found: %s"), *CharacterName),
		EKalkiLogSeverity::Warning
	);
}

void UKalkiCheatManager::TestSelectCharacter(int32 SlotIndex)
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC || !PC->CharacterSelectionComponent)
	{
		KalkiLog::System(TEXT("TestSelectCharacter - No CharacterSelectionComponent"), EKalkiLogSeverity::Warning);
		return;
	}

	PC->CharacterSelectionComponent->SelectCharacterBySlot(SlotIndex);
}

void UKalkiCheatManager::TestCycleCharacter()
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC || !PC->CharacterSelectionComponent)
	{
		KalkiLog::System(TEXT("TestCycleCharacter - No CharacterSelectionComponent"), EKalkiLogSeverity::Warning);
		return;
	}

	PC->CharacterSelectionComponent->SelectNextCharacter();
}

void UKalkiCheatManager::PrintControlledCharacters()
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC || !PC->CharacterSelectionComponent)
	{
		KalkiLog::System(TEXT("PrintControlledCharacters - No CharacterSelectionComponent"), EKalkiLogSeverity::Warning);
		return;
	}

	UKalkiCharacterSelectionComponent* SelectionComp = PC->CharacterSelectionComponent;
	
	KalkiLog::System(TEXT("========================================"));
	KalkiLog::System(TEXT("CONTROLLED CHARACTERS"));
	KalkiLog::System(TEXT("========================================"));
	
	int32 Count = SelectionComp->GetControlledCharacterCount();
	KalkiLog::System(FString::Printf(TEXT("Total Controlled: %d"), Count));
	
	if (Count == 0)
	{
		KalkiLog::System(TEXT("No characters assigned"));
	}
	else
	{
		TArray<AKalkiCharacter*> Characters = SelectionComp->GetControlledCharacters();
		for (int32 i = 0; i < Characters.Num(); ++i)
		{
			AKalkiCharacter* Character = Characters[i];
			bool bSelected = (Character == SelectionComp->GetSelectedCharacter());
			
			FKalkiGridCoord GridPos = IKalkiGridOccupant::Execute_GetGridPosition(Character);
			
			KalkiLog::System(
				FString::Printf(TEXT("  [%d] %s - Grid: %s %s"), 
					i + 1,
					*Character->GetName(),
					*GridPos.ToString(),
					bSelected ? TEXT("(SELECTED)") : TEXT(""))
			);
		}
	}
	
	if (SelectionComp->HasCharacterSelected())
	{
		KalkiLog::System(
			FString::Printf(TEXT("Currently Selected: %s"), 
				*SelectionComp->GetSelectedCharacter()->GetName())
		);
	}
	else
	{
		KalkiLog::System(TEXT("No character selected"));
	}
	
	KalkiLog::System(TEXT("========================================"));
}

// ========================================
// CHARACTER DEBUG COMMANDS
// ========================================

void UKalkiCheatManager::ToggleCharacterDebug()
{
	// Find all characters
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (Character)
		{
			// Toggle debug for this frame
			DrawCharacterStats(Character);
		}
	}

	KalkiLog::System(TEXT("Drew debug stats for all characters (one frame)"));
}

void UKalkiCheatManager::ToggleSelectedCharacterDebug()
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("ToggleSelectedCharacterDebug - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	DrawCharacterStats(Character);
}

void UKalkiCheatManager::ShowAllCharacterStats()
{
	// Find all characters
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	if (FoundCharacters.Num() == 0)
	{
		KalkiLog::System(TEXT("No characters found in level"), EKalkiLogSeverity::Warning);
		return;
	}

	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (Character)
		{
			DrawCharacterStats(Character);
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("Drew stats for %d characters"), FoundCharacters.Num())
	);
}

void UKalkiCheatManager::ShowCharacterGridAlignment()
{
	// Find all characters
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	if (FoundCharacters.Num() == 0)
	{
		KalkiLog::System(TEXT("No characters found in level"), EKalkiLogSeverity::Warning);
		return;
	}

	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (Character)
		{
			DrawCharacterGridAlignment(Character);
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("Drew grid alignment for %d characters"), FoundCharacters.Num())
	);
}

void UKalkiCheatManager::PrintCharacterInfo(const FString& CharacterName)
{
	// Find all characters
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	for (AActor* Actor : FoundCharacters)
	{
		AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
		if (Character && Character->GetName().Contains(CharacterName))
		{
			PrintCharacterDetails(Character);
			return;
		}
	}

	KalkiLog::System(
		FString::Printf(TEXT("Character not found: %s"), *CharacterName),
		EKalkiLogSeverity::Warning
	);
}

void UKalkiCheatManager::PrintAllCharacters()
{
	// Find all characters
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiCharacter::StaticClass(), FoundCharacters);

	KalkiLog::System(TEXT("========================================"));
	KalkiLog::System(TEXT("ALL CHARACTERS IN LEVEL"));
	KalkiLog::System(TEXT("========================================"));
	KalkiLog::System(FString::Printf(TEXT("Total: %d"), FoundCharacters.Num()));
	KalkiLog::System(TEXT(""));

	if (FoundCharacters.Num() == 0)
	{
		KalkiLog::System(TEXT("No characters found"));
	}
	else
	{
		for (AActor* Actor : FoundCharacters)
		{
			AKalkiCharacter* Character = Cast<AKalkiCharacter>(Actor);
			if (Character)
			{
				FKalkiGridCoord GridPos = IKalkiGridOccupant::Execute_GetGridPosition(Character);
				FVector WorldPos = Character->GetActorLocation();

				KalkiLog::System(
					FString::Printf(TEXT("  %s"), *Character->GetName())
				);
				KalkiLog::System(
					FString::Printf(TEXT("    Grid: %s"), *GridPos.ToString())
				);
				KalkiLog::System(
					FString::Printf(TEXT("    World: (%.0f, %.0f, %.0f)"), 
						WorldPos.X, WorldPos.Y, WorldPos.Z)
				);
				KalkiLog::System(TEXT(""));
			}
		}
	}

	KalkiLog::System(TEXT("========================================"));
}

// ========================================
// HEALTH DEBUG COMMANDS
// ========================================

void UKalkiCheatManager::DamageCharacter(float Amount)
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("DamageCharacter - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// ✅ UPDATED - Use GAS properly
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	UKalkiAttributeSet* AttributeSet = Character->GetAttributeSet();
	
	if (!ASC || !AttributeSet)
	{
		KalkiLog::System(TEXT("DamageCharacter - No ASC or AttributeSet"), EKalkiLogSeverity::Error);
		return;
	}

	float CurrentHealth = AttributeSet->GetHealth();
	float NewHealth = FMath::Max(0.0f, CurrentHealth - FMath::Abs(Amount));
	
	ASC->SetNumericAttributeBase(
		UKalkiAttributeSet::GetHealthAttribute(),
		NewHealth
	);

	KalkiLog::System(
		FString::Printf(TEXT("Damaged %s for %.0f damage (HP: %.0f -> %.0f)"), 
			*Character->GetName(), 
			FMath::Abs(Amount),
			CurrentHealth,
			NewHealth)
	);
}

void UKalkiCheatManager::HealCharacter(float Amount)
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("HealCharacter - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// ✅ UPDATED - Use GAS properly
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	UKalkiAttributeSet* AttributeSet = Character->GetAttributeSet();
	
	if (!ASC || !AttributeSet)
	{
		KalkiLog::System(TEXT("HealCharacter - No ASC or AttributeSet"), EKalkiLogSeverity::Error);
		return;
	}

	float CurrentHealth = AttributeSet->GetHealth();
	float MaxHealth = AttributeSet->GetMaxHealth();
	float NewHealth = FMath::Min(MaxHealth, CurrentHealth + FMath::Abs(Amount));
	
	ASC->SetNumericAttributeBase(
		UKalkiAttributeSet::GetHealthAttribute(),
		NewHealth
	);

	KalkiLog::System(
		FString::Printf(TEXT("Healed %s for %.0f HP (HP: %.0f -> %.0f)"), 
			*Character->GetName(), 
			FMath::Abs(Amount),
			CurrentHealth,
			NewHealth)
	);
}

void UKalkiCheatManager::SetCharacterHealth(float Health)
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("SetCharacterHealth - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// ✅ UPDATED - Use GAS properly
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	UKalkiAttributeSet* AttributeSet = Character->GetAttributeSet();
	
	if (!ASC || !AttributeSet)
	{
		KalkiLog::System(TEXT("SetCharacterHealth - No ASC or AttributeSet"), EKalkiLogSeverity::Error);
		return;
	}

	float MaxHealth = AttributeSet->GetMaxHealth();
	float ClampedHealth = FMath::Clamp(Health, 0.0f, MaxHealth);
	
	ASC->SetNumericAttributeBase(
		UKalkiAttributeSet::GetHealthAttribute(),
		ClampedHealth
	);

	KalkiLog::System(
		FString::Printf(TEXT("Set %s health to %.0f"), 
			*Character->GetName(), ClampedHealth)
	);
}

void UKalkiCheatManager::KillCharacter()
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("KillCharacter - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// ✅ UPDATED - Use GAS properly
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	
	if (!ASC)
	{
		KalkiLog::System(TEXT("KillCharacter - No ASC"), EKalkiLogSeverity::Error);
		return;
	}

	ASC->SetNumericAttributeBase(
		UKalkiAttributeSet::GetHealthAttribute(),
		0.0f
	);

	KalkiLog::System(
		FString::Printf(TEXT("Killed %s"), *Character->GetName())
	);
}

void UKalkiCheatManager::FullHealCharacter()
{
	AKalkiCharacter* Character = GetSelectedCharacter();
	if (!Character)
	{
		KalkiLog::System(TEXT("FullHealCharacter - No character selected"), EKalkiLogSeverity::Warning);
		return;
	}

	// ✅ UPDATED - Use GAS properly
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	UKalkiAttributeSet* AttributeSet = Character->GetAttributeSet();
	
	if (!ASC || !AttributeSet)
	{
		KalkiLog::System(TEXT("FullHealCharacter - No ASC or AttributeSet"), EKalkiLogSeverity::Error);
		return;
	}

	float MaxHealth = AttributeSet->GetMaxHealth();
	
	ASC->SetNumericAttributeBase(
		UKalkiAttributeSet::GetHealthAttribute(),
		MaxHealth
	);

	KalkiLog::System(
		FString::Printf(TEXT("Full healed %s to %.0f HP"), 
			*Character->GetName(), MaxHealth)
	);
}

// ========================================
// HELPER FUNCTIONS
// ========================================

AKalkiCharacter* UKalkiCheatManager::GetSelectedCharacter()
{
	AKalkiPlayerController* PC = Cast<AKalkiPlayerController>(GetOuterAPlayerController());
	if (!PC || !PC->CharacterSelectionComponent)
	{
		return nullptr;
	}

	return PC->CharacterSelectionComponent->GetSelectedCharacter();
}

UKalkiGridManager* UKalkiCheatManager::GetGridManager()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	return World->GetSubsystem<UKalkiGridManager>();
}

void UKalkiCheatManager::DrawCharacterStats(AKalkiCharacter* Character)
{
	if (!Character)
	{
		return;
	}

	UKalkiAttributeSet* AttributeSet = Character->GetAttributeSet();
	if (!AttributeSet)
	{
		return;
	}

	FVector Location = Character->GetActorLocation() + FVector(0, 0, 100);

	// Get grid status
	FString GridStatus = TEXT("Not on grid");
	FColor StatusColor = FColor::Red;
	
	UKalkiGridOccupancyComponent* GridComp = Character->GetGridOccupancyComponent_Implementation();
	if (GridComp && GridComp->IsOnGrid())
	{
		FKalkiGridCoord GridPos = IKalkiGridOccupant::Execute_GetGridPosition(Character);
		
		// Check alignment
		UKalkiGridManager* GridManager = GetGridManager();
		if (GridManager)
		{
			FVector TileCenter = GridManager->CoordToWorldPosition(GridPos);
			float DistanceFromCenter = FVector::Dist2D(Character->GetActorLocation(), TileCenter);
			
			if (DistanceFromCenter < 1.0f)
			{
				GridStatus = FString::Printf(TEXT("Grid: %s "), *GridPos.ToString());
				StatusColor = FColor::Green;
			}
			else
			{
				GridStatus = FString::Printf(TEXT("Grid: %s (OFF BY %.0f units)"), 
					*GridPos.ToString(), DistanceFromCenter);
				StatusColor = FColor::Yellow;
			}
		}
		else
		{
			GridStatus = FString::Printf(TEXT("Grid: %s"), *GridPos.ToString());
			StatusColor = FColor::White;
		}
	}

	FString DebugText = FString::Printf(
		TEXT("%s\n")
		TEXT("%s\n")
		TEXT("HP: %.0f/%.0f  AC: %.0f  Move: %d\n")
		TEXT("STR: %.0f  DEX: %.0f  CON: %.0f\n")
		TEXT("INT: %.0f  WIS: %.0f  CHA: %.0f"),
		*Character->GetName(),
		*GridStatus,
		AttributeSet->GetHealth(),
		AttributeSet->GetMaxHealth(),
		AttributeSet->GetArmorClass(),
		Character->GetMovementRange(),
		AttributeSet->GetStrength(),
		AttributeSet->GetDexterity(),
		AttributeSet->GetConstitution(),
		AttributeSet->GetIntelligence(),
		AttributeSet->GetWisdom(),
		AttributeSet->GetCharisma()
	);

	DrawDebugString(GetWorld(), Location, DebugText, nullptr, StatusColor, 5.0f, true, 1.2f);
}

void UKalkiCheatManager::DrawCharacterGridAlignment(AKalkiCharacter* Character)
{
	if (!Character)
	{
		return;
	}

	UKalkiGridOccupancyComponent* GridComp = Character->GetGridOccupancyComponent_Implementation();
	if (!GridComp || !GridComp->IsOnGrid())
	{
		return;
	}

	UKalkiGridManager* GridManager = GetGridManager();
	if (!GridManager)
	{
		return;
	}

	FKalkiGridCoord GridPos = IKalkiGridOccupant::Execute_GetGridPosition(Character);
	FVector TileCenter = GridManager->CoordToWorldPosition(GridPos);
	FVector CharacterPos = Character->GetActorLocation();
	float DistanceFromCenter = FVector::Dist2D(CharacterPos, TileCenter);
	
	// Draw line from character to tile center
	FColor LineColor = (DistanceFromCenter < 1.0f) ? FColor::Green : FColor::Red;
	
	DrawDebugLine(
		GetWorld(),
		CharacterPos,
		TileCenter,
		LineColor,
		false,
		5.0f,
		0,
		3.0f
	);
	
	// Draw sphere at tile center
	DrawDebugSphere(
		GetWorld(),
		TileCenter,
		25.0f,
		12,
		FColor::Green,
		false,
		5.0f,
		0,
		2.0f
	);
	
	// Draw sphere at character position
	DrawDebugSphere(
		GetWorld(),
		CharacterPos,
		20.0f,
		12,
		LineColor,
		false,
		5.0f,
		0,
		2.0f
	);
	
	// Draw distance text
	FVector MidPoint = (CharacterPos + TileCenter) * 0.5f;
	FString DistanceText = FString::Printf(TEXT("%.1f units"), DistanceFromCenter);
	
	DrawDebugString(
		GetWorld(),
		MidPoint + FVector(0, 0, 50),
		DistanceText,
		nullptr,
		LineColor,
		5.0f,
		true,
		1.0f
	);
}

void UKalkiCheatManager::PrintCharacterDetails(AKalkiCharacter* Character)
{
	if (!Character)
	{
		return;
	}

	UKalkiAttributeSet* AttributeSet = Character->GetAttributeSet();
	FKalkiGridCoord GridPos = IKalkiGridOccupant::Execute_GetGridPosition(Character);
	FVector WorldPos = Character->GetActorLocation();
	
	KalkiLog::System(TEXT("========================================"));
	KalkiLog::System(TEXT("CHARACTER DETAILS"));
	KalkiLog::System(TEXT("========================================"));
	KalkiLog::System(FString::Printf(TEXT("Name: %s"), *Character->GetName()));
	KalkiLog::System(TEXT(""));
	
	// Grid info
	KalkiLog::System(TEXT("GRID POSITION:"));
	if (GridPos.IsValid())
	{
		KalkiLog::System(FString::Printf(TEXT("  Coordinate: %s"), *GridPos.ToString()));
		
		// Check alignment
		UKalkiGridManager* GridManager = GetGridManager();
		if (GridManager)
		{
			FVector TileCenter = GridManager->CoordToWorldPosition(GridPos);
			float Distance = FVector::Dist2D(WorldPos, TileCenter);
			
			if (Distance < 1.0f)
			{
				KalkiLog::System(TEXT("Alignment: Perfect"));
			}
			else
			{
				KalkiLog::System(FString::Printf(TEXT("  Alignment: Off by %.1f units"), Distance));
			}
		}
	}
	else
	{
		KalkiLog::System(TEXT("  Not on grid"));
	}
	
	KalkiLog::System(TEXT(""));
	KalkiLog::System(TEXT("WORLD POSITION:"));
	KalkiLog::System(FString::Printf(TEXT("  X: %.2f"), WorldPos.X));
	KalkiLog::System(FString::Printf(TEXT("  Y: %.2f"), WorldPos.Y));
	KalkiLog::System(FString::Printf(TEXT("  Z: %.2f"), WorldPos.Z));
	
	// Attributes
	if (AttributeSet)
	{
		KalkiLog::System(TEXT(""));
		KalkiLog::System(TEXT("ATTRIBUTES:"));
		KalkiLog::System(FString::Printf(TEXT("  HP: %.0f / %.0f"), 
			AttributeSet->GetHealth(), AttributeSet->GetMaxHealth()));
		KalkiLog::System(FString::Printf(TEXT("  AC: %.0f"), AttributeSet->GetArmorClass()));
		KalkiLog::System(FString::Printf(TEXT("  Movement: %d tiles"), Character->GetMovementRange()));
		KalkiLog::System(TEXT(""));
		KalkiLog::System(FString::Printf(TEXT("  STR: %.0f"), AttributeSet->GetStrength()));
		KalkiLog::System(FString::Printf(TEXT("  DEX: %.0f"), AttributeSet->GetDexterity()));
		KalkiLog::System(FString::Printf(TEXT("  CON: %.0f"), AttributeSet->GetConstitution()));
		KalkiLog::System(FString::Printf(TEXT("  INT: %.0f"), AttributeSet->GetIntelligence()));
		KalkiLog::System(FString::Printf(TEXT("  WIS: %.0f"), AttributeSet->GetWisdom()));
		KalkiLog::System(FString::Printf(TEXT("  CHA: %.0f"), AttributeSet->GetCharisma()));
	}
	
	KalkiLog::System(TEXT("========================================"));
}