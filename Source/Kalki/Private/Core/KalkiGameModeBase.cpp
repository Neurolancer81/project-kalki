// Copyright of V.S. Puranam and no one else


#include "Core/KalkiGameModeBase.h"
#include "Characters/KalkiCharacter.h"
#include "Core/KalkiPlayerController.h"
#include "Core/KalkiPlayerState.h"
#include "Logging/KalkiLog.h"
#include "Logging/KalkiLogSubsystem.h"
#include "UI/Common/KalkiUIManager.h"
#include "UI/Common/KalkiWidgetTheme.h"
#include "UI/CombatLog/KalkiCombatLogViewModel.h"

AKalkiGameModeBase::AKalkiGameModeBase()
{
	// Set default classes
	PlayerControllerClass = AKalkiPlayerController::StaticClass();
	PlayerStateClass = AKalkiPlayerState::StaticClass();
	DefaultPawnClass = AKalkiCharacter::StaticClass();
}

void AKalkiGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// UKalkiLogSubsystem* LogSys = GetWorld()->GetSubsystem<UKalkiLogSubsystem>();    
	// Test combat session
	// LogSys->StartCombatSession(TEXT("Test Combat"));
	// KalkiLog::Combat(TEXT("This message should be tagged with combat session"));
	// LogSys->EndCombatSession();     
	// KalkiLog::Combat(TEXT("This message should NOT be tagged with combat session"));

	// Get UIManager
	UKalkiUIManager* UIManager = GetWorld()->GetSubsystem<UKalkiUIManager>();
	if (UIManager)
	{
		// Register CombatLog ViewModel
		UIManager->RegisterViewModelClass(UKalkiCombatLogViewModel::StaticClass());

		// Load and set theme
		UKalkiWidgetTheme* Theme = LoadObject<UKalkiWidgetTheme>(nullptr, TEXT("/Game/UI/Themes/DA_DefaultTheme"));
		if (Theme)
		{
			UIManager->SetGlobalTheme(Theme);
		}
	}

	
}
